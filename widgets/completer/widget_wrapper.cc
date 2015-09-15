// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "./widget_wrapper.h"

#include <QVBoxLayout>
#include <QDebug>

WidgetWrapper::WidgetWrapper(QWidget *parent) :
    QFrame(parent),
    m_widget(0),
    m_own(true),
    m_oldParent(0)
{
  setAttribute(Qt::WA_WindowPropagation);

  if (parentWidget() == NULL)
    setAttribute(Qt::WA_DeleteOnClose);

  setLayout(new QVBoxLayout());
  layout()->setContentsMargins(0,0,0,0);
}

void WidgetWrapper::setWidget(QWidget *widget, bool own)
{
  if (m_widget)
  {
    layout()->removeWidget(m_widget);

    if (m_own)
    {
      m_widget->setParent(0);
      delete m_widget;
    }
    else
    {
      m_widget->setParent(m_oldParent);
    }
  }

  m_widget = widget;
  m_own = own;
  m_oldParent = 0;

  if (m_widget)
  {
    m_oldParent = m_widget->parentWidget();
    m_widget->setParent(this);
    layout()->addWidget(m_widget);
  }
}

bool WidgetWrapper::eventFilter(QObject* object, QEvent* event) {
  qDebug() << "WidgetWrapper::eventFilter: object = " << object << ", event = " << event;
  return false;
}
