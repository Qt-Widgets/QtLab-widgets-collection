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
#include <QDebug>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

namespace {

class IdentityItemViewContainer : public QFrame
                                , public ICompleterItemViewContainer {
  Q_OBJECT

 public:
  IdentityItemViewContainer(QAbstractItemView* item_view)
      : item_view_(item_view) { }

  QAbstractItemView* getItemView() const {
    return item_view_;
  }

  QWidget* getItemViewContainer() {
    return this;
  }

  void setItemView(QAbstractItemView* item_view) {
    connect(item_view, SIGNAL(destroyed()),
            this, SLOT(viewDestroyed()));
  }

 private slots:
  void viewDestroyed() {
    item_view_ = nullptr;
  }

 private:
  QAbstractItemView* item_view_;
};

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

class QtCompleterImpl : public QObject {
  Q_OBJECT

 public:
  QtCompleterImpl() : widget(0) { }
  virtual ~QtCompleterImpl() { delete popup; }

  void setCurrentIndex(QModelIndex index, bool select = true);
  void showPopup(const QRect& rect);
  QString itemText(const QModelIndex &index) const;

 public slots:
  void _q_completionSelected(const QItemSelection& selection);
  void _q_complete(QModelIndex index, bool highlighted = false);
  void _q_autoResizePopup(const QModelIndex &parent, int first, int last);

 public:
  QPointer<QWidget> widget;
  QAbstractItemModel* model { nullptr };
  QAbstractItemView* popup { nullptr };
  // IComplexCompleterPopup* complex_popup { nullptr };

  bool eatFocusOut;
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

void QtCompleterImpl::setCurrentIndex(QModelIndex index, bool select)
{
  if (!q->popup())
    return;
  if (!select) {
    popup->selectionModel()->setCurrentIndex(index, QItemSelectionModel::NoUpdate);
  } else {
    if (!index.isValid())
      popup->selectionModel()->clear();
    else
      popup->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select
                                               | QItemSelectionModel::Rows);
  }
  index = popup->selectionModel()->currentIndex();
  if (!index.isValid())
    popup->scrollToTop();
  else
    popup->scrollTo(index, QAbstractItemView::PositionAtTop);
}

void QtCompleterImpl::_q_completionSelected(const QItemSelection& selection)
{
  QModelIndex index;
  if (!selection.indexes().isEmpty())
    index = selection.indexes().first();

  _q_complete(index, true);
}

QString QtCompleterImpl::itemText(const QModelIndex &index) const
{
  return index.isValid()
      ? model->data(index, Qt::DisplayRole).toString()
      : filter_pattern;
}

void QtCompleterImpl::_q_complete(QModelIndex index, bool highlighted)
{
  QString completion = itemText(index);

  if (highlighted) {
    emit q->highlighted(index);
    emit q->highlighted(completion);
  } else {
    emit q->activated(index);
    emit q->activated(completion);
  }
}

void QtCompleterImpl::_q_autoResizePopup(const QModelIndex &parent, int first, int last)
{
  (void)parent;
  (void)first;
  (void)last;

  if (popup)
    return;

  if (popup && !popup->isVisible())
    return;

  showPopup(popupRect);
}

void QtCompleterImpl::showPopup(const QRect& rect)
{
  const QRect screen = QApplication::desktop()->availableGeometry(widget);
  Qt::LayoutDirection dir = widget->layoutDirection();
  QPoint pos;
  int rh, w;
  // TODO(lutts): for tree view, we can not use row count
  int visibleItems = 0;
  if (QTreeView *tree_view = qobject_cast<QTreeView *>(popup)) {
    visibleItems = maxVisibleItems;
  } else {
    visibleItems = qMin(maxVisibleItems, popup->model()->rowCount());
  }
  int h = (popup->sizeHintForRow(0) * visibleItems  + 3) + 3;
  QScrollBar *hsb = popup->horizontalScrollBar();
  if (hsb && hsb->isVisible())
    h += popup->horizontalScrollBar()->sizeHint().height();

  if (rect.isValid()) {
    rh = rect.height();
    w = rect.width();
    pos = widget->mapToGlobal(dir == Qt::RightToLeft ? rect.bottomRight() : rect.bottomLeft());
  } else {
    rh = widget->height();
    pos = widget->mapToGlobal(QPoint(0, widget->height() - 2));
    w = widget->width();
  }

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
  h = qMax(h, popup->minimumHeight());
  if (h > bottom) {  // bottom has no enough space
    // if (top < bottom) then h = bottom
    // if (top > bottom) then if (top > h) h = h; else h = top
    h = qMin(qMax(top, bottom), h);

    if (top > bottom)
      pos.setY(pos.y() - h - rh + 2);  // show above the widget/rect
  }

  popup->setGeometry(pos.x(), pos.y(), w, h);

  if (!popup->isVisible())
    popup->show();
}

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
  if (d->widget)
    d->widget->removeEventFilter(this);

  d->widget = widget;

  if (d->widget)
    d->widget->installEventFilter(this);

  if (d->popup) {
    d->popup->hide();
    d->popup->setFocusProxy(d->widget);
  }
}

QWidget* QtCompleter::widget() const {
  return d->widget;
}

void QtCompleter::setModel(QAbstractItemModel* model) {
  QAbstractItemModel* old_model = d->model;

  if (old_model && old_model->QObject::parent() == this)
    delete old_model;

  d->model = model;

  if (d->popup)
    setPopup(d->popup);

  QObject::connect(model, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
                   d.get(), SLOT(_q_autoResizePopup(const QModelIndex &, int, int)));
}

