#pragma once



#include "src/dialogs/settingsdialog/isettingsdialogfactory.h"



class SettingsDialogFactory : public ISettingsDialogFactory
{
public:
    SettingsDialogFactory();
    ~SettingsDialogFactory();

    SettingsDialogFactory(const SettingsDialogFactory& another)            = delete;
    SettingsDialogFactory& operator=(const SettingsDialogFactory& another) = delete;

    std::shared_ptr<ISettingsDialog> newInstance(
        IConfig*                           config,
        IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
        QWidget*                           parent
    ) override;
};
