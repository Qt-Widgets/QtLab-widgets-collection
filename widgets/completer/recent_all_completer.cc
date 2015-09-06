// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QDebug>

#include "recent_all_completer.h"

class RecentAllModel : public QAbstractItemModel {
 public:
  enum TopLevelRow {
    RECENT_MODEL_ROW = 0,
    ALL_MODEL_ROW,
    MAX_ROW_COUNT
  };

  static const QString kRecentModelRootDisplayName;
  static const QString kAllModelRootDisplayName;

  RecentAllModel(const QAbstractItemModel& recent_model,
                 const QAbstractItemModel& all_model)
      : recent_model_(recent_model)
      , all_model_(all_model) { }
  virtual ~RecentAllModel() = default;

  QVariant data(const QModelIndex &index, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

 private:
  RecentAllModel(const RecentAllModel& other) = delete;
  RecentAllModel& operator=(const RecentAllModel& other) = delete;

  QAbstractItemModel* indexOwnerModel(const QModelIndex& index) const;
  bool isTopLevelIndex(const QModelIndex& index) const;
  bool isRecentModelParent(const QModelIndex& index) const;
  bool isAllModelParent(const QModelIndex& index) const;
  bool isRecentModelIndex(const QModelIndex& index) const;
  bool isAllModelIndex(const QModelIndex& index) const;

  QModelIndex recentModelRootIndex() const;
  QModelIndex allModelRootIndex() const;

  const QAbstractItemModel& recent_model_;
  const QAbstractItemModel& all_model_;
};

const QString
RecentAllModel::kRecentModelRootDisplayName { QStringLiteral("RecentModel") };
const QString
RecentAllModel::kAllModelRootDisplayName { QStringLiteral("AllModel") };

QAbstractItemModel*
RecentAllModel::indexOwnerModel(const QModelIndex& index) const {
  return static_cast<QAbstractItemModel*>(index.internalPointer());
}

bool RecentAllModel::isTopLevelIndex(const QModelIndex& index) const {
  return indexOwnerModel(index) == nullptr;
}

bool RecentAllModel::isRecentModelParent(const QModelIndex& index) const {
  return isTopLevelIndex(index) &&
      (index.row() == RECENT_MODEL_ROW);
}

bool RecentAllModel::isAllModelParent(const QModelIndex& index) const {
  return isTopLevelIndex(index) &&
      (index.row() == ALL_MODEL_ROW);
}

bool RecentAllModel::isRecentModelIndex(const QModelIndex& index) const {
  return indexOwnerModel(index) == &recent_model_;
}

bool RecentAllModel::isAllModelIndex(const QModelIndex& index) const {
  return indexOwnerModel(index) == &all_model_;
}

QVariant RecentAllModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  int row = index.row();
  int col = index.column();

  if (isTopLevelIndex(index)) {
    if ( (role == Qt::DisplayRole) || (role == Qt::EditRole) ) {
      if (row == RECENT_MODEL_ROW) {
        return kRecentModelRootDisplayName;
      } else if (row == ALL_MODEL_ROW) {
        return kAllModelRootDisplayName;
      }
    }
  }

  if (isRecentModelIndex(index)) {
    QModelIndex actual_idx = recent_model_.index(row, col);
    return recent_model_.data(actual_idx, role);
  }

  if (isAllModelIndex(index)) {
    QModelIndex actual_idx = all_model_.index(row, col);
    return all_model_.data(actual_idx, role);
  }

  return QVariant();
}

Qt::ItemFlags RecentAllModel::flags(const QModelIndex &index) const {
  return  QAbstractItemModel::flags(index);
}

QModelIndex RecentAllModel::recentModelRootIndex() const {
  return createIndex(RECENT_MODEL_ROW, 0);
}

QModelIndex RecentAllModel::allModelRootIndex() const {
  return createIndex(ALL_MODEL_ROW, 0);
}

QModelIndex RecentAllModel::index(int row, int column,
                                  const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  if (!parent.isValid()) {
    // create top level indexes
    if (row == RECENT_MODEL_ROW) {
      return recentModelRootIndex();
    } else if (row == ALL_MODEL_ROW) {
      return allModelRootIndex();
    }
  } else {
    // create child indexes
    if (isRecentModelParent(parent)) {
      auto& m = const_cast<QAbstractItemModel&>(recent_model_);
      return createIndex(row, column, static_cast<void*>(&m));
    } else if (isAllModelParent(parent)) {
      auto& m = const_cast<QAbstractItemModel&>(all_model_);
      return createIndex(row, column, reinterpret_cast<void*>(&m));
    }
  }

  return QModelIndex();
}

QModelIndex RecentAllModel::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  if (isTopLevelIndex(index))
    return QModelIndex();

  if (isRecentModelIndex(index)) {
    return recentModelRootIndex();
  }

  if (isAllModelIndex(index)) {
    return allModelRootIndex();
  }

  return QModelIndex();
}

int RecentAllModel::rowCount(const QModelIndex &parent) const {
  if (!parent.isValid()) {
    return MAX_ROW_COUNT;
  } else if (isRecentModelParent(parent)) {
    return recent_model_.rowCount();
  } else if (isAllModelParent(parent)) {
    return all_model_.rowCount();
  }

  return 0;
}

int RecentAllModel::columnCount(const QModelIndex &) const {
  return 1;
}

RecentAllCompleter::RecentAllCompleter(const QAbstractItemModel& recent_model,
                   const QAbstractItemModel& all_model) {
  setModel(new RecentAllModel(recent_model, all_model));
}

QStringList RecentAllCompleter::splitPath(const QString& path) const {
  static QString empty_path_replacement =
      RecentAllModel::kRecentModelRootDisplayName + QStringLiteral("@@@@@");

  QStringList path_list;

  if (path.isEmpty()) {
    path_list << RecentAllModel::kRecentModelRootDisplayName << path;

    auto self = const_cast<RecentAllCompleter*>(this);
    self->setCompletionPrefix(empty_path_replacement);
  } else if (path != empty_path_replacement) {
    path_list << RecentAllModel::kAllModelRootDisplayName << path;
  }

  return path_list;
}
