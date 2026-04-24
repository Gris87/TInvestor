#pragma once



#include "src/config/iconfig.h"
#include "src/dialogs/settingsdialog/isettingsdialog.h"



class ISettingsDialogFactory
{
public:
    ISettingsDialogFactory()          = default;
    virtual ~ISettingsDialogFactory() = default;

    ISettingsDialogFactory(const ISettingsDialogFactory& another)            = delete;
    ISettingsDialogFactory& operator=(const ISettingsDialogFactory& another) = delete;

    virtual std::shared_ptr<ISettingsDialog> newInstance(IConfig* config, QWidget* parent) const = 0;
};
