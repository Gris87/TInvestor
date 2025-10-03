#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

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
#include "src/config/decisions/sell/selldecision5config/selldecision5configwidget/iselldecision5configwidgetfactory_mock.h"
#include "src/config/iconfig_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
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



using ::testing::_;
using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_DecisionMakerWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        operationsTableWidgetFactoryMock     = new StrictMock<OperationsTableWidgetFactoryMock>();
        accountChartWidgetFactoryMock        = new StrictMock<AccountChartWidgetFactoryMock>();
        logsFilterWidgetFactoryMock          = new StrictMock<LogsFilterWidgetFactoryMock>();
        logsTableWidgetFactoryMock           = new StrictMock<LogsTableWidgetFactoryMock>();
        decisionMakerConfigWidgetFactoryMock = new StrictMock<DecisionMakerConfigWidgetFactoryMock>();
        buyDecision1ConfigWidgetFactoryMock  = new StrictMock<BuyDecision1ConfigWidgetFactoryMock>();
        buyDecision2ConfigWidgetFactoryMock  = new StrictMock<BuyDecision2ConfigWidgetFactoryMock>();
        buyDecision3ConfigWidgetFactoryMock  = new StrictMock<BuyDecision3ConfigWidgetFactoryMock>();
        buyDecision4ConfigWidgetFactoryMock  = new StrictMock<BuyDecision4ConfigWidgetFactoryMock>();
        buyDecision5ConfigWidgetFactoryMock  = new StrictMock<BuyDecision5ConfigWidgetFactoryMock>();
        buyDecision6ConfigWidgetFactoryMock  = new StrictMock<BuyDecision6ConfigWidgetFactoryMock>();
        buyDecision7ConfigWidgetFactoryMock  = new StrictMock<BuyDecision7ConfigWidgetFactoryMock>();
        buyDecision8ConfigWidgetFactoryMock  = new StrictMock<BuyDecision8ConfigWidgetFactoryMock>();
        sellDecision1ConfigWidgetFactoryMock = new StrictMock<SellDecision1ConfigWidgetFactoryMock>();
        sellDecision2ConfigWidgetFactoryMock = new StrictMock<SellDecision2ConfigWidgetFactoryMock>();
        sellDecision3ConfigWidgetFactoryMock = new StrictMock<SellDecision3ConfigWidgetFactoryMock>();
        sellDecision4ConfigWidgetFactoryMock = new StrictMock<SellDecision4ConfigWidgetFactoryMock>();
        sellDecision5ConfigWidgetFactoryMock = new StrictMock<SellDecision5ConfigWidgetFactoryMock>();
        portfolioTreeWidgetFactoryMock       = new StrictMock<PortfolioTreeWidgetFactoryMock>();
        operationsTableWidgetMock            = new StrictMock<OperationsTableWidgetMock>();
        accountChartWidgetMock               = new StrictMock<AccountChartWidgetMock>();
        logsFilterWidgetMock                 = new StrictMock<LogsFilterWidgetMock>();
        logsTableWidgetMock                  = new StrictMock<LogsTableWidgetMock>();
        bestConfigWidgetMock                 = new StrictMock<DecisionMakerConfigWidgetMock>();
        portfolioTreeWidgetMock              = new StrictMock<PortfolioTreeWidgetMock>();
        operationsTableModelFactoryMock      = new StrictMock<OperationsTableModelFactoryMock>();
        logsTableModelFactoryMock            = new StrictMock<LogsTableModelFactoryMock>();
        portfolioTreeModelFactoryMock        = new StrictMock<PortfolioTreeModelFactoryMock>();
        fileDialogFactoryMock                = new StrictMock<FileDialogFactoryMock>();
        configMock                           = new StrictMock<ConfigMock>();
        configForSimulationMock              = new StrictMock<ConfigMock>();
        simulationSimulatorConfigMock        = new StrictMock<DecisionMakerConfigMock>();
        simulatorConfigMock                  = new StrictMock<DecisionMakerConfigMock>();
        autoPilotConfigMock                  = new StrictMock<DecisionMakerConfigMock>();
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();

        LogFilter filter;

        EXPECT_CALL(
            *operationsTableWidgetFactoryMock,
            newInstance(operationsTableModelFactoryMock, fileDialogFactoryMock, configMock, settingsEditorMock, NotNull())
        )
            .WillOnce(Return(operationsTableWidgetMock));
        EXPECT_CALL(*accountChartWidgetFactoryMock, newInstance(fileDialogFactoryMock, settingsEditorMock, NotNull()))
            .WillOnce(Return(accountChartWidgetMock));
        EXPECT_CALL(*logsFilterWidgetFactoryMock, newInstance(NotNull())).WillOnce(Return(logsFilterWidgetMock));
        EXPECT_CALL(
            *logsTableWidgetFactoryMock,
            newInstance(logsTableModelFactoryMock, fileDialogFactoryMock, settingsEditorMock, NotNull())
        )
            .WillOnce(Return(logsTableWidgetMock));
        EXPECT_CALL(*configForSimulationMock, getSimulatorConfig()).WillOnce(Return(simulationSimulatorConfigMock));
        EXPECT_CALL(
            *decisionMakerConfigWidgetFactoryMock,
            newInstance(
                simulationSimulatorConfigMock,
                buyDecision1ConfigWidgetFactoryMock,
                buyDecision2ConfigWidgetFactoryMock,
                buyDecision3ConfigWidgetFactoryMock,
                buyDecision4ConfigWidgetFactoryMock,
                buyDecision5ConfigWidgetFactoryMock,
                buyDecision6ConfigWidgetFactoryMock,
                buyDecision7ConfigWidgetFactoryMock,
                buyDecision8ConfigWidgetFactoryMock,
                sellDecision1ConfigWidgetFactoryMock,
                sellDecision2ConfigWidgetFactoryMock,
                sellDecision3ConfigWidgetFactoryMock,
                sellDecision4ConfigWidgetFactoryMock,
                sellDecision5ConfigWidgetFactoryMock,
                NotNull()
            )
        )
            .WillOnce(Return(bestConfigWidgetMock));
        EXPECT_CALL(
            *portfolioTreeWidgetFactoryMock,
            newInstance(portfolioTreeModelFactoryMock, fileDialogFactoryMock, settingsEditorMock, NotNull())
        )
            .WillOnce(Return(portfolioTreeWidgetMock));

        EXPECT_CALL(*logsFilterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
        EXPECT_CALL(*logsTableWidgetMock, setFilter(filter));
        EXPECT_CALL(*bestConfigWidgetMock, makeReadOnly());

        decisionMakerWidget = new DecisionMakerWidget(
            operationsTableWidgetFactoryMock,
            accountChartWidgetFactoryMock,
            logsFilterWidgetFactoryMock,
            logsTableWidgetFactoryMock,
            decisionMakerConfigWidgetFactoryMock,
            buyDecision1ConfigWidgetFactoryMock,
            buyDecision2ConfigWidgetFactoryMock,
            buyDecision3ConfigWidgetFactoryMock,
            buyDecision4ConfigWidgetFactoryMock,
            buyDecision5ConfigWidgetFactoryMock,
            buyDecision6ConfigWidgetFactoryMock,
            buyDecision7ConfigWidgetFactoryMock,
            buyDecision8ConfigWidgetFactoryMock,
            sellDecision1ConfigWidgetFactoryMock,
            sellDecision2ConfigWidgetFactoryMock,
            sellDecision3ConfigWidgetFactoryMock,
            sellDecision4ConfigWidgetFactoryMock,
            sellDecision5ConfigWidgetFactoryMock,
            portfolioTreeWidgetFactoryMock,
            operationsTableModelFactoryMock,
            logsTableModelFactoryMock,
            portfolioTreeModelFactoryMock,
            fileDialogFactoryMock,
            configMock,
            configForSimulationMock,
            settingsEditorMock
        );
    }

    void TearDown() override
    {
        delete decisionMakerWidget;
        delete operationsTableWidgetFactoryMock;
        delete accountChartWidgetFactoryMock;
        delete logsFilterWidgetFactoryMock;
        delete logsTableWidgetFactoryMock;
        delete decisionMakerConfigWidgetFactoryMock;
        delete buyDecision1ConfigWidgetFactoryMock;
        delete buyDecision2ConfigWidgetFactoryMock;
        delete buyDecision3ConfigWidgetFactoryMock;
        delete buyDecision4ConfigWidgetFactoryMock;
        delete buyDecision5ConfigWidgetFactoryMock;
        delete buyDecision6ConfigWidgetFactoryMock;
        delete buyDecision7ConfigWidgetFactoryMock;
        delete buyDecision8ConfigWidgetFactoryMock;
        delete sellDecision1ConfigWidgetFactoryMock;
        delete sellDecision2ConfigWidgetFactoryMock;
        delete sellDecision3ConfigWidgetFactoryMock;
        delete sellDecision4ConfigWidgetFactoryMock;
        delete sellDecision5ConfigWidgetFactoryMock;
        delete portfolioTreeWidgetFactoryMock;
        // It will be deleted by `delete ui;`
        /*
        delete operationsTableWidgetMock;
        delete accountChartWidgetMock;
        delete logsFilterWidgetMock;
        delete logsTableWidgetMock;
        delete bestConfigWidgetMock;
        delete portfolioTreeWidgetMock;
        */
        delete operationsTableModelFactoryMock;
        delete logsTableModelFactoryMock;
        delete portfolioTreeModelFactoryMock;
        delete fileDialogFactoryMock;
        delete configMock;
        delete configForSimulationMock;
        delete simulationSimulatorConfigMock;
        delete simulatorConfigMock;
        delete autoPilotConfigMock;
        delete settingsEditorMock;
    }

    DecisionMakerWidget*                              decisionMakerWidget;
    StrictMock<OperationsTableWidgetFactoryMock>*     operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>*        accountChartWidgetFactoryMock;
    StrictMock<LogsFilterWidgetFactoryMock>*          logsFilterWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>*           logsTableWidgetFactoryMock;
    StrictMock<DecisionMakerConfigWidgetFactoryMock>* decisionMakerConfigWidgetFactoryMock;
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>*  buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>*  buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>*  buyDecision3ConfigWidgetFactoryMock;
    StrictMock<BuyDecision4ConfigWidgetFactoryMock>*  buyDecision4ConfigWidgetFactoryMock;
    StrictMock<BuyDecision5ConfigWidgetFactoryMock>*  buyDecision5ConfigWidgetFactoryMock;
    StrictMock<BuyDecision6ConfigWidgetFactoryMock>*  buyDecision6ConfigWidgetFactoryMock;
    StrictMock<BuyDecision7ConfigWidgetFactoryMock>*  buyDecision7ConfigWidgetFactoryMock;
    StrictMock<BuyDecision8ConfigWidgetFactoryMock>*  buyDecision8ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock>* sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock>* sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock>* sellDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision4ConfigWidgetFactoryMock>* sellDecision4ConfigWidgetFactoryMock;
    StrictMock<SellDecision5ConfigWidgetFactoryMock>* sellDecision5ConfigWidgetFactoryMock;
    StrictMock<PortfolioTreeWidgetFactoryMock>*       portfolioTreeWidgetFactoryMock;
    StrictMock<OperationsTableWidgetMock>*            operationsTableWidgetMock;
    StrictMock<AccountChartWidgetMock>*               accountChartWidgetMock;
    StrictMock<LogsFilterWidgetMock>*                 logsFilterWidgetMock;
    StrictMock<LogsTableWidgetMock>*                  logsTableWidgetMock;
    StrictMock<DecisionMakerConfigWidgetMock>*        bestConfigWidgetMock;
    StrictMock<PortfolioTreeWidgetMock>*              portfolioTreeWidgetMock;
    StrictMock<OperationsTableModelFactoryMock>*      operationsTableModelFactoryMock;
    StrictMock<LogsTableModelFactoryMock>*            logsTableModelFactoryMock;
    StrictMock<PortfolioTreeModelFactoryMock>*        portfolioTreeModelFactoryMock;
    StrictMock<FileDialogFactoryMock>*                fileDialogFactoryMock;
    StrictMock<ConfigMock>*                           configMock;
    StrictMock<ConfigMock>*                           configForSimulationMock;
    StrictMock<DecisionMakerConfigMock>*              simulationSimulatorConfigMock;
    StrictMock<DecisionMakerConfigMock>*              simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock>*              autoPilotConfigMock;
    StrictMock<SettingsEditorMock>*                   settingsEditorMock;
};



