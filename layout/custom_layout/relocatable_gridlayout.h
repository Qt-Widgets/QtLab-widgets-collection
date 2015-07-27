/****************************************************************************
 **
 ** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the QtWidgets module of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Digia.  For licensing terms and
 ** conditions see http://qt.digia.com/licensing.  For further information
 ** use the contact form at http://qt.digia.com/contact-us.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Digia gives you certain additional
 ** rights.  These rights are described in the Digia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef CUSTOMGRIDLAYOUT_H
#define CUSTOMGRIDLAYOUT_H

#include <memory>

#include <QLayout>
#include <QWidget>

#include <limits.h>

class RelocatableGridLayoutPrivate;

class RelocatableGridLayout : public QLayout
{
  Q_OBJECT
  QDOC_PROPERTY(int horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing)
  QDOC_PROPERTY(int verticalSpacing READ verticalSpacing WRITE setVerticalSpacing)

  public:
  explicit RelocatableGridLayout(QWidget *parent);
  RelocatableGridLayout();

  ~RelocatableGridLayout();

  QSize sizeHint() const;
  QSize minimumSize() const;
  QSize maximumSize() const;

  void setHorizontalSpacing(int spacing);
  int horizontalSpacing() const;
  void setVerticalSpacing(int spacing);
  int verticalSpacing() const;
  void setSpacing(int spacing);
  int spacing() const;

  void setRowStretch(int row, int stretch);
  void setColumnStretch(int column, int stretch);
  int rowStretch(int row) const;
  int columnStretch(int column) const;

  void setRowMinimumHeight(int row, int minSize);
  void setColumnMinimumWidth(int column, int minSize);
  int rowMinimumHeight(int row) const;
  int columnMinimumWidth(int column) const;

  int columnCount() const;
  int rowCount() const;

  QRect cellRect(int row, int column) const;

  bool hasHeightForWidth() const;
  int heightForWidth(int) const;
  int minimumHeightForWidth(int) const;

  Qt::Orientations expandingDirections() const;
  void invalidate();

  inline void addWidget(QWidget *w) { QLayout::addWidget(w); }
  void addWidget(QWidget *, int row, int column, Qt::Alignment = 0);
  void addWidget(QWidget *, int row, int column, int rowSpan, int columnSpan, Qt::Alignment = 0);
  void addLayout(QLayout *, int row, int column, Qt::Alignment = 0);
  void addLayout(QLayout *, int row, int column, int rowSpan, int columnSpan, Qt::Alignment = 0);

  void setOriginCorner(Qt::Corner);
  Qt::Corner originCorner() const;

  QLayoutItem *itemAt(int index) const;
  QLayoutItem *itemAtPosition(int row, int column) const;
  QLayoutItem *takeAt(int index);
  int count() const;
  void setGeometry(const QRect&);

  void addItem(QLayoutItem *item, int row, int column, int rowSpan = 1, int columnSpan = 1, Qt::Alignment = 0);

  void setDefaultPositioning(int n, Qt::Orientation orient);
  void getItemPosition(int idx, int *row, int *column, int *rowSpan, int *columnSpan) const;
  // custom method begin
  void setItemPosition(int index, int row, int column, int rowSpan = 0, int columnSpan = 0);
  void setItemPosition(QLayoutItem* item, int row, int column, int rowSpan = 0, int columnSpan = 0);
  void setItemPosition(QWidget* widget, int row, int column, int rowSpan = 0, int columnSpan = 0);
  // custom method end

 protected:
  void addItem(QLayoutItem *);

 private:
  std::unique_ptr<RelocatableGridLayoutPrivate> d_ptr;
  friend class RelocatableGridLayoutPrivate;

 private:
  Q_DISABLE_COPY(RelocatableGridLayout)

};

#endif // CUSTOMGRIDLAYOUT_H
