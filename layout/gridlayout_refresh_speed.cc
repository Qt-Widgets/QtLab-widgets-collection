#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QDebug>
#include <QPushButton>
#include <QTimer>

#include <unistd.h>
#include <functional>

static int curr_layout = 1;

using LayoutFunc = std::function<void()>;

void clear(QLayout* layout) {
  qDebug() << "clear sub layout" << layout;
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

class DelayedAction : public QObject {
  Q_OBJECT
 public:
  DelayedAction() { }
  virtual ~DelayedAction() { }

  void trigger(QGridLayout* layout, QList<QLayoutItem*> old_items) {
    layout_ = layout;
    old_items_ = old_items;
    QTimer::singleShot(0, this, SLOT(run()));
  }

 public slots:
  void run() {
    while (!old_items_.isEmpty()) {
      auto item = old_items_.takeFirst();

      qDebug() << "remove old item " << item;

      layout_->removeItem(item);

      if (item->widget()) {
        item->widget()->deleteLater();
      }
      delete item;
    }
  }

 public:
  QGridLayout* layout_;
  QList<QLayoutItem*> old_items_;
};

#include "gridlayout_refresh_speed.moc"

static DelayedAction delayedAction;

void layout_driver(QGridLayout* layout, LayoutFunc layout_func) {
  QList<QLayoutItem*> old_items;
  for (int i = 0; i < layout->count(); ++i) {
    old_items.append(layout->itemAt(i));
  }

  layout_func();

  for (int index = 0; index < layout->count(); ++index) {
    int row, col, rowspan, colspan;

    layout->getItemPosition(index, &row, &col, &rowspan, &colspan);
    qDebug() << "row = " << row << ", col = " << col
             << ", rowspan = " << rowspan << ", colspan = " << colspan;
  }

  delayedAction.trigger(layout, old_items);

#if 0
  while (!old_items.isEmpty()) {
    auto item = old_items.takeFirst();

    qDebug() << "remove old item " << item;

    layout->removeItem(item);

    if (item->widget()) {
      item->widget()->deleteLater();
    }
    delete item;
  }
#endif
}

void layout1(QGridLayout* layout) {
  qDebug() << "===>layout1..............";

  layout_driver(layout, [layout]() {
      // 5 x 5
      for (int row = 0; row < 5; ++row) {
        for (int column = 0; column < 5; ++column) {
          layout->addWidget(new QLabel(QString("layout1: %1 x %2").arg(row).arg(column)),
                            row, column);
        }
      }
    });
  curr_layout = 1;
}

void layout2(QGridLayout* layout) {
  qDebug() << "===>layout2................";

  layout_driver(layout, [layout]() {
      // 6 x 6
      for (int row = 0; row < 6; ++row) {
        for (int column = 0; column < 6; ++column) {
          layout->addWidget(new QLabel(QString("layout2: %1 x %2").arg(row).arg(column)),
                            row, column);
        }
      }
    });

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