TEST_F(Test_DecisionMakerWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_DecisionMakerWidget, Test_setAccountName)
{
    const InSequence seq;

    EXPECT_CALL(*portfolioTreeWidgetMock, setAccountName(QString("Hello, friend")));

    decisionMakerWidget->setAccountName("Hello, friend");
}

TEST_F(Test_DecisionMakerWidget, Test_showSpinners)
{
    ASSERT_EQ(decisionMakerWidget->ui->operationsWaitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(decisionMakerWidget->ui->portfolioWaitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(
        decisionMakerWidget->ui->operationsWaitingStackedWidget->currentWidget(), decisionMakerWidget->ui->operationsWorkingPage
    );
    ASSERT_EQ(
        decisionMakerWidget->ui->portfolioWaitingStackedWidget->currentWidget(), decisionMakerWidget->ui->portfolioWorkingPage
    );

    decisionMakerWidget->showSpinners();

    ASSERT_EQ(decisionMakerWidget->ui->operationsWaitingSpinnerWidget->isSpinning(), true);
    ASSERT_EQ(decisionMakerWidget->ui->portfolioWaitingSpinnerWidget->isSpinning(), true);
    ASSERT_EQ(
        decisionMakerWidget->ui->operationsWaitingStackedWidget->currentWidget(), decisionMakerWidget->ui->operationsWaitingPage
    );
    ASSERT_EQ(
        decisionMakerWidget->ui->portfolioWaitingStackedWidget->currentWidget(), decisionMakerWidget->ui->portfolioWaitingPage
    );
}

TEST_F(Test_DecisionMakerWidget, Test_bestConfigFound)
{
    const InSequence seq;

    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->count(), 3);
    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->tabText(0), "Operations");
    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->tabText(1), "Chart");
    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->tabText(2), "Logs");

    EXPECT_CALL(*bestConfigWidgetMock, updateUiFromConfig());

    decisionMakerWidget->bestConfigFound();

    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->count(), 4);
    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->tabText(0), "Operations");
    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->tabText(1), "Chart");
    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->tabText(2), "Logs");
    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->tabText(3), "Best config");

    decisionMakerWidget->showSpinners();

    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->count(), 3);
    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->tabText(0), "Operations");
    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->tabText(1), "Chart");
    ASSERT_EQ(decisionMakerWidget->ui->tabWidget->tabText(2), "Logs");
}

