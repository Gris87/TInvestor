#include "src/widgets/tablerecords/stockstablerecord/stockstablerecord.h"

#include <QPushButton>
#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidget_mock.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/stock/istocktableitemwidget_mock.h"
#include "src/widgets/tableitems/stock/istocktableitemwidgetfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_StocksTableRecord : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        stockTableItemWidgetFactoryMock   = new StrictMock<StockTableItemWidgetFactoryMock>();
        actionsTableItemWidgetFactoryMock = new StrictMock<ActionsTableItemWidgetFactoryMock>();
        orderWavesDialogFactoryMock       = new StrictMock<OrderWavesDialogFactoryMock>();
        orderWavesWidgetFactoryMock       = new StrictMock<OrderWavesWidgetFactoryMock>();
        userStorageMock                   = new StrictMock<UserStorageMock>();
        orderBookThreadMock               = new StrictMock<OrderBookThreadMock>();
        httpClientMock                    = new StrictMock<HttpClientMock>();
        stockTableItemWidgetMock          = new StrictMock<StockTableItemWidgetMock>();   // tableWidget will take ownership
        actionsTableItemWidgetMock        = new StrictMock<ActionsTableItemWidgetMock>(); // tableWidget will take ownership
        tableWidget                       = new QTableWidget();
        stock                             = new Stock();

        tableWidget->setColumnCount(COLUMN_COUNT);

        StockOperationalData stockData;
        stockData.timestamp = 100;
        stockData.price     = 50;

        stock->meta.uid                    = "aaa";
        stock->meta.ticker                 = "WAGA";
        stock->meta.name                   = "Wata Giga";
        stock->meta.forQualInvestorFlag    = true;
        stock->meta.minPriceIncrement.nano = 10000;

        stock->operational.dayStartPrice      = 40;
        stock->operational.specifiedDatePrice = 20;
        stock->operational.turnover           = 1500000000;
        stock->operational.payback            = 90;
        stock->operational.detailedData.append(stockData);

        EXPECT_CALL(*stockTableItemWidgetFactoryMock, newInstance(userStorageMock, tableWidget))
            .WillOnce(Return(stockTableItemWidgetMock));

        EXPECT_CALL(
            *actionsTableItemWidgetFactoryMock,
            newInstance(
                orderWavesDialogFactoryMock,
                orderWavesWidgetFactoryMock,
                orderBookThreadMock,
                httpClientMock,
                stock,
                5,
                tableWidget
            )
        )
            .WillOnce(Return(actionsTableItemWidgetMock));

        EXPECT_CALL(*stockTableItemWidgetMock, setIcon(_));
        EXPECT_CALL(*stockTableItemWidgetMock, setQualInvestor(true));
        EXPECT_CALL(*stockTableItemWidgetMock, setText(QString("WAGA")));
        EXPECT_CALL(*stockTableItemWidgetMock, setFullText(QString("Wata Giga")));

        record = new StocksTableRecord(
            tableWidget,
            stockTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            stock
        );
    }

    void TearDown() override
    {
        delete record;
        delete stockTableItemWidgetFactoryMock;
        delete actionsTableItemWidgetFactoryMock;
        delete orderWavesDialogFactoryMock;
        delete orderWavesWidgetFactoryMock;
        delete userStorageMock;
        delete orderBookThreadMock;
        delete httpClientMock;
        // It will be deleted by tableWidget
        /*
        delete stockTableItemWidgetMock;
        delete actionsTableItemWidgetMock;
        */
        delete tableWidget;
        delete stock;
    }

    StocksTableRecord*                             record;
    StrictMock<StockTableItemWidgetFactoryMock>*   stockTableItemWidgetFactoryMock;
    StrictMock<ActionsTableItemWidgetFactoryMock>* actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesDialogFactoryMock>*       orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>*       orderWavesWidgetFactoryMock;
    StrictMock<UserStorageMock>*                   userStorageMock;
    StrictMock<OrderBookThreadMock>*               orderBookThreadMock;
    StrictMock<HttpClientMock>*                    httpClientMock;
    StrictMock<StockTableItemWidgetMock>*          stockTableItemWidgetMock;
    StrictMock<ActionsTableItemWidgetMock>*        actionsTableItemWidgetMock;
    QTableWidget*                                  tableWidget;
    Stock*                                         stock;
};



