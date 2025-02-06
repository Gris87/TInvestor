#include "decisionmakerconfigwidgetfactory.h"

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
    IDecisionMakerConfig *config,
    IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
    QWidget *parent
)
{
    return new DecisionMakerConfigWidget(
        config,
        buyDecision1ConfigWidgetFactory,
        buyDecision2ConfigWidgetFactory,
        buyDecision3ConfigWidgetFactory,
        sellDecision1ConfigWidgetFactory,
        sellDecision2ConfigWidgetFactory,
        sellDecision3ConfigWidgetFactory,
        parent
    );
}
