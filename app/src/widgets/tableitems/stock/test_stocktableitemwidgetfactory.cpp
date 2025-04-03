#include "src/widgets/tableitems/stock/stocktableitemwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_StockTableItemWidgetFactory, Test_constructor_and_destructor)
{
    StockTableItemWidgetFactory factory;
}

TEST(Test_StockTableItemWidgetFactory, Test_newInstance)
{
    StockTableItemWidgetFactory factory;

    StrictMock<UserStorageMock> userStorageMock;

    IStockTableItemWidget* widget = factory.newInstance(&userStorageMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
