#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QDebug>

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QDialog dialog;

  QGridLayout* layout = new QGridLayout(&dialog);
  layout->addWidget(new QLabel("test"), 0, 0);
  layout->addWidget(new QLabel("test"), 1, 1);

  dialog.show();

  QLayoutItem* item = layout->itemAtPosition(0, 1);
  if (item) {
    qDebug() << "item is " << (item->isEmpty() ? "Empty" : "not Empty");
  } else {
    qDebug() << "item is null";
  }

  QLabel* aLabel = new QLabel("test");
  layout->addWidget(aLabel, 2, 2);
  qDebug() << "row = " << layout->rowCount() << ", column = " << layout->columnCount() 
           << ", item count = " << layout->count();
  
  layout->takeAt(layout->indexOf(aLabel));
  qDebug() << "row = " << layout->rowCount() << ", column = " << layout->columnCount()
           << ", item count = " << layout->count();

  item = layout->itemAtPosition(100, 100);
  if (item == nullptr) {
    qDebug() << "(100, 100) has no item";
  }
   
  return app.exec();
}
