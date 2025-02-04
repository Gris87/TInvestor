#pragma once



#include "src/config/iconfig.h"
#include "src/config/settingsdialog/isettingsdialog.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"



class ISettingsDialogFactory
{
public:
    ISettingsDialogFactory() {}
    virtual ~ISettingsDialogFactory() = default;

    ISettingsDialogFactory(const ISettingsDialogFactory &another) = delete;
    ISettingsDialogFactory& operator=(const ISettingsDialogFactory &another) = delete;

    virtual ISettingsDialog* newInstance(
        IConfig *config,
        IDecisionMakerConfigWidgetFactory *decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
        QWidget *parent
    ) = 0;
};
