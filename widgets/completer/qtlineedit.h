// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef QTLINEEDIT_H_
#define QTLINEEDIT_H_

#include <QLineEdit>
#include <QTimer>
#include <QEvent>

class QtCompleter;

class QtLineEdit : public QLineEdit {
  Q_OBJECT

 public:
  explicit QtLineEdit(QWidget* parent = nullptr);
  virtual ~QtLineEdit() = default;

  void setQtCompleter(QtCompleter* completer);
  QtCompleter* qtcompleter();

 private slots:
  void on_textEdited(const QString& text);
  void slot_show_completer_popup();

 private:
  enum {
    kShowCompletionEvent = QEvent::User
  };

  QtLineEdit(const QtLineEdit& other) = delete;
  QtLineEdit& operator=(const QtLineEdit& other) = delete;

  void focusInEvent(QFocusEvent *e);
  void focusOutEvent(QFocusEvent *e);
  void mouseReleaseEvent(QMouseEvent * e) override;
  void customEvent(QEvent* e);
  void mayShowCompleterOnEmptyContent();

  QtCompleter* completer_ { nullptr };
  QTimer timer;
};


#endif  // QTLINEEDIT_H_
