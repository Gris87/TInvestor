#include "src/widgets/tableitems/moneytableitem.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_MoneyTableItem, Test_constructor_and_destructor)
{
    const MoneyTableItem item;
}

TEST(Test_MoneyTableItem, Test_setValue_and_getValue)
{
    MoneyTableItem item;

    item.setValue(10, 2);
    ASSERT_EQ(item.getValue(), 10);
    ASSERT_EQ(item.data(Qt::DisplayRole), QString("10.00 \u20BD"));

    item.setValue(20, 3);
    ASSERT_EQ(item.getValue(), 20);
    ASSERT_EQ(item.data(Qt::DisplayRole), QString("20.000 \u20BD"));
}

TEST(Test_MoneyTableItem, Test_less)
{
    MoneyTableItem item;
    MoneyTableItem item2;

    item.setValue(10, 2);
    item2.setValue(20, 2);

    ASSERT_LT(item, item2);

    item.setValue(30, 2);

    ASSERT_LT(item2, item);
}
// NOLINTEND(readability-magic-numbers)
