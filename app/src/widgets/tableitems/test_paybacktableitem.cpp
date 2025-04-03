#include "src/widgets/tableitems/paybacktableitem.h"

#include <gtest/gtest.h>



TEST(Test_PaybackTableItem, Test_constructor_and_destructor)
{
    PaybackTableItem item;
}

TEST(Test_PaybackTableItem, Test_setValue_and_getValue)
{
    PaybackTableItem item;

    item.setValue(90);
    ASSERT_EQ(item.getValue(), 90);
    ASSERT_EQ(item.data(Qt::DisplayRole), "90.00%");
    ASSERT_EQ(item.foreground().color(), QColor("#2BD793"));

    item.setValue(50);
    ASSERT_EQ(item.getValue(), 50);
    ASSERT_EQ(item.data(Qt::DisplayRole), "50.00%");
    ASSERT_EQ(item.foreground().color(), QColor("#97AEC4"));

    item.setValue(10);
    ASSERT_EQ(item.getValue(), 10);
    ASSERT_EQ(item.data(Qt::DisplayRole), "10.00%");
    ASSERT_EQ(item.foreground().color(), QColor("#ED6F7E"));
}

TEST(Test_PaybackTableItem, Test_less)
{
    PaybackTableItem item;
    PaybackTableItem item2;

    item.setValue(10);
    item2.setValue(20);

    ASSERT_LT(item, item2);

    item.setValue(30);

    ASSERT_LT(item2, item);
}
