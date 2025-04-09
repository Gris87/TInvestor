#pragma once



#include "src/dialogs/startautopilotdialog/istartautopilotdialogfactory.h"



class StartAutoPilotDialogFactory : public IStartAutoPilotDialogFactory
{
public:
    StartAutoPilotDialogFactory();
    ~StartAutoPilotDialogFactory();

    StartAutoPilotDialogFactory(const StartAutoPilotDialogFactory& another)            = delete;
    StartAutoPilotDialogFactory& operator=(const StartAutoPilotDialogFactory& another) = delete;

    std::shared_ptr<IStartAutoPilotDialog> newInstance(
        IUserStorage* userStorage, IMessageBoxUtils* messageBoxUtils, ISettingsEditor* settingsEditor, QWidget* parent
    ) const override;
};
