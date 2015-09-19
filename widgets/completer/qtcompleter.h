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

class ICompleterItemViewContainer {
 public:
  virtual ~ICompleterItemViewContainer() = default;

  virtual QAbstractItemView* getItemView() const = 0;
  virtual QWidget* getItemViewContainer() = 0;
};

class IdentityItemViewContainer;
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

  void setItemView(QAbstractItemView* item_view);
  QAbstractItemView* itemView();

  int maxVisibleItems() const;
  void setMaxVisibleItems(int maxItems);

 signals:
  void highlighted(const QString &text);
  void highlighted(const QModelIndex &index);
  void activated(const QString &text);
  void activated(const QModelIndex &index);
  void filterPatternChanged(const QString& filter_pattern);

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

  friend class IdentityItemViewContainer;
};

#endif  // QTCOMPLETER_H_
