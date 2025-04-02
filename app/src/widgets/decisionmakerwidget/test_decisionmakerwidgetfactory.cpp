#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory_mock.h"
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
    StrictMock<OperationsTableWidgetFactoryMock> operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>    accountChartWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>       logsTableWidgetFactoryMock;
    StrictMock<PortfolioTableWidgetFactoryMock>  portfolioTableWidgetFactoryMock;
    StrictMock<SettingsEditorMock>               settingsEditorMock;

    DecisionMakerWidgetFactory factory;

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
