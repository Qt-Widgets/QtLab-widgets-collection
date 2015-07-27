#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QLayoutItem>
#include <QDebug>
#include <QPushButton>

#include "./relocatable_gridlayout.h"

void init_layout(RelocatableGridLayout* layout) {
  // 5 x 5
  for (int row = 0; row < 5; ++row) {
    for (int column = 0; column < 5; ++column) {
      layout->addWidget(new QLabel(QString("layout1: %1 x %2").arg(row).arg(column)),
                        row, column);
    }
  }
}

void shuffle(RelocatableGridLayout* layout) {
  int rr = layout->rowCount();
  int cc = layout->columnCount();

  int start_row = std::rand() % rr;
  int start_col = std::rand() % cc;
  int index = 0;

  for (int r = start_row; r < start_row + rr; ++r) {
    for (int c = start_col; c < start_col + cc; ++c) {
      layout->setItemPosition(index, r % rr, c % cc);
      ++index;
    }
  }

  layout->invalidate();
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QDialog dialog;

  QVBoxLayout* parent_layout = new QVBoxLayout(&dialog);
  parent_layout->setSizeConstraint(QLayout::SetFixedSize);

  QPushButton* button = new QPushButton("re-layout");
  parent_layout->addWidget(button);


  RelocatableGridLayout* layout = new RelocatableGridLayout();
  layout->setSizeConstraint(QLayout::SetFixedSize);
  init_layout(layout);

  parent_layout->addLayout(layout);

  QPushButton* overlap_btn = new QPushButton("overlap");
  parent_layout->addWidget(overlap_btn);

  QObject::connect(button, &QPushButton::clicked,
                   [layout]() {
                     shuffle(layout);
                   });
  QObject::connect(overlap_btn, &QPushButton::clicked,
                   [layout]() {
                     layout->addWidget(new QLabel(QString("layout1: overlap")),
                                       0, 0);
                   });

  dialog.show();

  return app.exec();
}
