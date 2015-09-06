// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_QTUI_PREVIEW_COMPLETER_LINE_EDIT_H_
#define TEST_QTUI_PREVIEW_COMPLETER_LINE_EDIT_H_

#include <QLineEdit>
#include <QStringListModel>
#include <QTimer>

/*! Line edit widget with auto completion based on QStringListModel.
  Modified behaviour: completion list will appear even when contents of
  line edit is empty. Full list of options will be showed when line edit
  has focus and is empty.
*/
class Completer_line_edit : public QLineEdit {
  Q_OBJECT
 public:
  explicit Completer_line_edit(QWidget *parent = 0);

  //! Set list of options used for copmletion.
  inline void set_list(QStringList list) { model.setStringList(list); }

 private:
  QStringListModel model;
  void focusInEvent(QFocusEvent *e);
  void mouseDoubleClickEvent(QMouseEvent * e) override;
  void mouseReleaseEvent(QMouseEvent * e) override;
  void customEvent(QEvent* e);

  void bind_completer_activated();

  void maybeShowCompleterOnEmptyContent(bool show_popup = true);

  QTimer timer;

 private slots:
  void slot_text_edited();
  void slot_edit_finished();
  void slot_call_popup();
  void slot_completer_activated(const QModelIndex& index);

};

#endif  // TEST_QTUI_PREVIEW_COMPLETER_LINE_EDIT_H_
