#include "src/widgets/tablerecord/tablerecordfactory.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidget_mock.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecord/items/stock/istocktableitemwidget_mock.h"
#include "src/widgets/tablerecord/items/stock/istocktableitemwidgetfactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_TableRecordFactory, Test_constructor_and_destructor)
{
    TableRecordFactory factory;
}

TEST(Test_TableRecordFactory, Test_newInstance)
{
    InSequence seq;

    StrictMock<StockTableItemWidgetFactoryMock> stockTableItemWidgetFactoryMock;
    StrictMock<StockTableItemWidgetMock>*       stockTableItemWidgetMock =
        new StrictMock<StockTableItemWidgetMock>(); // tableWidget will take ownership
    StrictMock<ActionsTableItemWidgetFactoryMock> actionsTableItemWidgetFactoryMock;
    StrictMock<ActionsTableItemWidgetMock>*       actionsTableItemWidgetMock =
        new StrictMock<ActionsTableItemWidgetMock>(); // tableWidget will take ownership
    StrictMock<OrderWavesDialogFactoryMock> orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock> orderWavesWidgetFactoryMock;
    StrictMock<UserStorageMock>             userStorageMock;
    StrictMock<OrderBookThreadMock>         orderBookThreadMock;
    StrictMock<HttpClientMock>              httpClientMock;

    TableRecordFactory factory;

    QTableWidget tableWidget;
    Stock        stock;

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
            2,
            &tableWidget
        )
    )
        .WillOnce(Return(actionsTableItemWidgetMock));

    ITableRecord* record = factory.newInstance(
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
