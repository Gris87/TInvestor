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
    IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
    IAccountChartWidgetFactory*        accountChartWidgetFactory,
    ILogsTableWidgetFactory*           logsTableWidgetFactory,
    IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory,
    IOperationsTableRecordFactory*     operationsTableRecordFactory,
    IPortfolioTreeRecordFactory*       portfolioTreeRecordFactory,
    IInstrumentWidgetFactory*          instrumentWidgetFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    IFileDialogFactory*                fileDialogFactory,
    ISettingsEditor*                   settingsEditor,
    QWidget*                           parent
) const
{
    return new DecisionMakerWidget(
        operationsTableWidgetFactory,
        accountChartWidgetFactory,
        logsTableWidgetFactory,
        portfolioTreeWidgetFactory,
        operationsTableRecordFactory,
        portfolioTreeRecordFactory,
        instrumentWidgetFactory,
        instrumentTableItemWidgetFactory,
        userStorage,
        instrumentsStorage,
        fileDialogFactory,
        settingsEditor,
        parent
    );
}
