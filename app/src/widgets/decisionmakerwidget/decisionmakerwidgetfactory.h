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
        ILogsTableWidgetFactory*       logsTableWidgetFactory,
        IPortfolioTableWidgetFactory*  portfolioTableWidgetFactory,
        IOperationsTableRecordFactory* operationsTableRecordFactory,
        IStockTableItemWidgetFactory*  stockTableItemWidgetFactory,
        IUserStorage*                  userStorage,
        IInstrumentsStorage*           instrumentsStorage,
        ISettingsEditor*               settingsEditor,
        QWidget*                       parent
    ) const override;
};
