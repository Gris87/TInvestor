#pragma once



#include "src/dialogs/startautopilotdialog/istartautopilotdialog.h"

#include "src/storage/user/iuserstorage.h"
#include "src/utils/messagebox/imessagebox.h"
#include "src/utils/settingseditor/isettingseditor.h"



class IStartAutoPilotDialogFactory
{
public:
    IStartAutoPilotDialogFactory()          = default;
    virtual ~IStartAutoPilotDialogFactory() = default;

    IStartAutoPilotDialogFactory(const IStartAutoPilotDialogFactory& another)            = delete;
    IStartAutoPilotDialogFactory& operator=(const IStartAutoPilotDialogFactory& another) = delete;

    virtual std::shared_ptr<IStartAutoPilotDialog> newInstance(
        IUserStorage* userStorage, IMessageBoxUtils* messageBoxUtils, ISettingsEditor* settingsEditor, QWidget* parent
    ) const = 0;
};
