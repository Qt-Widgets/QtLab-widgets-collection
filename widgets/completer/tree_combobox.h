// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TREE_COMBOBOX_H_
#define TREE_COMBOBOX_H_

#include <QComboBox>
#include <QTreeView>
#include <QEvent>
#include <QMouseEvent>
#include <QModelIndex>
#include <QHeaderView>

class TreeComboBox : public QComboBox {
 public:
  TreeComboBox(QWidget* parent = 0)
      : QComboBox(parent)
      , skipNextHide(false) {
    auto treeView = new QTreeView(this);
    treeView->header()->hide();
    treeView->setMaximumHeight(500); // by default tree will show as 1 line high, need to reserve room
    treeView->setMinimumHeight(500);
    setView(treeView);
    view()->viewport()->installEventFilter(this);
    QComboBox::resize(200, 30);
  }

  bool eventFilter(QObject* object, QEvent* event)
  {
    if (event->type() == QEvent::MouseButtonPress && object == view()->viewport())
    {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      QModelIndex index = view()->indexAt(mouseEvent->pos());
      if (!view()->visualRect(index).contains(mouseEvent->pos()))
        skipNextHide = true;
    }
    return false;
  }

  void showPopup() override
  {
    setRootModelIndex(QModelIndex());
    QComboBox::showPopup();
  }

  void hidePopup() override
  {
    setRootModelIndex(view()->currentIndex().parent());
    setCurrentIndex(view()->currentIndex().row());
    if (skipNextHide)
      skipNextHide = false;
    else
      QComboBox::hidePopup();
  }

 private:
  bool skipNextHide;
};

#endif  // TREE_COMBOBOX_H_
