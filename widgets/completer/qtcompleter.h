// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef QTCOMPLETER_H_
#define QTCOMPLETER_H_

#include <QObject>
#include <QRect>

#include <memory>

class QAbstractItemModel;
class QAbstractItemView;
class QString;
class QModelIndex;

class QtCompleterImpl;

class QtCompleter : public QObject {
  Q_OBJECT;

 public:
  explicit QtCompleter(QObject* parent = nullptr);
  QtCompleter(QAbstractItemModel *model, QObject *parent = 0);
  virtual ~QtCompleter();

  void setWidget(QWidget* widget);
  QWidget* widget() const;

  void setModel(QAbstractItemModel* c);
  QAbstractItemModel* model() const;

  void setPopup(QAbstractItemView* popup);
  QAbstractItemView* popup() const;

  void hidePopup() const;

  int maxVisibleItems() const;
  void setMaxVisibleItems(int maxItems);

  QString filterPattern() const;

 signals:
  void filterPatternChanged(const QString& filter_pattern);
  void clicked(const QModelIndex &index);
  void highlighted(const QModelIndex &index);

 public slots:
  void setFilterPattern(const QString& filter_pattern);
  void complete(const QRect& rect = QRect());

 protected:
  bool eventFilter(QObject *o, QEvent *e);
  bool event(QEvent *);

 private:
  QtCompleter(const QtCompleter& other) = delete;
  QtCompleter& operator=(const QtCompleter& other) = delete;

  std::unique_ptr<QtCompleterImpl> d;
  friend class QtCompleterImpl;
};

#endif  // QTCOMPLETER_H_
