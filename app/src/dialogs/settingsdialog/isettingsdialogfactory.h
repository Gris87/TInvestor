#pragma once



#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"
#include "src/config/iconfig.h"
#include "src/dialogs/settingsdialog/isettingsdialog.h"



class ISettingsDialogFactory
{
public:
    ISettingsDialogFactory()          = default;
    virtual ~ISettingsDialogFactory() = default;

    ISettingsDialogFactory(const ISettingsDialogFactory& another)            = delete;
    ISettingsDialogFactory& operator=(const ISettingsDialogFactory& another) = delete;

    virtual std::shared_ptr<ISettingsDialog> newInstance(
        IConfig*                           config,
        IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
        QWidget*                           parent
    ) const = 0;
};
