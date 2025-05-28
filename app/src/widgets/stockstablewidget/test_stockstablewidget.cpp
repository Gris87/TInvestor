#include "src/widgets/stockstablewidget/stockstablewidget.h"
#include "ui_stockstablewidget.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecord_mock.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_StocksTableWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stockTableRecordFactoryMock          = new StrictMock<StocksTableRecordFactoryMock>();
        instrumentTableItemWidgetFactoryMock = new StrictMock<InstrumentTableItemWidgetFactoryMock>();
        actionsTableItemWidgetFactoryMock    = new StrictMock<ActionsTableItemWidgetFactoryMock>();
        orderWavesDialogFactoryMock          = new StrictMock<OrderWavesDialogFactoryMock>();
        orderWavesWidgetFactoryMock          = new StrictMock<OrderWavesWidgetFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();
        orderBookThreadMock                  = new StrictMock<OrderBookThreadMock>();
        httpClientMock                       = new StrictMock<HttpClientMock>();
        fileDialogFactoryMock                = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();

        stocksTableWidget = new StocksTableWidget(
            stockTableRecordFactoryMock,
            instrumentTableItemWidgetFactoryMock,
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
        delete stockTableRecordFactoryMock;
        delete instrumentTableItemWidgetFactoryMock;
        delete actionsTableItemWidgetFactoryMock;
        delete orderWavesDialogFactoryMock;
        delete orderWavesWidgetFactoryMock;
        delete userStorageMock;
        delete orderBookThreadMock;
        delete httpClientMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
    }

    StocksTableWidget*                                stocksTableWidget;
    StrictMock<StocksTableRecordFactoryMock>*         stockTableRecordFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock>* instrumentTableItemWidgetFactoryMock;
    StrictMock<ActionsTableItemWidgetFactoryMock>*    actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesDialogFactoryMock>*          orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>*          orderWavesWidgetFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<OrderBookThreadMock>*                  orderBookThreadMock;
    StrictMock<HttpClientMock>*                       httpClientMock;
    StrictMock<FileDialogFactoryMock>*                fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*                   settingsEditorMock;
};



TEST_F(Test_StocksTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksTableWidget, Test_updateTable)
{
    const InSequence seq;

    StrictMock<StocksTableRecordMock> stocksTableRecordMock1;
    StrictMock<StocksTableRecordMock> stocksTableRecordMock2;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    const Filter filter;

    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock1,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock1));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock2,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock2));
    EXPECT_CALL(stocksTableRecordMock2, updateAll());
    EXPECT_CALL(stocksTableRecordMock2, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->updateTable(stocks, filter);

    // clang-format off
    ASSERT_EQ(stocksTableWidget->records.size(),   2);
    ASSERT_EQ(stocksTableWidget->records["aaaaa"], &stocksTableRecordMock1);
    ASSERT_EQ(stocksTableWidget->records["bbbbb"], &stocksTableRecordMock2);
    // clang-format on
}

TEST_F(Test_StocksTableWidget, Test_updateAll)
{
    const InSequence seq;

    StrictMock<StocksTableRecordMock> stocksTableRecordMock1;
    StrictMock<StocksTableRecordMock> stocksTableRecordMock2;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    const Filter filter;

    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock1,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock1));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock2,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock2));
    EXPECT_CALL(stocksTableRecordMock2, updateAll());
    EXPECT_CALL(stocksTableRecordMock2, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->updateTable(stocks, filter);

    // clang-format off
    ASSERT_EQ(stocksTableWidget->records.size(),   2);
    ASSERT_EQ(stocksTableWidget->records["aaaaa"], &stocksTableRecordMock1);
    ASSERT_EQ(stocksTableWidget->records["bbbbb"], &stocksTableRecordMock2);
    // clang-format on

    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(stocksTableRecordMock2, updateAll());
    EXPECT_CALL(stocksTableRecordMock2, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->updateAll(filter);
}

TEST_F(Test_StocksTableWidget, Test_updateLastPrices)
{
    const InSequence seq;

    StrictMock<StocksTableRecordMock> stocksTableRecordMock1;
    StrictMock<StocksTableRecordMock> stocksTableRecordMock2;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    const Filter filter;

    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock1,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock1));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock2,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock2));
    EXPECT_CALL(stocksTableRecordMock2, updateAll());
    EXPECT_CALL(stocksTableRecordMock2, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->updateTable(stocks, filter);

    // clang-format off
    ASSERT_EQ(stocksTableWidget->records.size(),   2);
    ASSERT_EQ(stocksTableWidget->records["aaaaa"], &stocksTableRecordMock1);
    ASSERT_EQ(stocksTableWidget->records["bbbbb"], &stocksTableRecordMock2);
    // clang-format on

    stocksTableWidget->lastPriceChanged("aaaaa");

    // clang-format off
    ASSERT_EQ(stocksTableWidget->lastPricesUpdates.size(),            1);
    ASSERT_EQ(stocksTableWidget->lastPricesUpdates.contains("aaaaa"), true);
    // clang-format on

    EXPECT_CALL(stocksTableRecordMock1, updatePrice());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->updateLastPrices(filter);

    ASSERT_EQ(stocksTableWidget->lastPricesUpdates.isEmpty(), true);
}

