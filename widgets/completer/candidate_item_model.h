// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
#define SRC_QTUI_CANDIDATE_ITEM_QMODEL_ADAPTER_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QDebug>

#include "candidate_item.h"

class CandidateItemModel : public QAbstractItemModel {
 public:
  CandidateItemModel(const CandidateItem& root_item)
      : root_item_(&root_item) { }
  virtual ~CandidateItemModel() = default;

  QVariant data(const QModelIndex &index, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  void reset();

 private:
  Q_DISABLE_COPY(CandidateItemModel)

  const CandidateItem* itemOfIndex(const QModelIndex& index) const;

  const CandidateItem* root_item_ { nullptr };
};


#endif  // SRC_QTUI_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
