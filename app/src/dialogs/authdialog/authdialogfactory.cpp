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

std::shared_ptr<IAuthDialog>
AuthDialogFactory::newInstance(IUserStorage* userStorage, IMessageBoxUtils* messageBoxUtils, QWidget* parent)
{
    return std::shared_ptr<IAuthDialog>(new AuthDialog(userStorage, messageBoxUtils, parent));
}
