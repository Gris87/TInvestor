#pragma once



#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget.h"
#include "src/config/decisions/idecisionmakerconfig.h"
#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory.h"



class IDecisionMakerConfigWidgetFactory
{
public:
    IDecisionMakerConfigWidgetFactory() {}
    virtual ~IDecisionMakerConfigWidgetFactory() = default;

    IDecisionMakerConfigWidgetFactory(const IDecisionMakerConfigWidgetFactory &another) = delete;
    IDecisionMakerConfigWidgetFactory& operator=(const IDecisionMakerConfigWidgetFactory &another) = delete;

    virtual IDecisionMakerConfigWidget* newInstance(
        IDecisionMakerConfig *config,
        IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
        QWidget *parent
    ) = 0;
};