QAbstractItemModel* QtCompleter::model() const {
  return d->model;
}

QAbstractItemView* QtCompleter::popup() const {
#ifndef QT_NO_LISTVIEW
  if (!d->popup) {
    QListView *listView = new QListView;
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setModelColumn(d->column);
    QtCompleter *that = const_cast<QtCompleter*>(this);
    that->setPopup(listView);
  }
#endif // QT_NO_LISTVIEW

  return d->popup;
}

void QtCompleter::setPopup(QAbstractItemView* popup) {
  if (d->popup) {
    QObject::disconnect(d->popup->selectionModel(), 0, this, 0);
    QObject::disconnect(d->popup, 0, this, 0);
  }

  if (d->popup != popup)
    delete d->popup;

  d->popup = nullptr;

  if (!popup)
    return;

  if (popup->model() != d->model)
    popup->setModel(d->model);

  popup->hide();

  Qt::FocusPolicy origPolicy = Qt::NoFocus;
  if (d->widget)
    origPolicy = d->widget->focusPolicy();
  popup->setParent(0, Qt::Popup);
  popup->setFocusPolicy(Qt::NoFocus);
  if (d->widget)
    d->widget->setFocusPolicy(origPolicy);

  popup->setFocusProxy(d->widget);
  popup->installEventFilter(this);
  popup->setItemDelegate(new QCompleterItemDelegate(popup));
#ifndef QT_NO_LISTVIEW
  if (QListView *listView = qobject_cast<QListView *>(popup)) {
    listView->setModelColumn(d->column);
  }
#endif

  QObject::connect(popup, SIGNAL(clicked(QModelIndex)),
                   d.get(), SLOT(_q_complete(QModelIndex)));
  QObject::connect(this, SIGNAL(activated(QModelIndex)),
                   popup, SLOT(hide()));

  QObject::connect(popup->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                   d.get(), SLOT(_q_completionSelected(QItemSelection)));
  d->popup = popup;
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
  // QModelIndex idx = d->model->currentIndex(false);
  Q_ASSERT(d->widget != 0);

  if (!d->model || d->model->rowCount(QModelIndex()) == 0) {
    if (d->popup)
      d->popup->hide(); // no suggestion, hide
    return;
  }

  popup();

  d->showPopup(rect);
  d->popupRect = rect;
}

// TODO(lutts): keypad navigation related code is removed
bool QtCompleter::eventFilter(QObject *o, QEvent *e) {
  if (d->eatFocusOut && o == d->widget && e->type() == QEvent::FocusOut) {
    if (d->popup && d->popup->isVisible())
      return true;
  }

  if (o != d->popup)
    return QObject::eventFilter(o, e);

  switch (e->type()) {
    case QEvent::KeyPress: {
      QKeyEvent *ke = static_cast<QKeyEvent *>(e);

      QModelIndex curIndex = d->popup->currentIndex();
      // TODO(lutts): should we normalize curIndex to d->column?
      QModelIndex normalized;
      if (curIndex.isValid() && curIndex.column() != d->column) {
        normalized = d->model->index(curIndex.row(), d->column, curIndex.parent());
      }
      if (normalized.isValid())
        curIndex = normalized;

      QModelIndexList selList = d->popup->selectionModel()->selectedIndexes();

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
          if (!curIndex.isValid()) {
            if (d->popup) {
              QKeyEvent event(QEvent::KeyPress, Qt::Key_End, Qt::NoModifier);
              (static_cast<QObject *>(d->popup))->event(&event);
            }
            return true;
          } else if ((curIndex.row() == 0) && (curIndex.parent().row() == -1)) {
            if (d->wrap && d->popup) {
              QKeyEvent event(QEvent::KeyPress, Qt::Key_End, Qt::NoModifier);
              (static_cast<QObject *>(d->popup))->event(&event);
            }
            return true;
          }
          return false;

        case Qt::Key_Down:
          if (!curIndex.isValid()) {
            if (d->popup) {
              QKeyEvent event(QEvent::KeyPress, Qt::Key_Home, Qt::NoModifier);
              (static_cast<QObject *>(d->popup))->event(&event);
            }
            return true;
          } else if (d->popup) {
            QKeyEvent event(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
            (static_cast<QObject *>(d->popup))->event(&event);
            if (!event.isAccepted()) {
              QKeyEvent event(QEvent::KeyPress, Qt::Key_Home, Qt::NoModifier);
              (static_cast<QObject *>(d->popup))->event(&event);
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
      (static_cast<QObject *>(d->widget))->event(ke);
      d->eatFocusOut = true;
      if (!d->widget || e->isAccepted() || !d->popup->isVisible()) {
        // widget lost focus, hide the popup
        if (d->widget && (!d->widget->hasFocus()))
          d->popup->hide();
        if (e->isAccepted())
          return true;
      }

      // default implementation for keys not handled by the widget when popup is open
      switch (key) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Tab:
          d->popup->hide();
          if (curIndex.isValid())
            d->_q_complete(curIndex);
          break;

        case Qt::Key_F4:
          if (ke->modifiers() & Qt::AltModifier)
            d->popup->hide();
          break;

        case Qt::Key_Backtab:
        case Qt::Key_Escape:
          d->popup->hide();
          break;

        default:
          break;
      }

      return true;
    }

    case QEvent::MouseButtonPress: {
      if (!d->popup->underMouse()) {
        d->popup->hide();
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
