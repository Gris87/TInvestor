#pragma once



#include "src/dialogs/settingsdialog/isettingsdialogfactory.h"



class SettingsDialogFactory : public ISettingsDialogFactory
{
public:
    SettingsDialogFactory();
    ~SettingsDialogFactory() override;

    SettingsDialogFactory(const SettingsDialogFactory& another)            = delete;
    SettingsDialogFactory& operator=(const SettingsDialogFactory& another) = delete;

    std::shared_ptr<ISettingsDialog> newInstance(IConfig* config, QWidget* parent) const override;
};