TEST_F(Test_DecisionMakerWidget, Test_operationsRead)
{
    const InSequence seq;

    const QList<Operation> operations;

    ASSERT_EQ(decisionMakerWidget->ui->operationsWaitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(
        decisionMakerWidget->ui->operationsWaitingStackedWidget->currentWidget(), decisionMakerWidget->ui->operationsWorkingPage
    );

    decisionMakerWidget->showSpinners();

    ASSERT_EQ(decisionMakerWidget->ui->operationsWaitingSpinnerWidget->isSpinning(), true);
    ASSERT_EQ(
        decisionMakerWidget->ui->operationsWaitingStackedWidget->currentWidget(), decisionMakerWidget->ui->operationsWaitingPage
    );

    EXPECT_CALL(*operationsTableWidgetMock, operationsRead(operations));
    EXPECT_CALL(*accountChartWidgetMock, operationsRead(operations));

    decisionMakerWidget->operationsRead(operations);

    ASSERT_EQ(decisionMakerWidget->ui->operationsWaitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(
        decisionMakerWidget->ui->operationsWaitingStackedWidget->currentWidget(), decisionMakerWidget->ui->operationsWorkingPage
    );
}

TEST_F(Test_DecisionMakerWidget, Test_operationsAdded)
{
    const InSequence seq;

    const QList<Operation> operations;

    EXPECT_CALL(*operationsTableWidgetMock, operationsAdded(operations));
    EXPECT_CALL(*accountChartWidgetMock, operationsAdded(operations));

    decisionMakerWidget->operationsAdded(operations);
}

TEST_F(Test_DecisionMakerWidget, Test_logsRead)
{
    const InSequence seq;

    const QList<LogEntry> entries;

    EXPECT_CALL(*logsTableWidgetMock, logsRead(entries));

    decisionMakerWidget->logsRead(entries);
}

TEST_F(Test_DecisionMakerWidget, Test_logAdded)
{
    const InSequence seq;

    const LogEntry entry;

    EXPECT_CALL(*logsTableWidgetMock, logAdded(entry));

    decisionMakerWidget->logAdded(entry);
}

TEST_F(Test_DecisionMakerWidget, Test_portfolioChanged)
{
    const InSequence seq;

    const Portfolio portfolio;

    ASSERT_EQ(decisionMakerWidget->ui->portfolioWaitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(
        decisionMakerWidget->ui->portfolioWaitingStackedWidget->currentWidget(), decisionMakerWidget->ui->portfolioWorkingPage
    );

    decisionMakerWidget->showSpinners();

    ASSERT_EQ(decisionMakerWidget->ui->portfolioWaitingSpinnerWidget->isSpinning(), true);
    ASSERT_EQ(
        decisionMakerWidget->ui->portfolioWaitingStackedWidget->currentWidget(), decisionMakerWidget->ui->portfolioWaitingPage
    );

    EXPECT_CALL(*portfolioTreeWidgetMock, portfolioChanged(portfolio));

    decisionMakerWidget->portfolioChanged(portfolio);

    ASSERT_EQ(decisionMakerWidget->ui->portfolioWaitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(
        decisionMakerWidget->ui->portfolioWaitingStackedWidget->currentWidget(), decisionMakerWidget->ui->portfolioWorkingPage
    );
}

TEST_F(Test_DecisionMakerWidget, Test_lastPriceChanged)
{
    const InSequence seq;

    EXPECT_CALL(*portfolioTreeWidgetMock, lastPriceChanged(QString("aaaaa"), FloatEq(1.5f)));

    decisionMakerWidget->lastPriceChanged("aaaaa", 1.5f);
}

TEST_F(Test_DecisionMakerWidget, Test_updateLastPrices)
{
    const InSequence seq;

    EXPECT_CALL(*portfolioTreeWidgetMock, updateLastPrices());

    decisionMakerWidget->updateLastPrices();
}

TEST_F(Test_DecisionMakerWidget, Test_logFilterChanged)
{
    const InSequence seq;

    const LogFilter filter;

    EXPECT_CALL(*logsTableWidgetMock, setFilter(filter));

    decisionMakerWidget->logFilterChanged(filter);
}

TEST_F(Test_DecisionMakerWidget, Test_on_yieldButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*accountChartWidgetMock, switchChart(CHART_TYPE_YIELD));

    decisionMakerWidget->ui->yieldButton->click();

    // clang-format off
    ASSERT_EQ(decisionMakerWidget->ui->yieldButton->isChecked(),         true);
    ASSERT_EQ(decisionMakerWidget->ui->monthlyYieldButton->isChecked(),  false);
    ASSERT_EQ(decisionMakerWidget->ui->dailyYieldButton->isChecked(),    false);
    ASSERT_EQ(decisionMakerWidget->ui->remainedMoneyButton->isChecked(), false);
    ASSERT_EQ(decisionMakerWidget->ui->totalMoneyButton->isChecked(),    false);
    ASSERT_EQ(decisionMakerWidget->ui->timeRangeComboBox->isEnabled(),   true);
    // clang-format on
}

TEST_F(Test_DecisionMakerWidget, Test_on_monthlyYieldButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*accountChartWidgetMock, switchChart(CHART_TYPE_MONTHLY_YIELD));

    decisionMakerWidget->ui->monthlyYieldButton->click();

    // clang-format off
    ASSERT_EQ(decisionMakerWidget->ui->yieldButton->isChecked(),         false);
    ASSERT_EQ(decisionMakerWidget->ui->monthlyYieldButton->isChecked(),  true);
    ASSERT_EQ(decisionMakerWidget->ui->dailyYieldButton->isChecked(),    false);
    ASSERT_EQ(decisionMakerWidget->ui->remainedMoneyButton->isChecked(), false);
    ASSERT_EQ(decisionMakerWidget->ui->totalMoneyButton->isChecked(),    false);
    ASSERT_EQ(decisionMakerWidget->ui->timeRangeComboBox->isEnabled(),   false);
    // clang-format on
}

TEST_F(Test_DecisionMakerWidget, Test_on_dailyYieldButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*accountChartWidgetMock, switchChart(CHART_TYPE_DAILY_YIELD));

    decisionMakerWidget->ui->dailyYieldButton->click();

    // clang-format off
    ASSERT_EQ(decisionMakerWidget->ui->yieldButton->isChecked(),         false);
    ASSERT_EQ(decisionMakerWidget->ui->monthlyYieldButton->isChecked(),  false);
    ASSERT_EQ(decisionMakerWidget->ui->dailyYieldButton->isChecked(),    true);
    ASSERT_EQ(decisionMakerWidget->ui->remainedMoneyButton->isChecked(), false);
    ASSERT_EQ(decisionMakerWidget->ui->totalMoneyButton->isChecked(),    false);
    ASSERT_EQ(decisionMakerWidget->ui->timeRangeComboBox->isEnabled(),   false);
    // clang-format on
}

TEST_F(Test_DecisionMakerWidget, Test_on_remainedMoneyButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*accountChartWidgetMock, switchChart(CHART_TYPE_REMAINED_MONEY));

    decisionMakerWidget->ui->remainedMoneyButton->click();

    // clang-format off
    ASSERT_EQ(decisionMakerWidget->ui->yieldButton->isChecked(),         false);
    ASSERT_EQ(decisionMakerWidget->ui->monthlyYieldButton->isChecked(),  false);
    ASSERT_EQ(decisionMakerWidget->ui->dailyYieldButton->isChecked(),    false);
    ASSERT_EQ(decisionMakerWidget->ui->remainedMoneyButton->isChecked(), true);
    ASSERT_EQ(decisionMakerWidget->ui->totalMoneyButton->isChecked(),    false);
    ASSERT_EQ(decisionMakerWidget->ui->timeRangeComboBox->isEnabled(),   true);
    // clang-format on
}

