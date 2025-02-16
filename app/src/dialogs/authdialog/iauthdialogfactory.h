#pragma once



#include "src/dialogs/authdialog/iauthdialog.h"

#include "src/storage/user/iuserstorage.h"



class IAuthDialogFactory
{
public:
    IAuthDialogFactory()
    {
    }
    virtual ~IAuthDialogFactory() = default;

    IAuthDialogFactory(const IAuthDialogFactory& another)            = delete;
    IAuthDialogFactory& operator=(const IAuthDialogFactory& another) = delete;

    virtual IAuthDialog* newInstance(IUserStorage* userStorage, QWidget* parent) = 0;
};
