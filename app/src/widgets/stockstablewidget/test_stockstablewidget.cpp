#include "src/widgets/stockstablewidget/stockstablewidget.h"
#include "ui_stockstablewidget.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/filedialog/ifiledialog_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/actionstableitemwidget/iactionstableitemwidget_mock.h"
#include "src/widgets/actionstableitemwidget/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel_mock.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodelfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_StocksTableWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        stocksTableModelFactoryMock       = new StrictMock<StocksTableModelFactoryMock>();
        actionsTableItemWidgetFactoryMock = new StrictMock<ActionsTableItemWidgetFactoryMock>();
        orderWavesDialogFactoryMock       = new StrictMock<OrderWavesDialogFactoryMock>();
        orderWavesWidgetFactoryMock       = new StrictMock<OrderWavesWidgetFactoryMock>();
        userStorageMock                   = new StrictMock<UserStorageMock>();
        orderBookThreadMock               = new StrictMock<OrderBookThreadMock>();
        httpClientMock                    = new StrictMock<HttpClientMock>();
        fileDialogFactoryMock             = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock                = new StrictMock<SettingsEditorMock>();

        stocksTableModelMock = new StrictMock<StocksTableModelMock>();

        appDir = qApp->applicationDirPath();
        QDir(appDir + "/test/dir_for_stocks_table").removeRecursively();
        QDir().mkpath(appDir + "/test/dir_for_stocks_table");

        EXPECT_CALL(*stocksTableModelFactoryMock, newInstance(userStorageMock, NotNull())).WillOnce(Return(stocksTableModelMock));
        EXPECT_CALL(*stocksTableModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
        EXPECT_CALL(*stocksTableModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(0));

        stocksTableWidget = new StocksTableWidget(
            stocksTableModelFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            fileDialogFactoryMock,
            settingsEditorMock
        );
    }

    void TearDown() override
    {
        delete stocksTableWidget;
        delete stocksTableModelFactoryMock;
        delete actionsTableItemWidgetFactoryMock;
        delete orderWavesDialogFactoryMock;
        delete orderWavesWidgetFactoryMock;
        delete userStorageMock;
        delete orderBookThreadMock;
        delete httpClientMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
        delete stocksTableModelMock;

        QDir(appDir + "/test/dir_for_stocks_table").removeRecursively();
    }

    StocksTableWidget*                             stocksTableWidget;
    StrictMock<StocksTableModelFactoryMock>*       stocksTableModelFactoryMock;
    StrictMock<ActionsTableItemWidgetFactoryMock>* actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesDialogFactoryMock>*       orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>*       orderWavesWidgetFactoryMock;
    StrictMock<UserStorageMock>*                   userStorageMock;
    StrictMock<OrderBookThreadMock>*               orderBookThreadMock;
    StrictMock<HttpClientMock>*                    httpClientMock;
    StrictMock<FileDialogFactoryMock>*             fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*                settingsEditorMock;
    StrictMock<StocksTableModelMock>*              stocksTableModelMock;
    QString                                        appDir;
};



TEST_F(Test_StocksTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksTableWidget, Test_setFilter)
{
    const InSequence seq;

    const StockFilter filter;

    EXPECT_CALL(*stocksTableModelMock, setFilter(filter));

    stocksTableWidget->setFilter(filter);
}

TEST_F(Test_StocksTableWidget, Test_updateTable)
{
    const InSequence seq;

    const QList<Stock*> stocks;

    EXPECT_CALL(*stocksTableModelMock, updateTable(stocks));

    stocksTableWidget->updateTable(stocks);
}

TEST_F(Test_StocksTableWidget, Test_updateAll)
{
    const InSequence seq;

    const QList<Stock*> stocks;

    EXPECT_CALL(*stocksTableModelMock, updateAll());

    stocksTableWidget->updateAll();
}

TEST_F(Test_StocksTableWidget, Test_lastPriceChanged)
{
    const InSequence seq;

    EXPECT_CALL(*stocksTableModelMock, lastPriceChanged(QString("aaaaa")));

    stocksTableWidget->lastPriceChanged("aaaaa");
}

TEST_F(Test_StocksTableWidget, Test_updateLastPrices)
{
    const InSequence seq;

    EXPECT_CALL(*stocksTableModelMock, updateLastPrices());

    stocksTableWidget->updateLastPrices();
}

TEST_F(Test_StocksTableWidget, Test_updatePrices)
{
    const InSequence seq;

    EXPECT_CALL(*stocksTableModelMock, updatePrices());

    stocksTableWidget->updatePrices();
}

