#include "src/widgets/tablerecord/items/pricetableitem.h"

#include <gtest/gtest.h>



TEST(Test_PriceTableItem, Test_constructor_and_destructor)
{
    PriceTableItem item;
}

TEST(Test_PriceTableItem, Test_setValue_and_getValue)
{
    PriceTableItem item;

    item.setValue(10, 2);
    ASSERT_EQ(item.getValue(), 10);
    ASSERT_EQ(item.data(Qt::DisplayRole), QString("10.00 ") + QChar(0x20BD));

    item.setValue(20, 3);
    ASSERT_EQ(item.getValue(), 20);
    ASSERT_EQ(item.data(Qt::DisplayRole), QString("20.000 ") + QChar(0x20BD));
}

TEST(Test_PriceTableItem, Test_less)
{
    PriceTableItem item;
    PriceTableItem item2;

    item.setValue(10, 2);
    item2.setValue(20, 2);

    ASSERT_LT(item, item2);

    item.setValue(30, 2);

    ASSERT_LT(item2, item);
}
