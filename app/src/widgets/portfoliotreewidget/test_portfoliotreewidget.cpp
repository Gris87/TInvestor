#include "src/widgets/portfoliotreewidget/portfoliotreewidget.h"
#include "ui_portfoliotreewidget.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialog_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodel_mock.h"
#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodelfactory_mock.h"



using ::testing::_;
using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_PortfolioTreeWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        portfolioTreeModelFactoryMock = new StrictMock<PortfolioTreeModelFactoryMock>();
        fileDialogFactoryMock         = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock            = new StrictMock<SettingsEditorMock>();

        portfolioTreeModelMock = new StrictMock<PortfolioTreeModelMock>();

        appDir = qApp->applicationDirPath();
        QDir(appDir + "/test/dir_for_portfolio_tree").removeRecursively();
        QDir().mkpath(appDir + "/test/dir_for_portfolio_tree");

        EXPECT_CALL(*portfolioTreeModelFactoryMock, newInstance(NotNull())).WillOnce(Return(portfolioTreeModelMock));
        EXPECT_CALL(*portfolioTreeModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
        EXPECT_CALL(*portfolioTreeModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(0));

        portfolioTreeWidget = new PortfolioTreeWidget(portfolioTreeModelFactoryMock, fileDialogFactoryMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete portfolioTreeWidget;
        delete portfolioTreeModelFactoryMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
        delete portfolioTreeModelMock;

        QDir(appDir + "/test/dir_for_portfolio_tree").removeRecursively();
    }

    PortfolioTreeWidget*                       portfolioTreeWidget;
    StrictMock<PortfolioTreeModelFactoryMock>* portfolioTreeModelFactoryMock;
    StrictMock<FileDialogFactoryMock>*         fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*            settingsEditorMock;
    StrictMock<PortfolioTreeModelMock>*        portfolioTreeModelMock;
    QString                                    appDir;
};



TEST_F(Test_PortfolioTreeWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioTreeWidget, Test_setAccountName)
{
    portfolioTreeWidget->setAccountName("Hello");

    ASSERT_EQ(portfolioTreeWidget->ui->accountNameLabel->text(), "Hello");

    portfolioTreeWidget->setAccountName("Bro");

    ASSERT_EQ(portfolioTreeWidget->ui->accountNameLabel->text(), "Bro");
}

