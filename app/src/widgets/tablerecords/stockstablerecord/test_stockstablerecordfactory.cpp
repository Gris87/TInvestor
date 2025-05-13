#include "src/widgets/tablerecords/stockstablerecord/stockstablerecordfactory.h"

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



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
TEST(Test_StocksTableRecordFactory, Test_constructor_and_destructor)
{
    const StocksTableRecordFactory factory;
}

TEST(Test_StocksTableRecordFactory, Test_newInstance)
{
    const InSequence seq;

    const StocksTableRecordFactory factory;

    StrictMock<StockTableItemWidgetFactoryMock>   stockTableItemWidgetFactoryMock;
    StrictMock<ActionsTableItemWidgetFactoryMock> actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesDialogFactoryMock>       orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>       orderWavesWidgetFactoryMock;
    StrictMock<UserStorageMock>                   userStorageMock;
    StrictMock<OrderBookThreadMock>               orderBookThreadMock;
    StrictMock<HttpClientMock>                    httpClientMock;

    // tableWidget will take ownership
    StrictMock<StockTableItemWidgetMock>*   stockTableItemWidgetMock   = new StrictMock<StockTableItemWidgetMock>();
    StrictMock<ActionsTableItemWidgetMock>* actionsTableItemWidgetMock = new StrictMock<ActionsTableItemWidgetMock>();

    QTableWidget tableWidget;
    Stock        stock;

    tableWidget.setColumnCount(STOCKS_COLUMN_COUNT);

    StockOperationalData stockData;
    stockData.timestamp = 100;
    stockData.price     = 50;

    stock.meta.uid                    = "aaa";
    stock.meta.ticker                 = "WAGA";
    stock.meta.name                   = "Wata Giga";
    stock.meta.forQualInvestorFlag    = true;
    stock.meta.minPriceIncrement.nano = 10000;

    stock.operational.dayStartPrice      = 40;
    stock.operational.specifiedDatePrice = 20;
    stock.operational.turnover           = 1500000000;
    stock.operational.payback            = 90;
    stock.operational.detailedData.append(stockData);

    EXPECT_CALL(stockTableItemWidgetFactoryMock, newInstance(&userStorageMock, &tableWidget))
        .WillOnce(Return(stockTableItemWidgetMock));

    EXPECT_CALL(
        actionsTableItemWidgetFactoryMock,
        newInstance(
            &orderWavesDialogFactoryMock,
            &orderWavesWidgetFactoryMock,
            &orderBookThreadMock,
            &httpClientMock,
            &stock,
            5,
            &tableWidget
        )
    )
        .WillOnce(Return(actionsTableItemWidgetMock));

    const IStocksTableRecord* record = factory.newInstance(
        &tableWidget,
        &stockTableItemWidgetFactoryMock,
        &actionsTableItemWidgetFactoryMock,
        &orderWavesDialogFactoryMock,
        &orderWavesWidgetFactoryMock,
        &userStorageMock,
        &orderBookThreadMock,
        &httpClientMock,
        &stock,
        nullptr
    );
    ASSERT_TRUE(record != nullptr);

    delete record;
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
