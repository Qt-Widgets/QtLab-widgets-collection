#include <QtWidgets>
#include <QDebug>
#include <QAction>

#include "testdialog.h"

TestDialog::TestDialog(QWidget *parent)
    : QDialog(parent)
{

  QAction* action = new QAction(QIcon(QStringLiteral(":/images/new.png")),
                           tr("test"), this);
  action->setObjectName(QStringLiteral("testaction"));
  action->setShortcut(tr("Ctrl+y"));
  action->setStatusTip(tr("test action"));
  action->setEnabled(true);
  connect(action, &QAction::triggered,
          [this]() {
            qDebug() << "action triggered";
          });

  setWindowTitle(tr("Test"));

  QToolButton* button = new QToolButton(this);
  button->setAutoRaise(true);
  button->setDefaultAction(action);
}
