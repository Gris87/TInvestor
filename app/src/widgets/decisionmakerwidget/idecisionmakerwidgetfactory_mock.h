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
         IAccountChartWidgetFactory*   accountChartWidgetFactory,
         ILogsTableWidgetFactory*      logsTableWidgetFactory,
         IPortfolioTableWidgetFactory* portfolioTableWidgetFactory,
         ISettingsEditor*              settingsEditor,
         QWidget*                      parent),
        (const, override)
    );
};
