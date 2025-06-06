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
        IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
        IAccountChartWidgetFactory*        accountChartWidgetFactory,
        ILogsTableWidgetFactory*           logsTableWidgetFactory,
        IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory,
        IOperationsTableRecordFactory*     operationsTableRecordFactory,
        ILogsTableRecordFactory*           logsTableRecordFactory,
        IPortfolioTreeRecordFactory*       portfolioTreeRecordFactory,
        IInstrumentWidgetFactory*          instrumentWidgetFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        IFileDialogFactory*                fileDialogFactory,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent
    ) const override;
};