TEST_F(Test_PortfolioTreeWidget, Test_portfolioChanged)
{
    const InSequence seq;

    const Portfolio portfolio;

    EXPECT_CALL(*portfolioTreeModelMock, portfolioChanged(portfolio));
    EXPECT_CALL(*portfolioTreeModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
    EXPECT_CALL(*portfolioTreeModelMock, totalCost()).WillOnce(Return(100000.0));
    EXPECT_CALL(*portfolioTreeModelMock, totalDailyCost()).WillOnce(Return(105000.0));
    EXPECT_CALL(*portfolioTreeModelMock, totalYield()).WillOnce(Return(10000.0));
    EXPECT_CALL(*portfolioTreeModelMock, totalDailyYield()).WillOnce(Return(5000.0));

    portfolioTreeWidget->portfolioChanged(portfolio);

    // clang-format off
    ASSERT_EQ(portfolioTreeWidget->ui->costLabel->text(),        "100000.00 \u20BD");
    ASSERT_EQ(portfolioTreeWidget->ui->allTimeLabel->text(),     "+10000.00 \u20BD(+10.00%)");
    ASSERT_EQ(portfolioTreeWidget->ui->forTodayLabel->text(),    "+5000.00 \u20BD(+4.76%)");
    ASSERT_EQ(portfolioTreeWidget->ui->allTimeLabel->toolTip(),  "From: 100000.00 \u20BD");
    ASSERT_EQ(portfolioTreeWidget->ui->forTodayLabel->toolTip(), "From: 105000.00 \u20BD");
    // clang-format on
}

TEST_F(Test_PortfolioTreeWidget, Test_lastPriceChanged)
{
    const InSequence seq;

    EXPECT_CALL(*portfolioTreeModelMock, lastPriceChanged(QString("aaaaa"), FloatEq(1.5f)));

    portfolioTreeWidget->lastPriceChanged("aaaaa", 1.5f);
}

TEST_F(Test_PortfolioTreeWidget, Test_updateLastPrices)
{
    const InSequence seq;

    const Portfolio portfolio;

    EXPECT_CALL(*portfolioTreeModelMock, portfolioChanged(portfolio));
    EXPECT_CALL(*portfolioTreeModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
    EXPECT_CALL(*portfolioTreeModelMock, totalCost()).WillOnce(Return(100000.0));
    EXPECT_CALL(*portfolioTreeModelMock, totalDailyCost()).WillOnce(Return(105000.0));
    EXPECT_CALL(*portfolioTreeModelMock, totalYield()).WillOnce(Return(10000.0));
    EXPECT_CALL(*portfolioTreeModelMock, totalDailyYield()).WillOnce(Return(5000.0));

    portfolioTreeWidget->portfolioChanged(portfolio);

    // clang-format off
    ASSERT_EQ(portfolioTreeWidget->ui->costLabel->text(),        "100000.00 \u20BD");
    ASSERT_EQ(portfolioTreeWidget->ui->allTimeLabel->text(),     "+10000.00 \u20BD(+10.00%)");
    ASSERT_EQ(portfolioTreeWidget->ui->forTodayLabel->text(),    "+5000.00 \u20BD(+4.76%)");
    ASSERT_EQ(portfolioTreeWidget->ui->allTimeLabel->toolTip(),  "From: 100000.00 \u20BD");
    ASSERT_EQ(portfolioTreeWidget->ui->forTodayLabel->toolTip(), "From: 105000.00 \u20BD");
    // clang-format on

    EXPECT_CALL(*portfolioTreeModelMock, updateLastPrices()).WillOnce(Return(true));
    EXPECT_CALL(*portfolioTreeModelMock, totalYield()).WillOnce(Return(0.0));
    EXPECT_CALL(*portfolioTreeModelMock, totalDailyYield()).WillOnce(Return(-5000.0));

    portfolioTreeWidget->updateLastPrices();

    qInfo() << portfolioTreeWidget->ui->forTodayLabel->text();

    // clang-format off
    ASSERT_EQ(portfolioTreeWidget->ui->costLabel->text(),        "100000.00 \u20BD");
    ASSERT_EQ(portfolioTreeWidget->ui->allTimeLabel->text(),     "0.00 \u20BD(0.00%)");
    ASSERT_EQ(portfolioTreeWidget->ui->forTodayLabel->text(),    "-5000.00 \u20BD(-4.76%)");
    ASSERT_EQ(portfolioTreeWidget->ui->allTimeLabel->toolTip(),  "From: 100000.00 \u20BD");
    ASSERT_EQ(portfolioTreeWidget->ui->forTodayLabel->toolTip(), "From: 105000.00 \u20BD");
    // clang-format on
}

TEST_F(Test_PortfolioTreeWidget, Test_on_treeView_customContextMenuRequested)
{
    const QPoint pos;

    portfolioTreeWidget->on_treeView_customContextMenuRequested(pos);
}

TEST_F(Test_PortfolioTreeWidget, Test_actionExportToExcelTriggered)
{
    const InSequence seq;

    StrictMock<FileDialogMock>* fileDialogMock =
        new StrictMock<FileDialogMock>(); // Will be deleted in actionExportToExcelTriggered

    QFile excelFile(appDir + "/test/dir_for_portfolio_tree/excel.xlsx");

    ASSERT_EQ(excelFile.exists(), false);

    EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/PortfolioTreeWidget/exportToExcelFile"), _))
        .WillOnce(Return(QVariant(appDir + "/test/dir_for_portfolio_tree/excel.xlsx")));
    EXPECT_CALL(
        *fileDialogFactoryMock,
        newInstance(
            portfolioTreeWidget, QString("Export"), appDir + "/test/dir_for_portfolio_tree", QString("Excel file (*.xlsx)")
        )
    )
        .WillOnce(Return(std::shared_ptr<IFileDialog>(fileDialogMock)));
    EXPECT_CALL(*fileDialogMock, setAcceptMode(QFileDialog::AcceptSave));
    EXPECT_CALL(*fileDialogMock, setDefaultSuffix(QString("xlsx")));
    EXPECT_CALL(*fileDialogMock, selectFile(appDir + "/test/dir_for_portfolio_tree/excel.xlsx"));
    EXPECT_CALL(*fileDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*fileDialogMock, selectedFiles())
        .WillOnce(Return(QStringList() << appDir + "/test/dir_for_portfolio_tree/excel.xlsx"));
    EXPECT_CALL(
        *settingsEditorMock,
        setValue(
            QString("MainWindow/PortfolioTreeWidget/exportToExcelFile"),
            QVariant(appDir + "/test/dir_for_portfolio_tree/excel.xlsx")
        )
    );
    EXPECT_CALL(*portfolioTreeModelMock, exportToExcel(_));

    portfolioTreeWidget->actionExportToExcelTriggered();

    // clang-format off
    ASSERT_EQ(excelFile.exists(), true);
    ASSERT_GE(excelFile.size(),   6183);
    // clang-format on
}

TEST_F(Test_PortfolioTreeWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Name"),         _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Available"),    _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Price"),        _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_AvgPrice"),     _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Cost"),         _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Part"),         _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Yield"),        _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_YieldPercent"), _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_DailyYield"),   _));
    // clang-format on

    portfolioTreeWidget->saveWindowState("AAAAA");
}

TEST_F(Test_PortfolioTreeWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),         _)).WillOnce(Return(QVariant(172)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Available"),    _)).WillOnce(Return(QVariant(80)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Price"),        _)).WillOnce(Return(QVariant(56)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_AvgPrice"),     _)).WillOnce(Return(QVariant(124)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Cost"),         _)).WillOnce(Return(QVariant(87)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Part"),         _)).WillOnce(Return(QVariant(56)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Yield"),        _)).WillOnce(Return(QVariant(68)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_YieldPercent"), _)).WillOnce(Return(QVariant(79)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_DailyYield"),   _)).WillOnce(Return(QVariant(1100)));
    // clang-format on

    portfolioTreeWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
