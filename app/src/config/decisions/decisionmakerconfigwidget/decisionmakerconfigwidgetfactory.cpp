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
    const DecisionMakerConfigWidgetFactoryNewInstanceArgsMore15& args, QWidget* parent
) const
{
    return new DecisionMakerConfigWidget(
        args.config,
        args.buyDecision1ConfigWidgetFactory,
        args.buyDecision2ConfigWidgetFactory,
        args.buyDecision3ConfigWidgetFactory,
        args.buyDecision4ConfigWidgetFactory,
        args.buyDecision5ConfigWidgetFactory,
        args.buyDecision6ConfigWidgetFactory,
        args.buyDecision7ConfigWidgetFactory,
        args.buyDecision8ConfigWidgetFactory,
        args.buyDecision9ConfigWidgetFactory,
        args.sellDecision1ConfigWidgetFactory,
        args.sellDecision2ConfigWidgetFactory,
        args.sellDecision3ConfigWidgetFactory,
        args.sellDecision4ConfigWidgetFactory,
        args.sellDecision5ConfigWidgetFactory,
        parent
    );
}
