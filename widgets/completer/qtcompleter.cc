// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "qtcompleter.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QItemSelection>
#include <QPointer>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QListView>
#include <QTreeView>
#include <QKeyEvent>
#include <QItemDelegate>
#include <QBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QLabel>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class IdentityItemViewContainer : public QFrame
                                , public ICompleterItemViewContainer {
  Q_OBJECT

 public:
  IdentityItemViewContainer(QAbstractItemView* item_view,
                            QtCompleter* completer);
  ~IdentityItemViewContainer() {
    delete item_view_;
  }

  // ICompleterItemViewContainer
  QAbstractItemView* getItemView() const override;
  QWidget* getItemViewContainer() override { return this; }

  void setItemView(QAbstractItemView* item_view);

 private slots:
  void viewDestroyed();
  void mayComplete(const QModelIndex& index);

 private:
  QAbstractItemView* item_view_ { nullptr };
  QtCompleter* completer_ { nullptr };
};

class QtCompleterImpl : public QObject {
  Q_OBJECT

 public:
  QtCompleterImpl() : widget(0) { }
  virtual ~QtCompleterImpl() {
    view_container_->getItemViewContainer()->removeEventFilter(q);
    delete view_container_;
    view_container_ = nullptr;
  }

  void setCurrentIndex(QModelIndex index, bool select = true);
  void showPopup(const QRect& rect);
  QString itemText(const QModelIndex &index) const;

  void setItemView(QAbstractItemView* item_view);
  QWidget* viewContainer(QAbstractItemView* item_view = nullptr);
  QAbstractItemView* itemView();

 public slots:
  void _q_completionSelected(const QItemSelection& selection);
  void _q_complete(QModelIndex index, bool highlighted = false);
  void _q_autoResizePopup(const QModelIndex &parent, int first, int last);

 public:
  QPointer<QWidget> widget;
  QAbstractItemModel* model { nullptr };
  ICompleterItemViewContainer* view_container_ { nullptr };

  bool eatFocusOut { true };
  QRect popupRect;

  QString filter_pattern;
  // TODO(lutts): should be able to set the expect column by the client
  int column { 0 };
  int maxVisibleItems { 10 };
  bool wrap { true };

  QtCompleter* q;

 private:
  QtCompleterImpl(const QtCompleterImpl& other) = delete;
  QtCompleterImpl& operator=(const QtCompleterImpl& other) = delete;
};

QWidget* QtCompleterImpl::viewContainer(QAbstractItemView* item_view) {
  if (!view_container_) {
    view_container_ = new IdentityItemViewContainer(item_view, q);

    auto container_widget = view_container_->getItemViewContainer();
    Qt::FocusPolicy origPolicy = Qt::NoFocus;
    if (widget)
      origPolicy = widget->focusPolicy();
    container_widget->setFocusPolicy(Qt::NoFocus);
    if (widget)
      widget->setFocusPolicy(origPolicy);

    container_widget->setFocusProxy(widget);
    container_widget->installEventFilter(q);

    auto item_view_ = view_container_->getItemView();
    item_view_->setModel(model);
    item_view_->setTextElideMode(Qt::ElideMiddle);

    connect(q, SIGNAL(activated(QModelIndex)),
            container_widget, SLOT(hide()));

    container_widget->hide();
  }

  if (view_container_) {
    view_container_->getItemViewContainer();
  } else {
    return nullptr;
  }
}

QAbstractItemView* QtCompleterImpl::itemView() {
  viewContainer();
  if (view_container_)
    return view_container_->getItemView();
  return nullptr;
}

void QtCompleterImpl::setItemView(QAbstractItemView* item_view) {
  if (view_container_) {
    view_container_->getItemViewContainer()->removeEventFilter(q);
    delete view_container_;
    view_container_ = nullptr;
  }

  viewContainer(item_view);
}

