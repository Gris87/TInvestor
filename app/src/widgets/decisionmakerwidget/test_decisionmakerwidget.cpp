#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

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



class Test_DecisionMakerWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        operationsTableWidgetFactoryMock = new StrictMock<OperationsTableWidgetFactoryMock>();
        accountChartWidgetFactoryMock    = new StrictMock<AccountChartWidgetFactoryMock>();
        logsTableWidgetFactoryMock       = new StrictMock<LogsTableWidgetFactoryMock>();
        portfolioTableWidgetFactoryMock  = new StrictMock<PortfolioTableWidgetFactoryMock>();
        settingsEditorMock               = new StrictMock<SettingsEditorMock>();

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
        delete settingsEditorMock;
    }

    DecisionMakerWidget*                          decisionMakerWidget;
    StrictMock<OperationsTableWidgetFactoryMock>* operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>*    accountChartWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>*       logsTableWidgetFactoryMock;
    StrictMock<PortfolioTableWidgetFactoryMock>*  portfolioTableWidgetFactoryMock;
    StrictMock<SettingsEditorMock>*               settingsEditorMock;
};



TEST_F(Test_DecisionMakerWidget, Test_constructor_and_destructor)
{
}
