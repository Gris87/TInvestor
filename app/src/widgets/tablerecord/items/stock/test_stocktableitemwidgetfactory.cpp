#include "src/widgets/tablerecord/items/stock/stocktableitemwidgetfactory.h"

#include <gtest/gtest.h>



TEST(Test_StockTableItemWidgetFactory, Test_constructor_and_destructor)
{
    StockTableItemWidgetFactory factory;
}

TEST(Test_StockTableItemWidgetFactory, Test_newInstance)
{
    StockTableItemWidgetFactory factory;

    IStockTableItemWidget* widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
