#include "src/utils/messagebox/messagebox.h"

#include <QDebug>



MessageBoxUtils::MessageBoxUtils() :
    IMessageBoxUtils()
{
    qDebug() << "Create MessageBoxUtils";
}

MessageBoxUtils::~MessageBoxUtils()
{
    qDebug() << "Destroy MessageBoxUtils";
}

QMessageBox::StandardButton MessageBoxUtils::warning(
    QWidget*                     parent,
    const QString&               title,
    const QString&               text,
    QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton  defaultButton
)
{
    return QMessageBox::warning(parent, title, text, buttons, defaultButton);
}

QMessageBox::StandardButton MessageBoxUtils::question(
    QWidget*                     parent,
    const QString&               title,
    const QString&               text,
    QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton  defaultButton
)
{
    return QMessageBox::question(parent, title, text, buttons, defaultButton);
}
