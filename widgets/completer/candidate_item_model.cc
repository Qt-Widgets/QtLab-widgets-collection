// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "candidate_item_model.h"
#include "candidate_item.h"

const CandidateItem*
CandidateItemModel::itemOfIndex(const QModelIndex& index) const {
  if (!index.isValid())
    return root_item_;

  return static_cast<const CandidateItem*>(index.internalPointer());
}

QVariant CandidateItemModel::data(
    const QModelIndex &index, int role) const {
  (void)role;
  if (!index.isValid())
    return QVariant();

  if (!root_item_)
    return QVariant();

  auto item = itemOfIndex(index);

  switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
      return item->text();
    case Qt::ToolTipRole:
      return item->description();
    default:
      return QVariant();
  }
}

Qt::ItemFlags CandidateItemModel::flags(
    const QModelIndex &index) const {
  auto flags = QAbstractItemModel::flags(index);
  auto item = itemOfIndex(index);
  if (!item->isSelectable())
    flags &= ~Qt::ItemIsSelectable;

  return flags;
}

QModelIndex CandidateItemModel::index(
    int row, int column, const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  auto item = itemOfIndex(parent);
  const void* iptr = static_cast<const void*>((*item)[row]);
  return createIndex(row, column, const_cast<void*>(iptr));
}

QModelIndex CandidateItemModel::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  auto item = itemOfIndex(index);

  if (item == root_item_) {
    return QModelIndex();
  }

  auto parent_item = item->parent();
  int row = -1;
  if (parent_item != root_item_) {
    row = parent_item->row();
  }

  const void* iptr = static_cast<const void*>(parent_item);
  return createIndex(row, 0, const_cast<void*>(iptr));
}

int CandidateItemModel::rowCount(const QModelIndex &parent) const {
  auto item = itemOfIndex(parent);
  return item->num_children();
}

int CandidateItemModel::columnCount(const QModelIndex &parent) const {
  (void)parent;
  return 1;
}