void QtCompleterImpl::setCurrentIndex(QModelIndex index, bool select) {
  auto item_view = itemView();

  if (item_view)
    return;

  if (!select) {
    item_view->selectionModel()->setCurrentIndex(index,
                                                 QItemSelectionModel::NoUpdate);
  } else {
    if (!index.isValid())
      item_view->selectionModel()->clear();
    else
      item_view->selectionModel()->setCurrentIndex(
          index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
  }

  index = item_view->selectionModel()->currentIndex();
  if (!index.isValid())
    item_view->scrollToTop();
  else
    item_view->scrollTo(index, QAbstractItemView::PositionAtTop);
}

void QtCompleterImpl::_q_completionSelected(const QItemSelection& selection) {
  QModelIndex index;
  if (!selection.indexes().isEmpty())
    index = selection.indexes().first();

  _q_complete(index, true);
}

QString QtCompleterImpl::itemText(const QModelIndex &index) const {
  return index.isValid()
      ? model->data(index, Qt::DisplayRole).toString()
      : filter_pattern;
}

void QtCompleterImpl::_q_complete(QModelIndex index, bool highlighted) {
  QString completion = itemText(index);

  if (highlighted) {
    emit q->highlighted(index);
    emit q->highlighted(completion);
  } else {
    emit q->activated(index);
    emit q->activated(completion);
  }
}

void QtCompleterImpl::_q_autoResizePopup(
    const QModelIndex &parent, int first, int last) {
  (void)parent;
  (void)first;
  (void)last;

  auto container = viewContainer();
  if (container || !container->isVisible())
    return;

  showPopup(popupRect);
}

void QtCompleterImpl::showPopup(const QRect& rect) {
  const QRect screen = QApplication::desktop()->availableGeometry(widget);
  Qt::LayoutDirection dir = widget->layoutDirection();
  QPoint pos;
  int rh, w;
  QAbstractItemView* item_view = itemView();
  QWidget* container = viewContainer();

  int visibleItems = 0;
  if (qobject_cast<QTreeView *>(item_view)) {
    visibleItems = maxVisibleItems;
  } else {
    visibleItems = qMin(maxVisibleItems, model->rowCount());
  }
  int h = (item_view->sizeHintForRow(0) * visibleItems  + 3) + 3;
  QScrollBar *hsb = item_view->horizontalScrollBar();
  if (hsb && hsb->isVisible())
    h += item_view->horizontalScrollBar()->sizeHint().height();

  int old_minimumHeight = item_view->minimumHeight();

  h = qMax(h, item_view->minimumHeight());
  item_view->setMinimumHeight(h);
  item_view->setMaximumHeight(h);

  container->layout()->activate();

  if (rect.isValid()) {
    rh = rect.height();
    w = rect.width();
    pos = widget->mapToGlobal(dir == Qt::RightToLeft ? rect.bottomRight() : rect.bottomLeft());
  } else {
    rh = widget->height();
    pos = widget->mapToGlobal(QPoint(0, widget->height() - 2));
    w = widget->width();
  }

  container->setGeometry(pos.x(), pos.y(), w, h);
  if (!container->isVisible())
    container->show();

  item_view->setMinimumHeight(old_minimumHeight);
  h = container->frameGeometry().height();

  if (w > screen.width())
    w = screen.width();
  if ((pos.x() + w) > (screen.x() + screen.width()))
    pos.setX(screen.x() + screen.width() - w);
  if (pos.x() < screen.x())
    pos.setX(screen.x());

  // top is space above the widget or rect
  int top = pos.y() - rh - screen.top() + 2;
  // bottom is space below the pos
  int bottom = screen.bottom() - pos.y();
  if (h > bottom) {  // bottom has no enough space
    // if (top < bottom) then h = bottom
    // if (top > bottom) then if (top > h) h = h; else h = top
    h = qMin(qMax(top, bottom), h);

    if (top > bottom)
      pos.setY(pos.y() - h - rh + 2);  // show above the widget/rect
  }

  container->setGeometry(pos.x(), pos.y(), w, h);

#ifndef Q_OS_MAC
  const bool updatesEnabled = container->updatesEnabled();
  container->setUpdatesEnabled(false);
#endif

  container->raise();
  // item_view->setFocus();
  item_view->scrollTo(item_view->currentIndex(),
      QAbstractItemView::PositionAtCenter);

#ifndef Q_OS_MAC
  container->setUpdatesEnabled(updatesEnabled);
#endif

  container->update();
}

namespace {

class QCompleterItemDelegate : public QItemDelegate {
 public:
  QCompleterItemDelegate(QAbstractItemView *view)
      : QItemDelegate(view), view(view) { }
  void paint(QPainter *p, const QStyleOptionViewItem& opt, const QModelIndex& idx) const {
    QStyleOptionViewItem optCopy = opt;
    optCopy.showDecorationSelected = true;
    if (view->currentIndex() == idx)
      optCopy.state |= QStyle::State_HasFocus;
    QItemDelegate::paint(p, optCopy, idx);
  }

 private:
  QAbstractItemView *view;
};

}  // namespace

IdentityItemViewContainer::IdentityItemViewContainer(
    QAbstractItemView* item_view,
    QtCompleter* completer)
    : QFrame(0, Qt::Popup)
    , completer_(completer) {
  setLineWidth(1);
  setFrameStyle(QFrame::StyledPanel | QFrame::Plain);

  QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
  layout->setSpacing(0);
  layout->setMargin(0);

  setItemView(item_view);
}

QAbstractItemView* IdentityItemViewContainer::getItemView() const {
#ifndef QT_NO_LISTVIEW
  if (!item_view_) {
    QListView *listView = new QListView;
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setModelColumn(completer_->d->column);
    auto that = const_cast<IdentityItemViewContainer*>(this);
    that->setItemView(listView);
  }
#endif // QT_NO_LISTVIEW
  return item_view_;
}

void IdentityItemViewContainer::mayComplete(const QModelIndex& index) {
  if ((index.flags() & Qt::ItemIsEnabled)
      && (index.flags() & Qt::ItemIsSelectable)) {
    completer_->d->_q_complete(index);
  }
}

void IdentityItemViewContainer::setItemView(QAbstractItemView* item_view) {
  if (item_view_) {
    disconnect(item_view_->selectionModel(), 0,
               completer_->d.get(), 0);
    disconnect(item_view_, 0, completer_->d.get(), 0);
  }

  if (item_view_ != item_view) {
    delete item_view_;
    item_view_ = nullptr;
  }

  if (!item_view)
    return;

  if (item_view->model() != completer_->d->model)
    item_view->setModel(completer_->d->model);

  item_view->setParent(this);
  qobject_cast<QBoxLayout*>(layout())->insertWidget(0, item_view);
  qobject_cast<QBoxLayout*>(layout())->addWidget(new QPushButton("haha", this));
  qobject_cast<QBoxLayout*>(layout())->addWidget(new QPushButton("hoho", this));
  // item_view->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  item_view->setItemDelegate(new QCompleterItemDelegate(item_view));

  item_view->setSelectionMode(QAbstractItemView::SingleSelection);
  item_view->setFrameStyle(QFrame::NoFrame);
  item_view->setLineWidth(0);
  item_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

#ifndef QT_NO_LISTVIEW
  if (QListView *listView = qobject_cast<QListView *>(item_view)) {
    listView->setModelColumn(completer_->d->column);
  }
#endif

  connect(item_view, SIGNAL(clicked(const QModelIndex&)),
          this, SLOT(mayComplete(const QModelIndex&)));

  connect(item_view->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          completer_->d.get(), SLOT(_q_completionSelected(QItemSelection)));

  item_view_ = item_view;

  connect(item_view, SIGNAL(destroyed()),
          this, SLOT(viewDestroyed()));
}

void IdentityItemViewContainer::viewDestroyed() {
  item_view_ = nullptr;
}

////////////////////////////////////////////////

QtCompleter::QtCompleter(QObject* parent)
    : QObject(parent)
    , d(make_unique<QtCompleterImpl>()) {
  d->q = this;
}

QtCompleter::QtCompleter(QAbstractItemModel *model, QObject *parent)
    : QObject(parent)
    , d(make_unique<QtCompleterImpl>()) {
  d->q = this;
  setModel(model);
}

QtCompleter::~QtCompleter() = default;

void QtCompleter::setWidget(QWidget* widget) {
  if (d->widget == widget)
    return;

  if (d->widget)
    d->widget->removeEventFilter(this);

  d->widget = widget;

  if (d->widget)
    d->widget->installEventFilter(this);

  auto container = d->viewContainer();
  if (container) {
    qDebug() << __LINE__ << ": hide";
    container->hide();
    container->setFocusProxy(d->widget);
  }
}

QWidget* QtCompleter::widget() const {
  return d->widget;
}

void QtCompleter::setModel(QAbstractItemModel* model) {
  QAbstractItemModel* old_model = d->model;

  if (old_model) {
    if (old_model->QObject::parent() == this) {
      delete old_model;
    } else
      disconnect(old_model, 0, d.get(), 0);
  }

  d->model = model;

  auto item_view = d->itemView();
  if (item_view) {
    item_view->setModel(model);
  }

  connect(model, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
          d.get(), SLOT(_q_autoResizePopup(const QModelIndex &, int, int)));
}

QAbstractItemModel* QtCompleter::model() const {
  return d->model;
}

QAbstractItemView* QtCompleter::itemView() {
  return d->itemView();
}

void QtCompleter::setItemView(QAbstractItemView* item_view) {
  d->setItemView(item_view);
}

int QtCompleter::maxVisibleItems() const {
  return d->maxVisibleItems;
}

void QtCompleter::setMaxVisibleItems(int maxItems) {
  d->maxVisibleItems = maxItems;
}

void QtCompleter::setFilterPattern(const QString& filter_pattern) {
  emit filterPatternChanged(filter_pattern);
}

void QtCompleter::complete(const QRect& rect) {
  Q_ASSERT(d->widget != 0);

  qDebug() << "=====>completer......";

  auto container = d->viewContainer();
  if (!d->model || d->model->rowCount(QModelIndex()) == 0) {
    if (container) {
      qDebug() << __LINE__ << ": hide";
      container->hide(); // no suggestion, hide
    }
    return;
  }

  if (!container)
    return;

  d->showPopup(rect);
  d->popupRect = rect;
}

// TODO(lutts): keypad navigation related code is removed
bool QtCompleter::eventFilter(QObject *o, QEvent *e) {
  if (e->type() != QEvent::Paint)
    qDebug() << "o: " << o << ", e: " << e;
  auto container = d->viewContainer();

  if (d->eatFocusOut && o == d->widget && e->type() == QEvent::FocusOut) {
    if (container && container->isVisible())
      return true;
  }

  if (o != container)
    return QObject::eventFilter(o, e);

  switch (e->type()) {
    case QEvent::KeyPress: {
      QKeyEvent *ke = static_cast<QKeyEvent *>(e);

      auto item_view = d->itemView();

      QModelIndex curIndex = item_view->currentIndex();
      // TODO(lutts): should we normalize curIndex to d->column?
      QModelIndex normalized;
      if (curIndex.isValid() && curIndex.column() != d->column) {
        normalized = d->model->index(curIndex.row(), d->column, curIndex.parent());
      }
      if (normalized.isValid())
        curIndex = normalized;

      QModelIndexList selList = item_view->selectionModel()->selectedIndexes();

      qDebug() << "key: " << ke
               << ", selList.isEmpty: " << selList.isEmpty()
               << ", curIndex: " << curIndex
               << ", cur text: " << curIndex.data(Qt::DisplayRole).toString();

      const int key = ke->key();
      // select the current item
      if ((key == Qt::Key_Up || key == Qt::Key_Down) && selList.isEmpty() && curIndex.isValid()) {
        d->setCurrentIndex(curIndex);
        return true;
      }

      // Handle popup navigation keys. These are hardcoded because up/down might make the
      // widget do something else (lineedit cursor moves to home/end on mac, for instance)
      switch (key) {
        case Qt::Key_End:
        case Qt::Key_Home:
          if (ke->modifiers() & Qt::ControlModifier)
            return false;
          break;

        case Qt::Key_Up:
          qDebug() << "curIndex: " << curIndex << ", parent: " << curIndex.parent();
          if (!curIndex.isValid()) {
            QKeyEvent event(QEvent::KeyPress, Qt::Key_End, Qt::NoModifier);
            (static_cast<QObject *>(item_view))->event(&event);
            return true;
          } else if ((curIndex.row() == 0) && (curIndex.parent().row() == -1)) {
            QKeyEvent event(QEvent::KeyPress, Qt::Key_End, Qt::NoModifier);
            (static_cast<QObject *>(item_view))->event(&event);
            return true;
          }
          return false;

        case Qt::Key_Down:
          qDebug() << "curIndex: " << curIndex;
          if (!curIndex.isValid()) {
            QKeyEvent event(QEvent::KeyPress, Qt::Key_Home, Qt::NoModifier);
            (static_cast<QObject *>(item_view))->event(&event);
            return true;
          } else {
            QKeyEvent event(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
            (static_cast<QObject *>(item_view))->event(&event);
            if (!event.isAccepted()) {
              qDebug() << "down not accepted...";
              QKeyEvent event(QEvent::KeyPress, Qt::Key_Home, Qt::NoModifier);
              (static_cast<QObject *>(item_view))->event(&event);
            } else {
              qDebug() << "down accepted...";
            }
            return true;
          }
          return false;

          // NOTE: Key_Right and Key_Left may be used by widget(e.g. QLineEdit),
          // so we can not filter these keys here

        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
          return false;
      }

      // Send the event to the widget. If the widget accepted the event, do nothing
      // If the widget did not accept the event, provide a default implementation
      d->eatFocusOut = false;
      qDebug() << "send to widget......";
      (static_cast<QObject *>(d->widget))->event(ke);
      d->eatFocusOut = true;
      if (!d->widget || e->isAccepted() || !container->isVisible()) {
        // widget lost focus, hide the popup
        if (d->widget && (!d->widget->hasFocus()))
          container->hide();
        if (e->isAccepted())
          return true;
      }

      qDebug() << "widget not accept the event...";

      // default implementation for keys not handled by the widget when popup is open
      switch (key) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Tab:
          container->hide();
          if (curIndex.isValid())
            d->_q_complete(curIndex);
          break;

        case Qt::Key_F4:
          if (ke->modifiers() & Qt::AltModifier)
            container->hide();
          break;

        case Qt::Key_Backtab:
        case Qt::Key_Escape:
          container->hide();
          break;

        default:
          break;
      }

      return true;
    }

#if 0
    case QEvent::MouseMove:
      qDebug() << "mouse move";
      if (container->isVisible()) {
        QMouseEvent *m = static_cast<QMouseEvent *>(e);
        QModelIndex indexUnderMouse = d->itemView()->indexAt(m->pos());
        if (indexUnderMouse.isValid()) {
          d->itemView()->setCurrentIndex(indexUnderMouse);
        }
      }
      break;
#endif

    case QEvent::MouseButtonPress: {
      if (!container->underMouse()) {
        container->hide();
        return true;
      }
    }
      return false;

    case QEvent::InputMethod:
    case QEvent::ShortcutOverride:
      QApplication::sendEvent(d->widget, e);
      break;

    default:
      return false;
  }
  return false;
}

bool QtCompleter::event(QEvent* ev) {
  return QObject::event(ev);
}

#include "qtcompleter.moc"
