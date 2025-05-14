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
    IStockTableItemWidgetFactory*  stockTableItemWidgetFactory,
    IUserStorage*                  userStorage,
    IInstrumentsStorage*           instrumentsStorage,
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
        stockTableItemWidgetFactory,
        userStorage,
        instrumentsStorage,
        settingsEditor,
        parent
    );
}
