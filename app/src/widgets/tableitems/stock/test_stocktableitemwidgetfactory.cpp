#include "src/widgets/tableitems/stock/stocktableitemwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::StrictMock;



TEST(Test_StockTableItemWidgetFactory, Test_constructor_and_destructor)
{
    const StockTableItemWidgetFactory factory;
}

TEST(Test_StockTableItemWidgetFactory, Test_newInstance)
{
    const StockTableItemWidgetFactory factory;

    StrictMock<UserStorageMock> userStorageMock;

    const IStockTableItemWidget* widget = factory.newInstance(&userStorageMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
