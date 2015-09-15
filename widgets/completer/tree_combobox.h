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

class QPushButton;

class TreeComboBox : public QComboBox {
  Q_OBJECT

 public:
  TreeComboBox(QWidget* parent = 0);

  bool eventFilter(QObject* object, QEvent* event) override;
  void keyPressEvent(QKeyEvent *e) override;
  void showPopup() override;
  void hidePopup() override;

  void mousePressEvent(QMouseEvent *e);

  void doSetEditable(bool editable);

 private slots:
  void on_currentIndexChanged(int index);
  void on_editTextChanged(const QString &);
  void on_activated(int index);
  void on_activated(const QString &);
  void on_highlighted(int index);
  void on_highlighted(const QString &);
  void on_currentIndexChanged(const QString &);
  void on_currentTextChanged(const QString &);

  void on_editingFinished();


 private:
  bool skipNextHide;
  QPushButton* add_more_button { nullptr };
  QLineEdit* search_lineedit { nullptr };
};

#endif  // TREE_COMBOBOX_H_
