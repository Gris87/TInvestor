#pragma once



#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/ibuydecision5configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/ibuydecision6configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/ibuydecision7configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/ibuydecision8configwidgetfactory.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget.h"
#include "src/config/decisions/idecisionmakerconfig.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidgetfactory.h"



struct DecisionMakerConfigWidgetFactoryNewInstanceArgsMore15
{
    explicit DecisionMakerConfigWidgetFactoryNewInstanceArgsMore15(
        IDecisionMakerConfig*              _config,
        IBuyDecision1ConfigWidgetFactory*  _buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  _buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  _buyDecision3ConfigWidgetFactory,
        IBuyDecision4ConfigWidgetFactory*  _buyDecision4ConfigWidgetFactory,
        IBuyDecision5ConfigWidgetFactory*  _buyDecision5ConfigWidgetFactory,
        IBuyDecision6ConfigWidgetFactory*  _buyDecision6ConfigWidgetFactory,
        IBuyDecision7ConfigWidgetFactory*  _buyDecision7ConfigWidgetFactory,
        IBuyDecision8ConfigWidgetFactory*  _buyDecision8ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* _sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* _sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* _sellDecision3ConfigWidgetFactory,
        ISellDecision4ConfigWidgetFactory* _sellDecision4ConfigWidgetFactory
    ) :
        config(_config),
        buyDecision1ConfigWidgetFactory(_buyDecision1ConfigWidgetFactory),
        buyDecision2ConfigWidgetFactory(_buyDecision2ConfigWidgetFactory),
        buyDecision3ConfigWidgetFactory(_buyDecision3ConfigWidgetFactory),
        buyDecision4ConfigWidgetFactory(_buyDecision4ConfigWidgetFactory),
        buyDecision5ConfigWidgetFactory(_buyDecision5ConfigWidgetFactory),
        buyDecision6ConfigWidgetFactory(_buyDecision6ConfigWidgetFactory),
        buyDecision7ConfigWidgetFactory(_buyDecision7ConfigWidgetFactory),
        buyDecision8ConfigWidgetFactory(_buyDecision8ConfigWidgetFactory),
        sellDecision1ConfigWidgetFactory(_sellDecision1ConfigWidgetFactory),
        sellDecision2ConfigWidgetFactory(_sellDecision2ConfigWidgetFactory),
        sellDecision3ConfigWidgetFactory(_sellDecision3ConfigWidgetFactory),
        sellDecision4ConfigWidgetFactory(_sellDecision4ConfigWidgetFactory)
    {
    }

    bool operator==(const DecisionMakerConfigWidgetFactoryNewInstanceArgsMore15& another) const = default;

    IDecisionMakerConfig*              config;
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory;
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory;
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory;
    IBuyDecision4ConfigWidgetFactory*  buyDecision4ConfigWidgetFactory;
    IBuyDecision5ConfigWidgetFactory*  buyDecision5ConfigWidgetFactory;
    IBuyDecision6ConfigWidgetFactory*  buyDecision6ConfigWidgetFactory;
    IBuyDecision7ConfigWidgetFactory*  buyDecision7ConfigWidgetFactory;
    IBuyDecision8ConfigWidgetFactory*  buyDecision8ConfigWidgetFactory;
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory;
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory;
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory;
    ISellDecision4ConfigWidgetFactory* sellDecision4ConfigWidgetFactory;
};



class IDecisionMakerConfigWidgetFactory
{
public:
    IDecisionMakerConfigWidgetFactory()          = default;
    virtual ~IDecisionMakerConfigWidgetFactory() = default;

    IDecisionMakerConfigWidgetFactory(const IDecisionMakerConfigWidgetFactory& another)            = delete;
    IDecisionMakerConfigWidgetFactory& operator=(const IDecisionMakerConfigWidgetFactory& another) = delete;

    virtual IDecisionMakerConfigWidget*
    newInstance(const DecisionMakerConfigWidgetFactoryNewInstanceArgsMore15& args, QWidget* parent) const = 0;
};
