#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialog_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::_;
using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_AccountChartWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        fileDialogFactoryMock = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock    = new StrictMock<SettingsEditorMock>();

        appDir = qApp->applicationDirPath();
        QDir(appDir + "/test/dir_for_account_charts").removeRecursively();
        QDir().mkpath(appDir + "/test/dir_for_account_charts");

        accountChartWidget = new AccountChartWidget(fileDialogFactoryMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete accountChartWidget;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;

        QDir(appDir + "/test/dir_for_account_charts").removeRecursively();
    }

    AccountChartWidget*                accountChartWidget;
    StrictMock<FileDialogFactoryMock>* fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*    settingsEditorMock;
    QString                            appDir;
};



TEST_F(Test_AccountChartWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_AccountChartWidget, Test_eventFilter)
{
    QMouseEvent mouseEvent(QEvent::MouseMove, QPointF(0, 0), QPointF(0, 0), Qt::NoButton, Qt::NoButton, Qt::NoModifier);

    ASSERT_EQ(accountChartWidget->eventFilter(accountChartWidget->viewport(), &mouseEvent), false);

    QWheelEvent wheelEvent(
        QPointF(0, 0), QPointF(0, 0), QPoint(0, 0), QPoint(0, 0), Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, false
    );

    ASSERT_EQ(accountChartWidget->eventFilter(accountChartWidget->viewport(), &wheelEvent), true);

    QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);

    ASSERT_EQ(accountChartWidget->eventFilter(accountChartWidget->viewport(), &keyEvent), false);
}

TEST_F(Test_AccountChartWidget, Test_switchChart)
{
    accountChartWidget->switchChart(CHART_TYPE_YIELD);
    accountChartWidget->switchChart(CHART_TYPE_MONTHLY_YIELD);
    accountChartWidget->switchChart(CHART_TYPE_REMAINED_MONEY);
    accountChartWidget->switchChart(CHART_TYPE_TOTAL_MONEY);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip,                     nullptr);
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(0, 0), false);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), true);
    ASSERT_NE(accountChartWidget->tooltip,                     nullptr);
    // clang-format on

    accountChartWidget->switchChart(CHART_TYPE_YIELD);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip,                     nullptr);
    // clang-format on
}

TEST_F(Test_AccountChartWidget, Test_operationsRead_and_operationsAdded)
{
    QList<Operation> operations;

    accountChartWidget->operationsRead(operations);

    Operation operation;
    operation.timestamp                       = 1704056400000;
    operation.totalYieldWithCommissionPercent = 1.0f;
    operations.append(operation);

    accountChartWidget->operationsRead(operations);

    operations[0].timestamp                       = 1704056460000;
    operations[0].totalYieldWithCommissionPercent = 2.0f;

    accountChartWidget->operationsAdded(operations);
}

TEST_F(Test_AccountChartWidget, Test_contextMenuRequested)
{
    const QPoint pos;

    accountChartWidget->contextMenuRequested(pos);
}

TEST_F(Test_AccountChartWidget, Test_actionExportToExcelTriggered)
{
    const InSequence seq;

    QList<Operation> operations;

    Operation operation;
    operation.timestamp                       = 1704056400000;
    operation.totalYieldWithCommissionPercent = 1.0f;
    operations.append(operation);

    accountChartWidget->operationsRead(operations);

    StrictMock<FileDialogMock>* fileDialogMock =
        new StrictMock<FileDialogMock>(); // Will be deleted in actionExportToExcelTriggered

    QFile excelFile(appDir + "/test/dir_for_account_charts/excel.xlsx");

    ASSERT_EQ(excelFile.exists(), false);

    EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/AccountChartWidget/exportToExcelFile"), _))
        .WillOnce(Return(QVariant(appDir + "/test/dir_for_account_charts/excel.xlsx")));
    EXPECT_CALL(
        *fileDialogFactoryMock,
        newInstance(
            accountChartWidget, QString("Export"), appDir + "/test/dir_for_account_charts", QString("Excel file (*.xlsx)")
        )
    )
        .WillOnce(Return(std::shared_ptr<IFileDialog>(fileDialogMock)));
    EXPECT_CALL(*fileDialogMock, setAcceptMode(QFileDialog::AcceptSave));
    EXPECT_CALL(*fileDialogMock, setDefaultSuffix(QString("xlsx")));
    EXPECT_CALL(*fileDialogMock, selectFile(appDir + "/test/dir_for_account_charts/excel.xlsx"));
    EXPECT_CALL(*fileDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*fileDialogMock, selectedFiles())
        .WillOnce(Return(QStringList() << appDir + "/test/dir_for_account_charts/excel.xlsx"));
    EXPECT_CALL(
        *settingsEditorMock,
        setValue(
            QString("MainWindow/AccountChartWidget/exportToExcelFile"),
            QVariant(appDir + "/test/dir_for_account_charts/excel.xlsx")
        )
    );

    accountChartWidget->actionExportToExcelTriggered();

    // clang-format off
    ASSERT_EQ(excelFile.exists(), true);
    ASSERT_GE(excelFile.size(),   15992);
    // clang-format on
}

