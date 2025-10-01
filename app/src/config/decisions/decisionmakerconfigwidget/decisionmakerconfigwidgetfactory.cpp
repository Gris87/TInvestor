#include "src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidget.h"



DecisionMakerConfigWidgetFactory::DecisionMakerConfigWidgetFactory() :
    IDecisionMakerConfigWidgetFactory()
{
    qDebug() << "Create DecisionMakerConfigWidgetFactory";
}

DecisionMakerConfigWidgetFactory::~DecisionMakerConfigWidgetFactory()
{
    qDebug() << "Destroy DecisionMakerConfigWidgetFactory";
}

IDecisionMakerConfigWidget* DecisionMakerConfigWidgetFactory::newInstance(
    IDecisionMakerConfig*              config,
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
    IBuyDecision4ConfigWidgetFactory*  buyDecision4ConfigWidgetFactory,
    IBuyDecision5ConfigWidgetFactory*  buyDecision5ConfigWidgetFactory,
    IBuyDecision6ConfigWidgetFactory*  buyDecision6ConfigWidgetFactory,
    IBuyDecision7ConfigWidgetFactory*  buyDecision7ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
    ISellDecision4ConfigWidgetFactory* sellDecision4ConfigWidgetFactory,
    QWidget*                           parent
) const
{
    return new DecisionMakerConfigWidget(
        config,
        buyDecision1ConfigWidgetFactory,
        buyDecision2ConfigWidgetFactory,
        buyDecision3ConfigWidgetFactory,
        buyDecision4ConfigWidgetFactory,
        buyDecision5ConfigWidgetFactory,
        buyDecision6ConfigWidgetFactory,
        buyDecision7ConfigWidgetFactory,
        sellDecision1ConfigWidgetFactory,
        sellDecision2ConfigWidgetFactory,
        sellDecision3ConfigWidgetFactory,
        sellDecision4ConfigWidgetFactory,
        parent
    );
}
