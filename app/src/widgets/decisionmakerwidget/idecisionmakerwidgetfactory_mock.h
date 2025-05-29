#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidgetfactory.h"

#include <gmock/gmock.h>



class DecisionMakerWidgetFactoryMock : public IDecisionMakerWidgetFactory
{
public:
    DecisionMakerWidgetFactoryMock() :
        IDecisionMakerWidgetFactory()
    {
    }
    ~DecisionMakerWidgetFactoryMock() override = default;

    DecisionMakerWidgetFactoryMock(const DecisionMakerWidgetFactoryMock& another)            = delete;
    DecisionMakerWidgetFactoryMock& operator=(const DecisionMakerWidgetFactoryMock& another) = delete;

    MOCK_METHOD(
        IDecisionMakerWidget*,
        newInstance,
        (IOperationsTableWidgetFactory * operationsTableWidgetFactory,
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
         QWidget*                           parent),
        (const, override)
    );
};
