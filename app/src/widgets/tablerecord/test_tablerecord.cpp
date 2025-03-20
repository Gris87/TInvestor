#include "src/widgets/tablerecord/tablerecord.h"

#include <QPushButton>
#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidget_mock.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidgetfactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_TableRecord : public ::testing::Test
{
protected:
    void SetUp()
    {
        InSequence seq;

        actionsTableItemWidgetFactoryMock = new StrictMock<ActionsTableItemWidgetFactoryMock>();
        orderWavesDialogFactoryMock      = new StrictMock<OrderWavesDialogFactoryMock>();
        orderBookThreadMock             = new StrictMock<OrderBookThreadMock>();
        httpClientMock                    = new StrictMock<HttpClientMock>();
        actionsTableItemWidgetMock        = new StrictMock<ActionsTableItemWidgetMock>(); // tableWidget will take ownership
        tableWidget                       = new QTableWidget();
        stock                             = new Stock();

        tableWidget->setColumnCount(ACTIONS_COLUMN + 1);

        StockData stockData;
        stockData.timestamp = 100;
        stockData.price     = 50;

        stock->meta.uid                    = "aaa";
        stock->meta.ticker                 = "WAGA";
        stock->meta.name                   = "Wata Giga";
        stock->meta.minPriceIncrement.nano = 10000;

        stock->operational.dayStartPrice      = 40;
        stock->operational.specifiedDatePrice = 20;
        stock->operational.payback            = 90;
        stock->operational.detailedData.append(stockData);

        EXPECT_CALL(
            *actionsTableItemWidgetFactoryMock,
            newInstance(orderWavesDialogFactoryMock, orderBookThreadMock, httpClientMock, stock, tableWidget)
        )
            .WillOnce(Return(actionsTableItemWidgetMock));

        record = new TableRecord(
            tableWidget,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderBookThreadMock,
            httpClientMock,
            stock
        );
    }

    void TearDown()
    {
        delete record;
        delete actionsTableItemWidgetFactoryMock;
        delete orderWavesDialogFactoryMock;
        delete orderBookThreadMock;
        delete httpClientMock;
        // It will be deleted by tableWidget
        /*
        delete actionsTableItemWidgetMock;
        */
        delete tableWidget;
        delete stock;
    }

    TableRecord*                                   record;
    StrictMock<ActionsTableItemWidgetFactoryMock>* actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesDialogFactoryMock>*      orderWavesDialogFactoryMock;
    StrictMock<OrderBookThreadMock>*             orderBookThreadMock;
    StrictMock<HttpClientMock>*                    httpClientMock;
    StrictMock<ActionsTableItemWidgetMock>*        actionsTableItemWidgetMock;
    QTableWidget*                                  tableWidget;
    Stock*                                         stock;
};



TEST_F(Test_TableRecord, Test_constructor_and_destructor)
{
}

TEST_F(Test_TableRecord, Test_updateAll)
{
    record->updateAll();

    // clang-format off
    ASSERT_EQ(tableWidget->item(0, STOCK_COLUMN)->data(Qt::DisplayRole),       "WAGA");
    ASSERT_EQ(tableWidget->item(0, STOCK_COLUMN)->toolTip(),                   "Wata Giga");
    ASSERT_EQ(tableWidget->item(0, PRICE_COLUMN)->data(Qt::DisplayRole),       QString("50.00000 ") + QChar(0x20BD));
    ASSERT_EQ(tableWidget->item(0, DAY_CHANGE_COLUMN)->data(Qt::DisplayRole),  "+25.00%");
    ASSERT_EQ(tableWidget->item(0, DATE_CHANGE_COLUMN)->data(Qt::DisplayRole), "+150.00%");
    ASSERT_EQ(tableWidget->item(0, PAYBACK_COLUMN)->data(Qt::DisplayRole),     "90.00%");
    // clang-format on
}

TEST_F(Test_TableRecord, Test_updatePrice)
{
    record->updatePrice();

    // clang-format off
    ASSERT_EQ(tableWidget->item(0, PRICE_COLUMN)->data(Qt::DisplayRole),       QString("50.00000 ") + QChar(0x20BD));
    ASSERT_EQ(tableWidget->item(0, DAY_CHANGE_COLUMN)->data(Qt::DisplayRole),  "+25.00%");
    ASSERT_EQ(tableWidget->item(0, DATE_CHANGE_COLUMN)->data(Qt::DisplayRole), "+150.00%");
    // clang-format on
}

TEST_F(Test_TableRecord, Test_updatePayback)
{
    record->updatePayback();

    ASSERT_EQ(tableWidget->item(0, PAYBACK_COLUMN)->data(Qt::DisplayRole), "90.00%");
}

TEST_F(Test_TableRecord, Test_filter)
{
    Filter filter;

    filter.useTicker          = true;
    filter.ticker             = "WAG";
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

    record->filter(tableWidget, filter);

    ASSERT_EQ(tableWidget->isRowHidden(0), true);

    filter.paybackTo = 95.0f;
    record->filter(tableWidget, filter);

    ASSERT_EQ(tableWidget->isRowHidden(0), false);
}
