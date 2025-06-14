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
        args.portfolioTreeWidgetFactory,
        args.operationsTableRecordFactory,
        args.logsTableModelFactory,
        args.portfolioTreeRecordFactory,
        args.instrumentWidgetFactory,
        args.instrumentTableItemWidgetFactory,
        args.userStorage,
        args.instrumentsStorage,
        args.logosStorage,
        args.fileDialogFactory,
        args.settingsEditor,
        parent
    );
}
