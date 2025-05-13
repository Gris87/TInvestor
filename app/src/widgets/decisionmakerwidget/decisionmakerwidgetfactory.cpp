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

IDecisionMakerWidget* DecisionMakerWidgetFactory::newInstance(
    IOperationsTableWidgetFactory* operationsTableWidgetFactory,
    IAccountChartWidgetFactory*    accountChartWidgetFactory,
    ILogsTableWidgetFactory*       logsTableWidgetFactory,
    IPortfolioTableWidgetFactory*  portfolioTableWidgetFactory,
    IOperationsTableRecordFactory* operationsTableRecordFactory,
    ISettingsEditor*               settingsEditor,
    QWidget*                       parent
) const
{
    return new DecisionMakerWidget(
        operationsTableWidgetFactory,
        accountChartWidgetFactory,
        logsTableWidgetFactory,
        portfolioTableWidgetFactory,
        operationsTableRecordFactory,
        settingsEditor,
        parent
    );
}
