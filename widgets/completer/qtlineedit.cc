// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QApplication>
#include <QFocusEvent>

#include "qtlineedit.h"
#include "qtcompleter.h"

QtLineEdit::QtLineEdit(QWidget* parent)
    : QLineEdit(parent) {
  connect(this, SIGNAL(textEdited(const QString &)),
          this, SLOT(on_textEdited(const QString &)));
}

void QtLineEdit::setQtCompleter(QtCompleter* completer) {
  if (completer == completer_)
    return;

  if (completer_) {
    completer_->setWidget(nullptr);
    if (completer_->parent() == this)
      delete completer_;
  }

  completer_ = completer;

  if (!completer_)
    return;

  if (completer_->widget() == nullptr)
    completer_->setWidget(this);
}

QtCompleter* QtLineEdit::qtcompleter() {
  return completer_;
}

void QtLineEdit::focusInEvent(QFocusEvent *e) {
  QLineEdit::focusInEvent(e);

  if (completer_ &&
      ((e->reason() == Qt::MouseFocusReason) ||
       (e->reason() == Qt::TabFocusReason))) {
    completer_->setWidget(this);
    completer_->setFilterPattern(text());
    completer_->complete();
  }
}

void QtLineEdit::focusOutEvent(QFocusEvent *e) {
  QLineEdit::focusOutEvent(e);
}

void QtLineEdit::mouseReleaseEvent(QMouseEvent * e) {
  QLineEdit::mouseReleaseEvent(e);
  mayShowCompleterOnEmptyContent();
}

void QtLineEdit::customEvent(QEvent* e) {
  QLineEdit::customEvent(e);
  if (e->type() == static_cast<QEvent::Type>(kShowCompletionEvent)) {
    // force completion after text is deleted
    completer_->setFilterPattern(text());
    completer_->complete();
  }
}

void QtLineEdit::mayShowCompleterOnEmptyContent() {
  if (text().isEmpty()) {
    timer.singleShot(0, this, SLOT(slot_show_completer_popup()));
  }
}

void QtLineEdit::slot_show_completer_popup() {
  // apparently, complete() works only in event handler
  QApplication::postEvent(
      this,
      new QEvent(static_cast<QEvent::Type>(kShowCompletionEvent)));
}

void QtLineEdit::on_textEdited(const QString& text) {
  if (!completer_ || isReadOnly() || echoMode() != QLineEdit::Normal)
    return;

  completer_->setFilterPattern(text);
  completer_->complete();
}
