#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidget_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidget_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidget_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory_mock.h"
#include "src/widgets/portfoliotablewidget/iportfoliotablewidget_mock.h"
#include "src/widgets/portfoliotablewidget/iportfoliotablewidgetfactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_DecisionMakerWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        InSequence seq;

        operationsTableWidgetFactoryMock = new StrictMock<OperationsTableWidgetFactoryMock>();
        accountChartWidgetFactoryMock    = new StrictMock<AccountChartWidgetFactoryMock>();
        logsTableWidgetFactoryMock       = new StrictMock<LogsTableWidgetFactoryMock>();
        portfolioTableWidgetFactoryMock  = new StrictMock<PortfolioTableWidgetFactoryMock>();
        operationsTableWidgetMock        = new StrictMock<OperationsTableWidgetMock>();
        accountChartWidgetMock           = new StrictMock<AccountChartWidgetMock>();
        logsTableWidgetMock              = new StrictMock<LogsTableWidgetMock>();
        portfolioTableWidgetMock         = new StrictMock<PortfolioTableWidgetMock>();
        settingsEditorMock               = new StrictMock<SettingsEditorMock>();

        EXPECT_CALL(*operationsTableWidgetFactoryMock, newInstance(settingsEditorMock, NotNull()))
            .WillOnce(Return(operationsTableWidgetMock));
        EXPECT_CALL(*accountChartWidgetFactoryMock, newInstance(NotNull())).WillOnce(Return(accountChartWidgetMock));
        EXPECT_CALL(*logsTableWidgetFactoryMock, newInstance(settingsEditorMock, NotNull()))
            .WillOnce(Return(logsTableWidgetMock));
        EXPECT_CALL(*portfolioTableWidgetFactoryMock, newInstance(settingsEditorMock, NotNull()))
            .WillOnce(Return(portfolioTableWidgetMock));

        decisionMakerWidget = new DecisionMakerWidget(
            operationsTableWidgetFactoryMock,
            accountChartWidgetFactoryMock,
            logsTableWidgetFactoryMock,
            portfolioTableWidgetFactoryMock,
            settingsEditorMock
        );
    }

    void TearDown()
    {
        delete decisionMakerWidget;
        delete operationsTableWidgetFactoryMock;
        delete accountChartWidgetFactoryMock;
        delete logsTableWidgetFactoryMock;
        delete portfolioTableWidgetFactoryMock;
        // It will be deleted by `delete ui;`
        /*
        delete stocksControlsWidgetMock;
        delete stocksTableWidgetMock;
        delete simulatorDecisionMakerWidgetMock;
        delete autoPilotDecisionMakerWidgetMock;
        */
        delete settingsEditorMock;
    }

    DecisionMakerWidget*                          decisionMakerWidget;
    StrictMock<OperationsTableWidgetFactoryMock>* operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>*    accountChartWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>*       logsTableWidgetFactoryMock;
    StrictMock<PortfolioTableWidgetFactoryMock>*  portfolioTableWidgetFactoryMock;
    StrictMock<OperationsTableWidgetMock>*        operationsTableWidgetMock;
    StrictMock<AccountChartWidgetMock>*           accountChartWidgetMock;
    StrictMock<LogsTableWidgetMock>*              logsTableWidgetMock;
    StrictMock<PortfolioTableWidgetMock>*         portfolioTableWidgetMock;
    StrictMock<SettingsEditorMock>*               settingsEditorMock;
};



TEST_F(Test_DecisionMakerWidget, Test_constructor_and_destructor)
{
}