TEST_F(Test_AccountChartWidget, Test_lineSeriesHovered)
{
    QList<Operation> operations;

    Operation operation;
    operation.timestamp                       = 1704056460000;
    operation.totalYieldWithCommissionPercent = -3.0f;
    operation.remainedMoney.units             = 50;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = 100;
    operation.totalMoney.nano                 = 0;
    operations.append(operation);

    operation.timestamp                       = 1704056430000;
    operation.totalYieldWithCommissionPercent = 2.0f;
    operation.remainedMoney.units             = 2000;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = 200000;
    operation.totalMoney.nano                 = 0;
    operations.append(operation);

    operation.timestamp                       = 1704056400000;
    operation.totalYieldWithCommissionPercent = 1.0f;
    operation.remainedMoney.units             = 5000;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = 50000;
    operation.totalMoney.nano                 = 0;
    operations.append(operation);

    accountChartWidget->operationsRead(operations);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip,                     nullptr);
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(0, 0), false);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), true);
    ASSERT_NE(accountChartWidget->tooltip,                     nullptr);
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(1704056300000, 0), true);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip->text(),             "2024-01-01 00:00:00\n+1.00 %");
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(1704056400000, 0), true);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip->text(),             "2024-01-01 00:00:00\n+1.00 %");
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(1704056410000, 0), true);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip->text(),             "2024-01-01 00:00:00\n+1.00 %");
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(1704056420000, 0), true);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip->text(),             "2024-01-01 00:00:30\n+2.00 %");
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(1704056440000, 0), true);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip->text(),             "2024-01-01 00:00:30\n+2.00 %");
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(1704056450000, 0), true);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip->text(),             "2024-01-01 00:01:00\n-3.00 %");
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(1704056460000, 0), true);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip->text(),             "2024-01-01 00:01:00\n-3.00 %");
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(1704056500000, 0), true);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip->text(),             "2024-01-01 00:01:00\n-3.00 %");
    // clang-format on
}

TEST_F(Test_AccountChartWidget, Test_barSeriesHovered)
{
    QList<Operation> operations;

    Operation operation;
    operation.timestamp                       = 1706821200000;
    operation.totalYieldWithCommissionPercent = -3.0f;
    operation.remainedMoney.units             = 50;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = 100;
    operation.totalMoney.nano                 = 0;
    operations.append(operation);

    operation.timestamp                       = 1704056430000;
    operation.totalYieldWithCommissionPercent = 2.0f;
    operation.remainedMoney.units             = 2000;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = 200000;
    operation.totalMoney.nano                 = 0;
    operations.append(operation);

    operation.timestamp                       = 1704056400000;
    operation.totalYieldWithCommissionPercent = 1.0f;
    operation.remainedMoney.units             = 5000;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = 50000;
    operation.totalMoney.nano                 = 0;
    operations.append(operation);

    accountChartWidget->operationsRead(operations);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip,                     nullptr);
    // clang-format on

    QBarSet barSet("HOOYAK");
    barSet.append(-1);
    barSet.append(1);

    accountChartWidget->barSeriesHovered(false, 0, &barSet);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), true);
    ASSERT_NE(accountChartWidget->tooltip,                     nullptr);
    // clang-format on

    accountChartWidget->barSeriesHovered(true, 0, &barSet);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip->text(),             "Jan 2024\n-1.00 %");
    // clang-format on

    accountChartWidget->barSeriesHovered(true, 1, &barSet);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip->text(),             "Feb 2024\n+1.00 %");
    // clang-format on
}

TEST_F(Test_AccountChartWidget, Test_tooltipHideTimerTicked)
{
    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_EQ(accountChartWidget->tooltip,                     nullptr);
    // clang-format on

    accountChartWidget->lineSeriesHovered(QPointF(0, 0), false);

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), true);
    ASSERT_NE(accountChartWidget->tooltip,                     nullptr);
    // clang-format on

    accountChartWidget->tooltipHideTimerTicked();

    // clang-format off
    ASSERT_EQ(accountChartWidget->tooltipHideTimer.isActive(), false);
    ASSERT_NE(accountChartWidget->tooltip,                     nullptr);
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
