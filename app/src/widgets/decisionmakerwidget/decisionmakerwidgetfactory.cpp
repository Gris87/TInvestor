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
    ILogsFilterWidgetFactory*      logsFilterWidgetFactory,
    ILogsTableWidgetFactory*       logsTableWidgetFactory,
    IPortfolioTreeWidgetFactory*   portfolioTreeWidgetFactory,
    IOperationsTableModelFactory*  operationsTableModelFactory,
    ILogsTableModelFactory*        logsTableModelFactory,
    IPortfolioTreeModelFactory*    portfolioTreeModelFactory,
    ILogosStorage*                 logosStorage,
    IFileDialogFactory*            fileDialogFactory,
    ISettingsEditor*               settingsEditor,
    QWidget*                       parent
) const
{
    return new DecisionMakerWidget(
        operationsTableWidgetFactory,
        accountChartWidgetFactory,
        logsFilterWidgetFactory,
        logsTableWidgetFactory,
        portfolioTreeWidgetFactory,
        operationsTableModelFactory,
        logsTableModelFactory,
        portfolioTreeModelFactory,
        logosStorage,
        fileDialogFactory,
        settingsEditor,
        parent
    );
}
