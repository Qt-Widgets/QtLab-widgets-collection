// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_RECENT_ALL_COMPLETER_H_
#define SRC_QTUI_RECENT_ALL_COMPLETER_H_

#include <QCompleter>

class RecentAllCompleter : public QCompleter {
 public:
  RecentAllCompleter(const QAbstractItemModel& recent_model,
                     const QAbstractItemModel& all_model);
  virtual ~RecentAllCompleter() = default;

  QStringList splitPath(const QString& path) const override;

 private:
  RecentAllCompleter(const RecentAllCompleter& other) = delete;
  RecentAllCompleter& operator=(const RecentAllCompleter& other) = delete;
};


#endif  // SRC_QTUI_RECENT_ALL_COMPLETER_H_
