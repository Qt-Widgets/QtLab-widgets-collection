// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_QTUI_PREVIEW_ANOTHER_LINE_EDIT_H_
#define TEST_QTUI_PREVIEW_ANOTHER_LINE_EDIT_H_

#include <QLineEdit>
#include <QStringList>
#include <QStringListModel>
#include <QString>
#include <QCompleter>

class CustomUpdateCompleter : public QCompleter
{
  Q_OBJECT

 public:
  inline CustomUpdateCompleter(const QStringList& words, QObject * parent) :
      QCompleter(parent), m_list(words), m_model()
  {
    setModel(&m_model);
  }

  inline void update(QString word)
  {
    // Do any filtering you like.
    // Here we just include all items that contain word.
    QStringList filtered = m_list.filter(word, caseSensitivity());
    m_model.setStringList(filtered);
    m_word = word;
    complete();
  }

  inline QString word()
  {
    return m_word;
  }

 private:
  QStringList m_list;
  QStringListModel m_model;
  QString m_word;
};

class LineEditWithCustomUpdateCompleter : public QLineEdit
{
  Q_OBJECT

 public:
  LineEditWithCustomUpdateCompleter(QWidget *parent = 0);
  ~LineEditWithCustomUpdateCompleter();

  void setCompleter(CustomUpdateCompleter *c);
  CustomUpdateCompleter *completer() const;

 protected:
  void keyPressEvent(QKeyEvent *e);

 private slots:
  void insertCompletion(const QString &completion);

 private:
  CustomUpdateCompleter *c;
};

#endif  // TEST_QTUI_PREVIEW_ANOTHER_LINE_EDIT_H_