TEST_F(Test_StocksTableRecord, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksTableRecord, Test_updateAll)
{
    const InSequence seq;

    EXPECT_CALL(*stockTableItemWidgetMock, setIcon(_));
    EXPECT_CALL(*stockTableItemWidgetMock, setQualInvestor(true));
    EXPECT_CALL(*stockTableItemWidgetMock, setText(QString("WAGA")));
    EXPECT_CALL(*stockTableItemWidgetMock, setFullText(QString("Wata Giga")));

    record->updateAll();

    // clang-format off
    ASSERT_EQ(tableWidget->item(0, PRICE_COLUMN)->data(Qt::DisplayRole),       QString("50.00000 ") + QChar(0x20BD));
    ASSERT_EQ(tableWidget->item(0, DAY_CHANGE_COLUMN)->data(Qt::DisplayRole),  "+25.00%");
    ASSERT_EQ(tableWidget->item(0, DATE_CHANGE_COLUMN)->data(Qt::DisplayRole), "+150.00%");
    ASSERT_EQ(tableWidget->item(0, TURNOVER_COLUMN)->data(Qt::DisplayRole),    QString("1.50B ") + QChar(0x20BD));
    ASSERT_EQ(tableWidget->item(0, PAYBACK_COLUMN)->data(Qt::DisplayRole),     "90.00%");
    // clang-format on
}

TEST_F(Test_StocksTableRecord, Test_updatePrice)
{
    record->updatePrice();

    // clang-format off
    ASSERT_EQ(tableWidget->item(0, PRICE_COLUMN)->data(Qt::DisplayRole),       QString("50.00000 ") + QChar(0x20BD));
    ASSERT_EQ(tableWidget->item(0, DAY_CHANGE_COLUMN)->data(Qt::DisplayRole),  "+25.00%");
    ASSERT_EQ(tableWidget->item(0, DATE_CHANGE_COLUMN)->data(Qt::DisplayRole), "+150.00%");
    // clang-format on
}

TEST_F(Test_StocksTableRecord, Test_updatePeriodicData)
{
    ASSERT_EQ(tableWidget->item(0, TURNOVER_COLUMN)->data(Qt::DisplayRole), QString("1.50B ") + QChar(0x20BD));
    ASSERT_EQ(tableWidget->item(0, PAYBACK_COLUMN)->data(Qt::DisplayRole), "90.00%");

    stock->operational.turnover = 1250000;
    stock->operational.payback  = 60;

    record->updatePeriodicData();

    ASSERT_EQ(tableWidget->item(0, TURNOVER_COLUMN)->data(Qt::DisplayRole), QString("1.25M ") + QChar(0x20BD));
    ASSERT_EQ(tableWidget->item(0, PAYBACK_COLUMN)->data(Qt::DisplayRole), "60.00%");
}

TEST_F(Test_StocksTableRecord, Test_filter)
{
    // const InSequence seq;

    Filter filter;

    filter.useTicker          = true;
    filter.ticker             = "WAG";
    filter.useQualInvestor    = true;
    filter.qualInvestor       = QUAL_INVESTOR_ONLY_WITH_STATUS;
    filter.usePrice           = true;
    filter.priceFrom          = 20.0f;
    filter.priceTo            = 100.0f;
    filter.useDayStartChange  = true;
    filter.dayStartChangeFrom = 20.0f;
    filter.dayStartChangeTo   = 30.0f;
    filter.useDateChange      = true;
    filter.dateChangeFrom     = 100.0f;
    filter.dateChangeTo       = 200.0f;
    filter.usePayback         = true;
    filter.paybackFrom        = 70.0f;
    filter.paybackTo          = 80.0f;

    ASSERT_EQ(tableWidget->isRowHidden(0), false);

    EXPECT_CALL(*stockTableItemWidgetMock, text()).WillOnce(Return("WAGA"));
    EXPECT_CALL(*stockTableItemWidgetMock, fullText()).WillOnce(Return("Wata Giga"));
    EXPECT_CALL(*stockTableItemWidgetMock, forQualInvestorFlag()).WillOnce(Return(true));

    record->filter(tableWidget, filter);

    ASSERT_EQ(tableWidget->isRowHidden(0), true);

    EXPECT_CALL(*stockTableItemWidgetMock, text()).WillOnce(Return("WAGA"));
    EXPECT_CALL(*stockTableItemWidgetMock, fullText()).WillOnce(Return("Wata Giga"));
    EXPECT_CALL(*stockTableItemWidgetMock, forQualInvestorFlag()).WillOnce(Return(true));

    filter.paybackTo = 95.0f;
    record->filter(tableWidget, filter);

    ASSERT_EQ(tableWidget->isRowHidden(0), false);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
