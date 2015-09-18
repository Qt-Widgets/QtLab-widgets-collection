// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "./tree_combobox.h"

#include <QLineEdit>
#include <QBoxLayout>
#include <QLayoutItem>
#include <QPushButton>
#include <QDebug>

class QtTreeView : public QTreeView {
 public:
  explicit QtTreeView(QWidget* parent = nullptr) : QTreeView(parent) { }
  int row_height() {
    return rowHeight(model()->index(0, 0));
  }
};

TreeComboBox::TreeComboBox(QWidget* parent)
    : QComboBox(parent)
    , skipNextHide(false) {
  auto treeView = new QtTreeView(this);
  treeView->header()->hide();
  setView(treeView);
  view()->viewport()->installEventFilter(this);
  QComboBox::resize(200, 30);

  qDebug() << "has auto scroll: " << treeView->hasAutoScroll();

  setInsertPolicy(QComboBox::NoInsert);

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(on_currentIndexChanged(int)));

  connect(this, SIGNAL(editTextChanged(const QString &)),
          this, SLOT(on_editTextChanged(const QString &)));
  connect(this, SIGNAL(activated(int)),
          this, SLOT(on_activated(int)));
  connect(this, SIGNAL(activated(const QString &)),
          this, SLOT(on_activated(const QString &)));
  connect(this, SIGNAL(highlighted(int)),
          this, SLOT(on_highlighted(int)));
  connect(this, SIGNAL(highlighted(const QString &)),
          this, SLOT(on_highlighted(const QString &)));
  connect(this, SIGNAL(currentIndexChanged(const QString &)),
          this, SLOT(on_currentIndexChanged(const QString &)));
  connect(this, SIGNAL(currentTextChanged(const QString &)),
          this, SLOT(on_currentTextChanged(const QString &)));
}

bool TreeComboBox::eventFilter(QObject* object, QEvent* event)
{
  // qDebug() << "TreeComboBox::eventFilter: object =" << object << ", event = " << event;

  if (object == view()->viewport()) {
    if (event->type() == QEvent::MouseButtonRelease) {
      qDebug() << "QEvent::MouseButtonRelease......";
      QMouseEvent *m = static_cast<QMouseEvent *>(event);
      QModelIndex index = view()->indexAt(m->pos());
      if (!view()->visualRect(index).contains(m->pos())) {
        if (index.flags() & Qt::ItemIsSelectable) {
          qDebug() << "skipNextHide.......";
          skipNextHide = true;
        }
      }
    }
  }
  return false;
}

void TreeComboBox::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
    case Qt::Key_Up:
      if (e->modifiers() & Qt::ControlModifier)
        break;
      // else fall throuth
    case Qt::Key_PageUp:
      e->ignore();
      return;

    case Qt::Key_Down:
      if ((e->modifiers() & Qt::AltModifier) ||
          (e->modifiers() & Qt::ControlModifier))
        break;
      // else fall through
    case Qt::Key_PageDown:
      // NOTE: maybe we can call view()->moveCursor() here
      // then emit itemSelected(new_index)
      // but currently we do not have this requirement,
      // so just ignore these events
      e->ignore();
      return;
    case Qt::Key_Home:
      if (!lineEdit()) {
        e->ignore();
        return;
      }

    case Qt::Key_End:
      if (!lineEdit()) {
        e->ignore();
        return;
      }
    default:
      // do nothing
      ;
  }

  QComboBox::keyPressEvent(e);
}

void TreeComboBox::wheelEvent(QWheelEvent *e) {
  (void)e;
}

void TreeComboBox::showPopup()
{
  // setRootModelIndex(QModelIndex());
  // TODO: maybe we can expand the tree here to ensure view height
  // TODO: use maxvisible item to calculate max/min height
  auto treeView = dynamic_cast<QtTreeView*>(view());
  int row_height = treeView->row_height();
  int fix_height = row_height * maxVisibleItems();
  // by default tree will show as 1 line high, need to reserve room
  treeView->setMaximumHeight(fix_height);
  treeView->setMinimumHeight(fix_height);
  QWidget* treeView_parent = treeView->parentWidget();
  QBoxLayout* container_layout = qobject_cast<QBoxLayout*>(treeView_parent->layout());
  bool push_button_already_added = false;
  bool search_lineedit_already_added = false;
  if (container_layout) {
    qDebug() << "layout count = " << container_layout->count();
    for (int i = 0; i < container_layout->count(); ++i) {
      QLayoutItem *item = container_layout->itemAt(i);
      if (item->widget()) {
        qDebug() << "widget: " << item->widget();

        if (item->widget() == add_more_button) {
          push_button_already_added = true;
        }

        if (item->widget() == search_lineedit) {
          search_lineedit_already_added = true;
        }
      }
    }

    if (!search_lineedit_already_added) {
      search_lineedit = new QLineEdit("type to search", treeView_parent);
      container_layout->insertWidget(0, search_lineedit);
    }

    if (!push_button_already_added) {
      add_more_button = new QPushButton("Add More......", treeView_parent);
      container_layout->addWidget(add_more_button);
    }
  }
  QComboBox::showPopup();
}

void TreeComboBox::hidePopup()
{
  // setRootModelIndex(view()->currentIndex().parent());
  // setCurrentIndex(view()->currentIndex().row());
  if (skipNextHide) {
    qDebug() << "a hide request ignored......";
    skipNextHide = false;
  } else {
    qDebug() << "hidePopup......";
    QComboBox::hidePopup();
  }
}

void TreeComboBox::mousePressEvent(QMouseEvent *e) {
  qDebug() << "TreeComboBox mouse press event";
  QComboBox::mousePressEvent(e);
}

void TreeComboBox::doSetEditable(bool editable) {
  setEditable(editable);
  if (editable) {
    connect(lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(on_editingFinished()));
  }
}

void TreeComboBox::on_currentIndexChanged(int index) {
  (void)index;
  qDebug() << "currentIndexChanged: index = " << index;


  QModelIndex curr_index = view()->currentIndex();
  qDebug() << curr_index.data();
}

void TreeComboBox::on_editTextChanged(const QString &text) {
  qDebug() << "editTextChanged: text = " << text;
}

void TreeComboBox::on_activated(int index) {
  qDebug() << "activated: index = " << index;
}

void TreeComboBox::on_activated(const QString &text) {
  qDebug() << "activated: text= " << text;
}

void TreeComboBox::on_highlighted(int index) {
  qDebug() << "hilighted: index = " << index;
}

void TreeComboBox::on_highlighted(const QString &text) {
  qDebug() << "highlighted: text = " << text;
}

void TreeComboBox::on_currentIndexChanged(const QString &text) {
  qDebug() << "currentIndexChanged: text= " << text;
}

void TreeComboBox::on_currentTextChanged(const QString &text) {
  qDebug() << "currentTextChanged: text = " << text;
}

void TreeComboBox::on_editingFinished() {
  qDebug() << "editing finished: text = " << lineEdit()->text();
  QModelIndex curr_index = view()->currentIndex();
  qDebug() << "current index: " << curr_index << ", text = " << curr_index.data(Qt::DisplayRole).toString();
}
