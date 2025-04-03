#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"

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



TEST(Test_DecisionMakerWidgetFactory, Test_constructor_and_destructor)
{
    DecisionMakerWidgetFactory factory;
}

TEST(Test_DecisionMakerWidgetFactory, Test_newInstance)
{
    InSequence seq;

    DecisionMakerWidgetFactory factory;

    StrictMock<OperationsTableWidgetFactoryMock> operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>    accountChartWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>       logsTableWidgetFactoryMock;
    StrictMock<PortfolioTableWidgetFactoryMock>  portfolioTableWidgetFactoryMock;
    StrictMock<SettingsEditorMock>               settingsEditorMock;

    // It will be deleted by `delete ui;`
    StrictMock<OperationsTableWidgetMock>* operationsTableWidgetMock = new StrictMock<OperationsTableWidgetMock>();
    StrictMock<AccountChartWidgetMock>*    accountChartWidgetMock    = new StrictMock<AccountChartWidgetMock>();
    StrictMock<LogsTableWidgetMock>*       logsTableWidgetMock       = new StrictMock<LogsTableWidgetMock>();
    StrictMock<PortfolioTableWidgetMock>*  portfolioTableWidgetMock  = new StrictMock<PortfolioTableWidgetMock>();

    EXPECT_CALL(operationsTableWidgetFactoryMock, newInstance(&settingsEditorMock, NotNull()))
        .WillOnce(Return(operationsTableWidgetMock));
    EXPECT_CALL(accountChartWidgetFactoryMock, newInstance(NotNull())).WillOnce(Return(accountChartWidgetMock));
    EXPECT_CALL(logsTableWidgetFactoryMock, newInstance(&settingsEditorMock, NotNull())).WillOnce(Return(logsTableWidgetMock));
    EXPECT_CALL(portfolioTableWidgetFactoryMock, newInstance(&settingsEditorMock, NotNull()))
        .WillOnce(Return(portfolioTableWidgetMock));

    IDecisionMakerWidget* widget = factory.newInstance(
        &operationsTableWidgetFactoryMock,
        &accountChartWidgetFactoryMock,
        &logsTableWidgetFactoryMock,
        &portfolioTableWidgetFactoryMock,
        &settingsEditorMock,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
