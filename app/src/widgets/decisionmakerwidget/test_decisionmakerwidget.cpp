#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <gtest/gtest.h>

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

        operationsTableWidgetFactoryMock = new StrictMock<OperationsTableWidgetFactoryMock>();
        accountChartWidgetFactoryMock    = new StrictMock<AccountChartWidgetFactoryMock>();
        logsFilterWidgetFactoryMock      = new StrictMock<LogsFilterWidgetFactoryMock>();
        logsTableWidgetFactoryMock       = new StrictMock<LogsTableWidgetFactoryMock>();
        portfolioTreeWidgetFactoryMock   = new StrictMock<PortfolioTreeWidgetFactoryMock>();
        operationsTableWidgetMock        = new StrictMock<OperationsTableWidgetMock>();
        accountChartWidgetMock           = new StrictMock<AccountChartWidgetMock>();
        logsFilterWidgetMock             = new StrictMock<LogsFilterWidgetMock>();
        logsTableWidgetMock              = new StrictMock<LogsTableWidgetMock>();
        portfolioTreeWidgetMock          = new StrictMock<PortfolioTreeWidgetMock>();
        operationsTableModelFactoryMock  = new StrictMock<OperationsTableModelFactoryMock>();
        logsTableModelFactoryMock        = new StrictMock<LogsTableModelFactoryMock>();
        portfolioTreeModelFactoryMock    = new StrictMock<PortfolioTreeModelFactoryMock>();
        fileDialogFactoryMock            = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock               = new StrictMock<SettingsEditorMock>();

        LogFilter filter;

        EXPECT_CALL(
            *operationsTableWidgetFactoryMock,
            newInstance(operationsTableModelFactoryMock, fileDialogFactoryMock, settingsEditorMock, NotNull())
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
        EXPECT_CALL(
            *portfolioTreeWidgetFactoryMock,
            newInstance(portfolioTreeModelFactoryMock, fileDialogFactoryMock, settingsEditorMock, NotNull())
        )
            .WillOnce(Return(portfolioTreeWidgetMock));

        EXPECT_CALL(*logsFilterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
        EXPECT_CALL(*logsTableWidgetMock, setFilter(filter));

        decisionMakerWidget = new DecisionMakerWidget(
            operationsTableWidgetFactoryMock,
            accountChartWidgetFactoryMock,
            logsFilterWidgetFactoryMock,
            logsTableWidgetFactoryMock,
            portfolioTreeWidgetFactoryMock,
            operationsTableModelFactoryMock,
            logsTableModelFactoryMock,
            portfolioTreeModelFactoryMock,
            fileDialogFactoryMock,
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
        delete portfolioTreeWidgetFactoryMock;
        // It will be deleted by `delete ui;`
        /*
        delete operationsTableWidgetMock;
        delete accountChartWidgetMock;
        delete logsFilterWidgetMock;
        delete logsTableWidgetMock;
        delete portfolioTreeWidgetMock;
        */
        delete operationsTableModelFactoryMock;
        delete logsTableModelFactoryMock;
        delete portfolioTreeModelFactoryMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
    }

    DecisionMakerWidget*                          decisionMakerWidget;
    StrictMock<OperationsTableWidgetFactoryMock>* operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>*    accountChartWidgetFactoryMock;
    StrictMock<LogsFilterWidgetFactoryMock>*      logsFilterWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>*       logsTableWidgetFactoryMock;
    StrictMock<PortfolioTreeWidgetFactoryMock>*   portfolioTreeWidgetFactoryMock;
    StrictMock<OperationsTableWidgetMock>*        operationsTableWidgetMock;
    StrictMock<AccountChartWidgetMock>*           accountChartWidgetMock;
    StrictMock<LogsFilterWidgetMock>*             logsFilterWidgetMock;
    StrictMock<LogsTableWidgetMock>*              logsTableWidgetMock;
    StrictMock<PortfolioTreeWidgetMock>*          portfolioTreeWidgetMock;
    StrictMock<OperationsTableModelFactoryMock>*  operationsTableModelFactoryMock;
    StrictMock<LogsTableModelFactoryMock>*        logsTableModelFactoryMock;
    StrictMock<PortfolioTreeModelFactoryMock>*    portfolioTreeModelFactoryMock;
    StrictMock<FileDialogFactoryMock>*            fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*               settingsEditorMock;
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

TEST_F(Test_DecisionMakerWidget, Test_on_yieldButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*accountChartWidgetMock, switchChart(CHART_TYPE_YIELD));

    decisionMakerWidget->ui->yieldButton->click();

    // clang-format off
    ASSERT_EQ(decisionMakerWidget->ui->yieldButton->isChecked(),         true);
    ASSERT_EQ(decisionMakerWidget->ui->monthlyYieldButton->isChecked(),  false);
    ASSERT_EQ(decisionMakerWidget->ui->remainedMoneyButton->isChecked(), false);
    ASSERT_EQ(decisionMakerWidget->ui->totalMoneyButton->isChecked(),    false);
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
    ASSERT_EQ(decisionMakerWidget->ui->remainedMoneyButton->isChecked(), false);
    ASSERT_EQ(decisionMakerWidget->ui->totalMoneyButton->isChecked(),    false);
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
    ASSERT_EQ(decisionMakerWidget->ui->remainedMoneyButton->isChecked(), true);
    ASSERT_EQ(decisionMakerWidget->ui->totalMoneyButton->isChecked(),    false);
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
    ASSERT_EQ(decisionMakerWidget->ui->remainedMoneyButton->isChecked(), false);
    ASSERT_EQ(decisionMakerWidget->ui->totalMoneyButton->isChecked(),    true);
    // clang-format on
}

TEST_F(Test_DecisionMakerWidget, Test_logFilterChanged)
{
    const InSequence seq;

    const LogFilter filter;

    EXPECT_CALL(*logsTableWidgetMock, setFilter(filter));

    decisionMakerWidget->logFilterChanged(filter);
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
