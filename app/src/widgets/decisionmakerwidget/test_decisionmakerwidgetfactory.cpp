#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/ibuydecision5configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/ibuydecision6configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/ibuydecision7configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/ibuydecision8configwidgetfactory_mock.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget_mock.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidgetfactory_mock.h"
#include "src/config/iconfig_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/messagebox/imessageboxutils_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidget_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory_mock.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidget_mock.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidgetfactory_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidget_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidget_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory_mock.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidget_mock.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory_mock.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory_mock.h"
#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodelfactory_mock.h"
#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodelfactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_DecisionMakerWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new DecisionMakerWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    DecisionMakerWidgetFactory* factory;
};



TEST_F(Test_DecisionMakerWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_DecisionMakerWidgetFactory, Test_newInstance)
{
    const InSequence seq;

    StrictMock<OperationsTableWidgetFactoryMock>     operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>        accountChartWidgetFactoryMock;
    StrictMock<LogsFilterWidgetFactoryMock>          logsFilterWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>           logsTableWidgetFactoryMock;
    StrictMock<DecisionMakerConfigWidgetFactoryMock> decisionMakerConfigWidgetFactoryMock;
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>  buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>  buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>  buyDecision3ConfigWidgetFactoryMock;
    StrictMock<BuyDecision4ConfigWidgetFactoryMock>  buyDecision4ConfigWidgetFactoryMock;
    StrictMock<BuyDecision5ConfigWidgetFactoryMock>  buyDecision5ConfigWidgetFactoryMock;
    StrictMock<BuyDecision6ConfigWidgetFactoryMock>  buyDecision6ConfigWidgetFactoryMock;
    StrictMock<BuyDecision7ConfigWidgetFactoryMock>  buyDecision7ConfigWidgetFactoryMock;
    StrictMock<BuyDecision8ConfigWidgetFactoryMock>  buyDecision8ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock> sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock> sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock> sellDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision4ConfigWidgetFactoryMock> sellDecision4ConfigWidgetFactoryMock;
    StrictMock<PortfolioTreeWidgetFactoryMock>       portfolioTreeWidgetFactoryMock;
    StrictMock<OperationsTableModelFactoryMock>      operationsTableModelFactoryMock;
    StrictMock<LogsTableModelFactoryMock>            logsTableModelFactoryMock;
    StrictMock<PortfolioTreeModelFactoryMock>        portfolioTreeModelFactoryMock;
    StrictMock<FileDialogFactoryMock>                fileDialogFactoryMock;
    StrictMock<MessageBoxUtilsMock>                  messageBoxUtilsMock;
    StrictMock<ConfigMock>                           configMock;
    StrictMock<ConfigMock>                           configForSimulationMock;
    StrictMock<DecisionMakerConfigMock>              simulatorConfigMock;
    StrictMock<SettingsEditorMock>                   settingsEditorMock;

    // It will be deleted by `delete ui;`
    StrictMock<OperationsTableWidgetMock>*     operationsTableWidgetMock = new StrictMock<OperationsTableWidgetMock>();
    StrictMock<AccountChartWidgetMock>*        accountChartWidgetMock    = new StrictMock<AccountChartWidgetMock>();
    StrictMock<LogsFilterWidgetMock>*          logsFilterWidgetMock      = new StrictMock<LogsFilterWidgetMock>();
    StrictMock<LogsTableWidgetMock>*           logsTableWidgetMock       = new StrictMock<LogsTableWidgetMock>();
    StrictMock<DecisionMakerConfigWidgetMock>* bestConfigWidgetMock      = new StrictMock<DecisionMakerConfigWidgetMock>();
    StrictMock<PortfolioTreeWidgetMock>*       portfolioTreeWidgetMock   = new StrictMock<PortfolioTreeWidgetMock>();

    LogFilter filter;

    EXPECT_CALL(
        operationsTableWidgetFactoryMock,
        newInstance(&operationsTableModelFactoryMock, &fileDialogFactoryMock, &configMock, &settingsEditorMock, NotNull())
    )
        .WillOnce(Return(operationsTableWidgetMock));
    EXPECT_CALL(accountChartWidgetFactoryMock, newInstance(&fileDialogFactoryMock, &settingsEditorMock, NotNull()))
        .WillOnce(Return(accountChartWidgetMock));
    EXPECT_CALL(logsFilterWidgetFactoryMock, newInstance(NotNull())).WillOnce(Return(logsFilterWidgetMock));
    EXPECT_CALL(
        logsTableWidgetFactoryMock,
        newInstance(&logsTableModelFactoryMock, &fileDialogFactoryMock, &settingsEditorMock, NotNull())
    )
        .WillOnce(Return(logsTableWidgetMock));
    EXPECT_CALL(configForSimulationMock, getSimulatorConfig()).WillOnce(Return(&simulatorConfigMock));
    EXPECT_CALL(
        decisionMakerConfigWidgetFactoryMock,
        newInstance(
            DecisionMakerConfigWidgetFactoryNewInstanceArgsMore15(
                &simulatorConfigMock,
                &buyDecision1ConfigWidgetFactoryMock,
                &buyDecision2ConfigWidgetFactoryMock,
                &buyDecision3ConfigWidgetFactoryMock,
                &buyDecision4ConfigWidgetFactoryMock,
                &buyDecision5ConfigWidgetFactoryMock,
                &buyDecision6ConfigWidgetFactoryMock,
                &buyDecision7ConfigWidgetFactoryMock,
                &buyDecision8ConfigWidgetFactoryMock,
                &sellDecision1ConfigWidgetFactoryMock,
                &sellDecision2ConfigWidgetFactoryMock,
                &sellDecision3ConfigWidgetFactoryMock,
                &sellDecision4ConfigWidgetFactoryMock
            ),
            NotNull()
        )
    )
        .WillOnce(Return(bestConfigWidgetMock));
    EXPECT_CALL(
        portfolioTreeWidgetFactoryMock,
        newInstance(&portfolioTreeModelFactoryMock, &fileDialogFactoryMock, &messageBoxUtilsMock, &settingsEditorMock, NotNull())
    )
        .WillOnce(Return(portfolioTreeWidgetMock));

    EXPECT_CALL(*logsFilterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(*logsTableWidgetMock, setFilter(filter));
    EXPECT_CALL(*bestConfigWidgetMock, makeReadOnly());

    const IDecisionMakerWidget* widget = factory->newInstance(
        DecisionMakerWidgetFactoryNewInstanceArgsMore15(
            &operationsTableWidgetFactoryMock,
            &accountChartWidgetFactoryMock,
            &logsFilterWidgetFactoryMock,
            &logsTableWidgetFactoryMock,
            &decisionMakerConfigWidgetFactoryMock,
            &buyDecision1ConfigWidgetFactoryMock,
            &buyDecision2ConfigWidgetFactoryMock,
            &buyDecision3ConfigWidgetFactoryMock,
            &buyDecision4ConfigWidgetFactoryMock,
            &buyDecision5ConfigWidgetFactoryMock,
            &buyDecision6ConfigWidgetFactoryMock,
            &buyDecision7ConfigWidgetFactoryMock,
            &buyDecision8ConfigWidgetFactoryMock,
            &sellDecision1ConfigWidgetFactoryMock,
            &sellDecision2ConfigWidgetFactoryMock,
            &sellDecision3ConfigWidgetFactoryMock,
            &sellDecision4ConfigWidgetFactoryMock,
            &portfolioTreeWidgetFactoryMock,
            &operationsTableModelFactoryMock,
            &logsTableModelFactoryMock,
            &portfolioTreeModelFactoryMock,
            &fileDialogFactoryMock,
            &messageBoxUtilsMock,
            &configMock,
            &configForSimulationMock,
            &settingsEditorMock
        ),
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
