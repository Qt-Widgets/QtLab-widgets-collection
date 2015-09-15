// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef WIDGET_WRAPPER_H_
#define WIDGET_WRAPPER_H_

#include <QFrame>

class QWidget;

class WidgetWrapper : public QFrame {
 public:
  WidgetWrapper(QWidget* parent = 0);
  void setWidget(QWidget *widget, bool own = true);
  inline QWidget* widget() const { return m_widget; }
  inline bool isOwned() const { return m_own; }

  bool eventFilter(QObject* object, QEvent* event) override;

 protected:
  QWidget *m_widget;
  bool m_own;
  QWidget *m_oldParent;
};

#endif  // WIDGET_WRAPPER_H_
