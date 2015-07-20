#include <QApplication>

#include "show_grid_border.h"  // NOLINT

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  GridTest test;
  test.show(); 

  return app.exec();
}
