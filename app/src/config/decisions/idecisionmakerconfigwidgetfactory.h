#pragma once



#include "src/config/decisions/idecisionmakerconfigwidget.h"
#include "src/config/decisions/idecisionmakerconfig.h"
#include "src/config/decisions/buy/ibuydecision1configwidgetfactory.h"
#include "src/config/decisions/buy/ibuydecision2configwidgetfactory.h"
#include "src/config/decisions/buy/ibuydecision3configwidgetfactory.h"
#include "src/config/decisions/sell/iselldecision1configwidgetfactory.h"
#include "src/config/decisions/sell/iselldecision2configwidgetfactory.h"
#include "src/config/decisions/sell/iselldecision3configwidgetfactory.h"



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
