#include "src/dialogs/authdialog/authdialogfactory.h"

#include <QDebug>

#include "src/dialogs/authdialog/authdialog.h"



AuthDialogFactory::AuthDialogFactory() :
    IAuthDialogFactory()
{
    qDebug() << "Create AuthDialogFactory";
}

AuthDialogFactory::~AuthDialogFactory()
{
    qDebug() << "Destroy AuthDialogFactory";
}

IAuthDialog* AuthDialogFactory::newInstance(QWidget* parent)
{
    return new AuthDialog(parent);
}
