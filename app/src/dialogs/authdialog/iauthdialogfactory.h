#pragma once



#include "src/dialogs/authdialog/iauthdialog.h"

#include "src/storage/user/iuserstorage.h"
#include "src/utils/messagebox/imessagebox.h"



class IAuthDialogFactory
{
public:
    IAuthDialogFactory()
    {
    }
    virtual ~IAuthDialogFactory() = default;

    IAuthDialogFactory(const IAuthDialogFactory& another)            = delete;
    IAuthDialogFactory& operator=(const IAuthDialogFactory& another) = delete;

    virtual IAuthDialog* newInstance(IUserStorage* userStorage, IMessageBoxUtils* messageBoxUtils, QWidget* parent) = 0;
};
