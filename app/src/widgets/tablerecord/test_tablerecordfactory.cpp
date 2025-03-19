#include "src/widgets/tablerecord/tablerecordfactory.h"

#include <gtest/gtest.h>

#include "src/utils/http/ihttpclient_mock.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidget_mock.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidgetfactory_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_TableRecordFactory, Test_constructor_and_destructor)
{
    TableRecordFactory factory;
}

TEST(Test_TableRecordFactory, Test_newInstance)
{
    StrictMock<ActionsTableItemWidgetFactoryMock> actionsTableItemWidgetFactoryMock;
    StrictMock<ActionsTableItemWidgetMock>*       actionsTableItemWidgetMock =
        new StrictMock<ActionsTableItemWidgetMock>(); // tableWidget will take ownership
    StrictMock<HttpClientMock> httpClientMock;

    TableRecordFactory factory;

    QTableWidget tableWidget;
    Stock        stock;

    EXPECT_CALL(actionsTableItemWidgetFactoryMock, newInstance(&httpClientMock, &stock, &tableWidget))
        .WillOnce(Return(actionsTableItemWidgetMock));

    ITableRecord* record =
        factory.newInstance(&tableWidget, &actionsTableItemWidgetFactoryMock, &httpClientMock, &stock, nullptr);
    ASSERT_TRUE(record != nullptr);

    delete record;
}
