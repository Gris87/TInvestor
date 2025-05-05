#pragma once



#include "src/dialogs/authdialog/iauthdialog.h"

#include "src/storage/user/iuserstorage.h"
#include "src/utils/messagebox/imessageboxutils.h"



class IAuthDialogFactory
{
public:
    IAuthDialogFactory()          = default;
    virtual ~IAuthDialogFactory() = default;

    IAuthDialogFactory(const IAuthDialogFactory& another)            = delete;
    IAuthDialogFactory& operator=(const IAuthDialogFactory& another) = delete;

    virtual std::shared_ptr<IAuthDialog>
    newInstance(IUserStorage* userStorage, IMessageBoxUtils* messageBoxUtils, QWidget* parent) const = 0;
};
