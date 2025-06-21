#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidgetfactory.h"



class DecisionMakerWidgetFactory : public IDecisionMakerWidgetFactory
{
public:
    DecisionMakerWidgetFactory();
    ~DecisionMakerWidgetFactory() override;

    DecisionMakerWidgetFactory(const DecisionMakerWidgetFactory& another)            = delete;
    DecisionMakerWidgetFactory& operator=(const DecisionMakerWidgetFactory& another) = delete;

    IDecisionMakerWidget* newInstance(
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
    ) const override;
};