TEST_F(Test_StocksTableWidget, Test_updatePrices)
{
    const InSequence seq;

    StrictMock<StocksTableRecordMock> stocksTableRecordMock1;
    StrictMock<StocksTableRecordMock> stocksTableRecordMock2;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    const Filter filter;

    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock1,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock1));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock2,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock2));
    EXPECT_CALL(stocksTableRecordMock2, updateAll());
    EXPECT_CALL(stocksTableRecordMock2, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->updateTable(stocks, filter);

    // clang-format off
    ASSERT_EQ(stocksTableWidget->records.size(),   2);
    ASSERT_EQ(stocksTableWidget->records["aaaaa"], &stocksTableRecordMock1);
    ASSERT_EQ(stocksTableWidget->records["bbbbb"], &stocksTableRecordMock2);
    // clang-format on

    EXPECT_CALL(stocksTableRecordMock1, updatePrice());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(stocksTableRecordMock2, updatePrice());
    EXPECT_CALL(stocksTableRecordMock2, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->updatePrices(filter);
}

TEST_F(Test_StocksTableWidget, Test_updatePeriodicData)
{
    const InSequence seq;

    StrictMock<StocksTableRecordMock> stocksTableRecordMock1;
    StrictMock<StocksTableRecordMock> stocksTableRecordMock2;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    const Filter filter;

    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock1,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock1));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock2,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock2));
    EXPECT_CALL(stocksTableRecordMock2, updateAll());
    EXPECT_CALL(stocksTableRecordMock2, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->updateTable(stocks, filter);

    // clang-format off
    ASSERT_EQ(stocksTableWidget->records.size(),   2);
    ASSERT_EQ(stocksTableWidget->records["aaaaa"], &stocksTableRecordMock1);
    ASSERT_EQ(stocksTableWidget->records["bbbbb"], &stocksTableRecordMock2);
    // clang-format on

    EXPECT_CALL(stocksTableRecordMock1, updatePeriodicData());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(stocksTableRecordMock2, updatePeriodicData());
    EXPECT_CALL(stocksTableRecordMock2, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->updatePeriodicData(filter);
}

TEST_F(Test_StocksTableWidget, Test_setDateChangeTooltip)
{
    ASSERT_EQ(stocksTableWidget->ui->tableWidget->horizontalHeaderItem(STOCKS_DATE_CHANGE_COLUMN)->toolTip(), "");

    stocksTableWidget->setDateChangeTooltip("AAAAA");
    ASSERT_EQ(stocksTableWidget->ui->tableWidget->horizontalHeaderItem(STOCKS_DATE_CHANGE_COLUMN)->toolTip(), "AAAAA");

    stocksTableWidget->setDateChangeTooltip("BBBBB");
    ASSERT_EQ(stocksTableWidget->ui->tableWidget->horizontalHeaderItem(STOCKS_DATE_CHANGE_COLUMN)->toolTip(), "BBBBB");
}

TEST_F(Test_StocksTableWidget, Test_filterChanged)
{
    const InSequence seq;

    StrictMock<StocksTableRecordMock> stocksTableRecordMock1;
    StrictMock<StocksTableRecordMock> stocksTableRecordMock2;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    const Filter filter;

    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock1,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock1));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(stocksTableRecordMock1, updateAll());
    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(
        *stockTableRecordFactoryMock,
        newInstance(
            stocksTableWidget->ui->tableWidget,
            instrumentTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            &stock2,
            stocksTableWidget
        )
    )
        .WillOnce(Return(&stocksTableRecordMock2));
    EXPECT_CALL(stocksTableRecordMock2, updateAll());
    EXPECT_CALL(stocksTableRecordMock2, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->updateTable(stocks, filter);

    // clang-format off
    ASSERT_EQ(stocksTableWidget->records.size(),   2);
    ASSERT_EQ(stocksTableWidget->records["aaaaa"], &stocksTableRecordMock1);
    ASSERT_EQ(stocksTableWidget->records["bbbbb"], &stocksTableRecordMock2);
    // clang-format on

    EXPECT_CALL(stocksTableRecordMock1, filter(stocksTableWidget->ui->tableWidget, filter));
    EXPECT_CALL(stocksTableRecordMock2, filter(stocksTableWidget->ui->tableWidget, filter));

    stocksTableWidget->filterChanged(filter);
}

TEST_F(Test_StocksTableWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Stock"),      _));
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
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Stock"),      _)).WillOnce(Return(QVariant(99)));
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
