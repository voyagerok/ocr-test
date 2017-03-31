#include "alerthelper.h"
#include <QMessageBox>

void AlertHelper::showMessageAlertWithText(const QString &messageText) {
    QMessageBox messageBox;
    messageBox.setText(messageText);
    messageBox.exec();
}