TEST_F(Test_StocksTableWidget, Test_setDateChangeTooltip)
{
    const InSequence seq;

    EXPECT_CALL(*stocksTableModelMock, setDateChangeTooltip(QString("Blah Blah")));

    stocksTableWidget->setDateChangeTooltip("Blah Blah");
}

TEST_F(Test_StocksTableWidget, Test_updatePeriodicData)
{
    const InSequence seq;

    EXPECT_CALL(*stocksTableModelMock, updatePeriodicData());

    stocksTableWidget->updatePeriodicData();
}

TEST_F(Test_StocksTableWidget, Test_on_tableView_customContextMenuRequested)
{
    const QPoint pos;

    stocksTableWidget->on_tableView_customContextMenuRequested(pos);
}

TEST_F(Test_StocksTableWidget, Test_actionExportToExcelTriggered)
{
    const InSequence seq;

    StrictMock<FileDialogMock>* fileDialogMock =
        new StrictMock<FileDialogMock>(); // Will be deleted in actionExportToExcelTriggered

    QFile excelFile(appDir + "/test/dir_for_stocks_table/excel.xlsx");

    ASSERT_EQ(excelFile.exists(), false);

    EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/StocksTableWidget/exportToExcelFile"), _))
        .WillOnce(Return(QVariant(appDir + "/test/dir_for_stocks_table/excel.xlsx")));
    EXPECT_CALL(
        *fileDialogFactoryMock,
        newInstance(stocksTableWidget, QString("Export"), appDir + "/test/dir_for_stocks_table", QString("Excel file (*.xlsx)"))
    )
        .WillOnce(Return(std::shared_ptr<IFileDialog>(fileDialogMock)));
    EXPECT_CALL(*fileDialogMock, setAcceptMode(QFileDialog::AcceptSave));
    EXPECT_CALL(*fileDialogMock, setDefaultSuffix(QString("xlsx")));
    EXPECT_CALL(*fileDialogMock, selectFile(appDir + "/test/dir_for_stocks_table/excel.xlsx"));
    EXPECT_CALL(*fileDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*fileDialogMock, selectedFiles())
        .WillOnce(Return(QStringList() << appDir + "/test/dir_for_stocks_table/excel.xlsx"));
    EXPECT_CALL(
        *settingsEditorMock,
        setValue(
            QString("MainWindow/StocksTableWidget/exportToExcelFile"), QVariant(appDir + "/test/dir_for_stocks_table/excel.xlsx")
        )
    );
    EXPECT_CALL(*stocksTableModelMock, exportToExcel(_));

    stocksTableWidget->actionExportToExcelTriggered();

    // clang-format off
    ASSERT_EQ(excelFile.exists(), true);
    ASSERT_GE(excelFile.size(),   6251);
    // clang-format on
}

TEST_F(Test_StocksTableWidget, Test_modelReset)
{
    const InSequence seq;

    // Will be deleted in StocksTableWidget destructor
    StrictMock<ActionsTableItemWidgetMock>* actionsTableItemWidgetMock = new StrictMock<ActionsTableItemWidgetMock>();

    EXPECT_CALL(*stocksTableModelMock, rowCount(QModelIndex())).WillOnce(Return(1));
    EXPECT_CALL(
        *actionsTableItemWidgetFactoryMock,
        newInstance(
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            orderBookThreadMock,
            httpClientMock,
            stocksTableModelMock,
            0,
            stocksTableWidget
        )
    )
        .WillOnce(Return(actionsTableItemWidgetMock));
    EXPECT_CALL(*stocksTableModelMock, rowCount(QModelIndex())).WillOnce(Return(1));
    EXPECT_CALL(*stocksTableModelMock, rowCount(QModelIndex())).WillOnce(Return(1));

    stocksTableWidget->modelReset();
}

TEST_F(Test_StocksTableWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Name"),       _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Price"),      _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_DayChange"),  _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_DateChange"), _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Turnover"),   _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Payback"),    _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Actions"),    _));
    // clang-format on

    stocksTableWidget->saveWindowState("AAAAA");
}

TEST_F(Test_StocksTableWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),       _)).WillOnce(Return(QVariant(103)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Price"),      _)).WillOnce(Return(QVariant(61)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_DayChange"),  _)).WillOnce(Return(QVariant(139)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_DateChange"), _)).WillOnce(Return(QVariant(157)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Turnover"),   _)).WillOnce(Return(QVariant(86)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Payback"),    _)).WillOnce(Return(QVariant(120)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Actions"),    _)).WillOnce(Return(QVariant(83)));
    // clang-format on

    stocksTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
