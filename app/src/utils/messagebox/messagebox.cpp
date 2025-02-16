#include "src/utils/messagebox/messagebox.h"

#include <QDebug>



MessageBox::MessageBox() :
    IMessageBox()
{
    qDebug() << "Create MessageBox";
}

MessageBox::~MessageBox()
{
    qDebug() << "Destroy MessageBox";
}

QMessageBox::StandardButton MessageBox::warning(
    QWidget*                     parent,
    const QString&               title,
    const QString&               text,
    QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton  defaultButton
)
{
    return QMessageBox::warning(parent, title, text, buttons, defaultButton);
}
