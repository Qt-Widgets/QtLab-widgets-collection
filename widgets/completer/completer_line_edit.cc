// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <QCompleter>
#include <QDebug>
#include <QApplication>

#include "./completer_line_edit.h"

Completer_line_edit::Completer_line_edit(QWidget *parent) :
    QLineEdit(parent)
{
  auto c = new QCompleter();
  c->setModel(&model);
  c->setCompletionMode(QCompleter::PopupCompletion);
  c->setCaseSensitivity(Qt::CaseInsensitive);
  setCompleter(c);
  bind_completer_activated();

  connect(this, SIGNAL(textEdited(QString)), this, SLOT(slot_text_edited()));
  connect(this, SIGNAL(editingFinished()), this, SLOT(slot_edit_finished()));
}

void Completer_line_edit::focusInEvent(QFocusEvent *e) {
  QLineEdit::focusInEvent(e);
  bind_completer_activated();

  // force completion when line edit is focued in
  maybeShowCompleterOnEmptyContent(false);
  completer()->complete();
}

void Completer_line_edit::maybeShowCompleterOnEmptyContent(bool show_popup) {
  static QStringList empty_word_list {
    QStringLiteral("haha"),
        QStringLiteral("hoho"),
        QStringLiteral("hehe"),
        };

  static QStringList non_empty_word_list {
    QStringLiteral("one"),
        QStringLiteral("two"),
        QStringLiteral("three"),
        QStringLiteral("four") };

  // force to show all items when text is empty
  if (text().isEmpty()) {
    model.setStringList(empty_word_list);
    completer()->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    // completion list will be hidden now; we will show it again after a delay
    if (show_popup)
      timer.singleShot(100, this, SLOT(slot_call_popup()));
  } else {
    model.setStringList(non_empty_word_list);
    completer()->setCompletionMode(QCompleter::PopupCompletion);
  }
}

void Completer_line_edit::mouseDoubleClickEvent(QMouseEvent * e) {
  QLineEdit::mouseDoubleClickEvent(e);
  maybeShowCompleterOnEmptyContent();
}

void Completer_line_edit::mouseReleaseEvent(QMouseEvent * e) {
  QLineEdit::mouseReleaseEvent(e);
  maybeShowCompleterOnEmptyContent();
}

void Completer_line_edit::slot_text_edited() {
  maybeShowCompleterOnEmptyContent();
}

void Completer_line_edit::slot_edit_finished() {
  qDebug() << "edit finished: text = " << text();
  //  auto currIdx = completer()->currentIndex();
  qDebug() << "currentCompletion() = " << completer()->currentCompletion();
}

void Completer_line_edit::bind_completer_activated() {
  if (hasFocus()) {
    QObject::connect(completer(),
                     SIGNAL(activated(const QModelIndex&)),
                     this, SLOT(slot_completer_activated(const QModelIndex&)));
  }
}

void Completer_line_edit::slot_completer_activated(const QModelIndex& index) {
  qDebug() << "activated text = " << completer()->completionModel()->data(index, Qt::DisplayRole);
}

void Completer_line_edit::slot_call_popup() {
  // apparently, complete() works only in event handler
  QApplication::postEvent(this, new QEvent(QEvent::User));
}

void Completer_line_edit::customEvent(QEvent *e) {
  QLineEdit::customEvent(e);
  // force completion after text is deleted
  completer()->complete();
}

#include "moc_completer_line_edit.cpp"
