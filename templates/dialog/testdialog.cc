#include <QtWidgets>

#include "testdialog.h"

TestDialog::TestDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Test"));
}
