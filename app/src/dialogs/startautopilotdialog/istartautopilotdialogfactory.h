#pragma once



#include "src/dialogs/startautopilotdialog/istartautopilotdialog.h"

#include "src/utils/settingseditor/isettingseditor.h"



class IStartAutoPilotDialogFactory
{
public:
    IStartAutoPilotDialogFactory()
    {
    }
    virtual ~IStartAutoPilotDialogFactory() = default;

    IStartAutoPilotDialogFactory(const IStartAutoPilotDialogFactory& another)            = delete;
    IStartAutoPilotDialogFactory& operator=(const IStartAutoPilotDialogFactory& another) = delete;

    virtual std::shared_ptr<IStartAutoPilotDialog> newInstance(ISettingsEditor* settingsEditor, QWidget* parent) = 0;
};
