// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <QAbstractItemView>
#include <QKeyEvent>

#include "./custom_update_line_edit.h"

LineEditWithCustomUpdateCompleter::LineEditWithCustomUpdateCompleter(QWidget *parent)
    : QLineEdit(parent), c(0) {
}

LineEditWithCustomUpdateCompleter::~LineEditWithCustomUpdateCompleter() {
}

void LineEditWithCustomUpdateCompleter::setCompleter(CustomUpdateCompleter *completer) {
  if (c)
    QObject::disconnect(c, 0, this, 0);

  c = completer;

  if (!c)
    return;

  c->setWidget(this);
  connect(completer, SIGNAL(activated(const QString&)),
          this, SLOT(insertCompletion(const QString&)));
}

CustomUpdateCompleter *LineEditWithCustomUpdateCompleter::completer() const {
  return c;
}

void LineEditWithCustomUpdateCompleter::insertCompletion(const QString& completion) {
  setText(completion);
  selectAll();
}


void LineEditWithCustomUpdateCompleter::keyPressEvent(QKeyEvent *e) {
  if (c && c->popup()->isVisible()) {
    // The following keys are forwarded by the completer to the widget
    switch (e->key()) {
      case Qt::Key_Enter:
      case Qt::Key_Return:
      case Qt::Key_Escape:
      case Qt::Key_Tab:
      case Qt::Key_Backtab:
        e->ignore();
        return;  // Let the completer do default behavior
    }
  }

  bool isShortcut = (e->modifiers() & Qt::ControlModifier) &&
    e->key() == Qt::Key_E;
  if (!isShortcut) {
    // Don't send the shortcut (CTRL-E) to the text edit.
    QLineEdit::keyPressEvent(e);
  }

  if (!c)
    return;

  bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
  if (!isShortcut && !ctrlOrShift && e->modifiers() != Qt::NoModifier) {
    c->popup()->hide();
    return;
  }

  c->update(text());
  c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
}

#include "moc_custom_update_line_edit.cpp"
