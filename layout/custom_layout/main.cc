#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QLayoutItem>
#include <QDebug>
#include <QPushButton>

#include "./relocatable_gridlayout.h"

bool do_shrink = false;

void init_layout(RelocatableGridLayout* layout) {
  // 5 x 5
  for (int row = 0; row < 5; ++row) {
    for (int column = 0; column < 5; ++column) {
      layout->addWidget(new QLabel(QString("layout1: %1 x %2").arg(row).arg(column)),
                        row, column);
    }
  }
}

void shuffle(RelocatableGridLayout* layout, int rr, int cc) {
  int start_row = std::rand() % rr;
  int start_col = std::rand() % cc;
  int index = 0;

  for (int r = start_row; r < start_row + rr; ++r) {
    for (int c = start_col; c < start_col + cc; ++c) {
      layout->setItemPosition(index, r % rr, c % cc);
      ++index;
    }
  }
}

void shrink_layout(RelocatableGridLayout* layout) {
  shuffle(layout, 5, 5);

  qDebug() << "count = " << layout->count();
  int count = layout->count();
  QList<QWidget*> wid_to_remove;

  for (int rm_index = 5 * 5; rm_index < count; ++rm_index) {
    auto item = layout->itemAt(rm_index);
    wid_to_remove.append(item->widget());
  }

  while (!wid_to_remove.isEmpty()) {
    auto wid = wid_to_remove.takeFirst();

    QLabel* label = qobject_cast<QLabel*>(wid);
    qDebug() << "remove label: " << label->text();

    layout->removeWidget(wid);
    wid->deleteLater();
  }

  layout->invalidate();

  do_shrink = false;
}

void enlarge_layout(RelocatableGridLayout* layout) {
  shuffle(layout, 5, 5);

  for (int row = 0; row < 5; ++row) {
    layout->addWidget(new QLabel(QString("layout1: %1 x %2").arg(row).arg(5)),
                      row, 5);
  }

  for (int col = 0; col < 6; ++col) {
    layout->addWidget(new QLabel(QString("layout1: %1 x %2").arg(5).arg(col)),
                      5, col);
  }

  do_shrink = true;
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
                     if (do_shrink) {
                       shrink_layout(layout);
                     } else {
                       enlarge_layout(layout);
                     }
                   });
  QObject::connect(overlap_btn, &QPushButton::clicked,
                   [layout]() {
                     layout->addWidget(new QLabel(QString("layout1: overlap")),
                                       0, 0);
                   });

  dialog.show();

  return app.exec();
}
