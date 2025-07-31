#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"

#include <QDebug>

#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"



DecisionMakerWidgetFactory::DecisionMakerWidgetFactory() :
    IDecisionMakerWidgetFactory()
{
    qDebug() << "Create DecisionMakerWidgetFactory";
}

DecisionMakerWidgetFactory::~DecisionMakerWidgetFactory()
{
    qDebug() << "Destroy DecisionMakerWidgetFactory";
}

IDecisionMakerWidget*
DecisionMakerWidgetFactory::newInstance(const DecisionMakerWidgetFactoryNewInstanceArgsMore15& args, QWidget* parent) const
{
    return new DecisionMakerWidget(
        args.operationsTableWidgetFactory,
        args.accountChartWidgetFactory,
        args.logsFilterWidgetFactory,
        args.logsTableWidgetFactory,
        args.decisionMakerConfigWidgetFactory,
        args.buyDecision1ConfigWidgetFactory,
        args.buyDecision2ConfigWidgetFactory,
        args.buyDecision3ConfigWidgetFactory,
        args.sellDecision1ConfigWidgetFactory,
        args.sellDecision2ConfigWidgetFactory,
        args.sellDecision3ConfigWidgetFactory,
        args.sellDecision4ConfigWidgetFactory,
        args.portfolioTreeWidgetFactory,
        args.operationsTableModelFactory,
        args.logsTableModelFactory,
        args.portfolioTreeModelFactory,
        args.fileDialogFactory,
        args.config,
        args.configForSimulation,
        args.settingsEditor,
        parent
    );
}
