// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <QApplication>
#include <QCompleter>
#include <QEvent>
#include <QDebug>

#include "./recent_all_line_edit.h"

RecentAllLineEdit::RecentAllLineEdit(QWidget* parent)
    : QLineEdit(parent) {
  connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(slot_text_edited(const QString&)));
  connect(this, SIGNAL(editingFinished()), this, SLOT(slot_editingFinished()));
}

void RecentAllLineEdit::focusInEvent(QFocusEvent *e) {
  QLineEdit::focusInEvent(e);
  // force completion when line edit is focued in
  QObject::connect(completer(),
                   SIGNAL(activated(const QModelIndex&)),
                   this, SLOT(slot_completer_activated(const QModelIndex&)),
                   Qt::UniqueConnection);

  completer()->setCompletionPrefix(text());
  completer()->complete();
}

void RecentAllLineEdit::mouseReleaseEvent(QMouseEvent * e) {
  QLineEdit::mouseReleaseEvent(e);
  mayShowCompleterOnEmptyContent();
}

void RecentAllLineEdit::slot_text_edited(const QString&) {
  mayShowCompleterOnEmptyContent();
}

void RecentAllLineEdit::slot_editingFinished() {
  qDebug() << "editing finished with text = " << text();
}

void RecentAllLineEdit::slot_completer_activated(const QModelIndex& index) {
  qDebug() << "activated text = "
           << completer()->completionModel()->data(index, Qt::DisplayRole)
           << " sender = " << sender();
}

void RecentAllLineEdit:: customEvent(QEvent* e) {
  QLineEdit::customEvent(e);
  if (e->type() == static_cast<QEvent::Type>(kShowCompletionEvent)) {
    // force completion after text is deleted
    completer()->setCompletionPrefix(text());
    completer()->complete();
  }
}

void RecentAllLineEdit::slot_call_popup() {
  // apparently, complete() works only in event handler
  QApplication::postEvent(this,
                          new QEvent(static_cast<QEvent::Type>(kShowCompletionEvent)));
}

void RecentAllLineEdit::mayShowCompleterOnEmptyContent() {
  if (text().isEmpty()) {
    timer.singleShot(0, this, SLOT(slot_call_popup()));
  }
}

#include "moc_recent_all_line_edit.cpp"