TEST_F(Test_DecisionMakerWidget, Test_on_totalMoneyButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*accountChartWidgetMock, switchChart(CHART_TYPE_TOTAL_MONEY));

    decisionMakerWidget->ui->totalMoneyButton->click();

    // clang-format off
    ASSERT_EQ(decisionMakerWidget->ui->yieldButton->isChecked(),         false);
    ASSERT_EQ(decisionMakerWidget->ui->monthlyYieldButton->isChecked(),  false);
    ASSERT_EQ(decisionMakerWidget->ui->dailyYieldButton->isChecked(),    false);
    ASSERT_EQ(decisionMakerWidget->ui->remainedMoneyButton->isChecked(), false);
    ASSERT_EQ(decisionMakerWidget->ui->totalMoneyButton->isChecked(),    true);
    ASSERT_EQ(decisionMakerWidget->ui->timeRangeComboBox->isEnabled(),   true);
    // clang-format on
}

TEST_F(Test_DecisionMakerWidget, Test_on_copyToSimulatorConfigButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*configMock, setSimulatorConfigCommon(false));
    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(false));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*configForSimulationMock, getSimulatorConfig()).WillOnce(Return(simulationSimulatorConfigMock));
    EXPECT_CALL(*simulatorConfigMock, assign(simulationSimulatorConfigMock));
    EXPECT_CALL(*configMock, save(settingsEditorMock));

    decisionMakerWidget->ui->copyToSimulatorConfigButton->click();
}

