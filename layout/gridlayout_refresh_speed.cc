#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QDebug>
#include <QPushButton>

static int curr_layout = 1;

void clear(QLayout* layout) {
  qDebug() << "clear.........";
  QLayoutItem *item;
  while ((item = layout->takeAt(0)) != 0) {
    if (item->layout()) {
      clear(item->layout());
      delete item->layout();
    }
    if (item->widget()) {
      item->widget()->deleteLater();
    }
    delete item;
  }
  layout->invalidate();
}

void layout1(QGridLayout* layout) {
  qDebug() << "layout1..............";
  clear(layout);
  // 5 x 5
  for (int row = 0; row < 5; ++row) {
    for (int column = 0; column < 5; ++column) {
      layout->addWidget(new QLabel(QString("layout1: %1 x %2").arg(row).arg(column)),
                        row, column);
    }
  }
  curr_layout = 1;
}

void layout2(QGridLayout* layout) {
  qDebug() << "layout2................";
  clear(layout);
  // 6 x 6
  for (int row = 0; row < 6; ++row) {
    for (int column = 0; column < 6; ++column) {
      layout->addWidget(new QLabel(QString("layout2: %1 x %2").arg(row).arg(column)),
                        row, column);
    }
  }

  curr_layout = 2;
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QDialog dialog;

  QVBoxLayout* parent_layout = new QVBoxLayout(&dialog);
  parent_layout->setSizeConstraint(QLayout::SetFixedSize);

  QPushButton* button = new QPushButton("re-layout");
  parent_layout->addWidget(button);


  QGridLayout* layout = new QGridLayout();
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout1(layout);

  parent_layout->addLayout(layout);

  QPushButton* clear_btn = new QPushButton("clear");
  parent_layout->addWidget(clear_btn);

  QObject::connect(button, &QPushButton::clicked,
                   [layout, &dialog]() {
                     qDebug() << "curr_layout = " << curr_layout;
                     if (curr_layout == 1) {
                       layout2(layout);
                     } else {
                       layout1(layout);
                     }
                     dialog.update();
                   });
  QObject::connect(clear_btn, &QPushButton::clicked,
                   [layout]{
                     clear(layout);
                   });

  dialog.show();

  return app.exec();
}
