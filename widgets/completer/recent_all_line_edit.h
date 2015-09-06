// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_QTUI_PREVIEW_RECENT_ALL_LINE_EDIT_H_
#define TEST_QTUI_PREVIEW_RECENT_ALL_LINE_EDIT_H_

#include <QLineEdit>
#include <QTimer>
#include <QEvent>

class QFocusEvent;
class QMouseEvent;

class RecentAllLineEdit : public QLineEdit {
  Q_OBJECT

 public:
  enum {
    kShowCompletionEvent = QEvent::User
  };

  RecentAllLineEdit(QWidget* parent = nullptr);
  virtual ~RecentAllLineEdit() = default;

 private:
  RecentAllLineEdit(const RecentAllLineEdit& other) = delete;
  RecentAllLineEdit& operator=(const RecentAllLineEdit& other) = delete;

  void focusInEvent(QFocusEvent *e) override;
  void mouseReleaseEvent(QMouseEvent * e) override;
  void customEvent(QEvent* e);
  void mayShowCompleterOnEmptyContent();

  QTimer timer;

 private slots:
  void slot_text_edited(const QString& text);
  void slot_editingFinished();
  void slot_call_popup();
  void slot_completer_activated(const QModelIndex& index);
};


#endif  // TEST_QTUI_PREVIEW_RECENT_ALL_LINE_EDIT_H_
