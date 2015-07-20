#include <QApplication>

#include "testdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestDialog *dialog = new TestDialog;
    dialog->show();
    return app.exec();
}
