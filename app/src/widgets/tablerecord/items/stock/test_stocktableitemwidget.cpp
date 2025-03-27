#include "src/widgets/tablerecord/items/stock/stocktableitemwidget.h"
#include "ui_stocktableitemwidget.h"

#include <gtest/gtest.h>



class Test_StockTableItemWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        widget = new StockTableItemWidget();
    }

    void TearDown()
    {
        delete widget;
    }

    StockTableItemWidget* widget;
};



TEST_F(Test_StockTableItemWidget, Test_constructor_and_destructor)
{
}