TEST_F(Test_DecisionMakerWidget, Test_on_copyToAutoPilotConfigButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*configMock, setSimulatorConfigCommon(false));
    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(false));
    EXPECT_CALL(*configMock, getAutoPilotConfig()).WillOnce(Return(autoPilotConfigMock));
    EXPECT_CALL(*configForSimulationMock, getSimulatorConfig()).WillOnce(Return(simulationSimulatorConfigMock));
    EXPECT_CALL(*autoPilotConfigMock, assign(simulationSimulatorConfigMock));
    EXPECT_CALL(*configMock, save(settingsEditorMock));

    decisionMakerWidget->ui->copyToAutoPilotConfigButton->click();
}

TEST_F(Test_DecisionMakerWidget, Test_on_copyToBothConfigsButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*configMock, setSimulatorConfigCommon(true));
    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(false));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*configForSimulationMock, getSimulatorConfig()).WillOnce(Return(simulationSimulatorConfigMock));
    EXPECT_CALL(*simulatorConfigMock, assign(simulationSimulatorConfigMock));
    EXPECT_CALL(*configMock, getAutoPilotConfig()).WillOnce(Return(autoPilotConfigMock));
    EXPECT_CALL(*configForSimulationMock, getSimulatorConfig()).WillOnce(Return(simulationSimulatorConfigMock));
    EXPECT_CALL(*autoPilotConfigMock, assign(simulationSimulatorConfigMock));
    EXPECT_CALL(*configMock, save(settingsEditorMock));

    decisionMakerWidget->ui->copyToBothConfigsButton->click();
}

TEST_F(Test_DecisionMakerWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock,        setValue(QString("AAAAA/splitter"), _));
    EXPECT_CALL(*operationsTableWidgetMock, saveWindowState(QString("AAAAA/OperationsTableWidget")));
    EXPECT_CALL(*logsTableWidgetMock,       saveWindowState(QString("AAAAA/LogsTableWidget")));
    EXPECT_CALL(*portfolioTreeWidgetMock,   saveWindowState(QString("AAAAA/PortfolioTreeWidget")));
    // clang-format on

    decisionMakerWidget->saveWindowState("AAAAA");
}

TEST_F(Test_DecisionMakerWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock,        value(QString("AAAAA/splitter"), QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
    EXPECT_CALL(*operationsTableWidgetMock, loadWindowState(QString("AAAAA/OperationsTableWidget")));
    EXPECT_CALL(*logsTableWidgetMock,       loadWindowState(QString("AAAAA/LogsTableWidget")));
    EXPECT_CALL(*portfolioTreeWidgetMock,   loadWindowState(QString("AAAAA/PortfolioTreeWidget")));
    // clang-format on

    decisionMakerWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
