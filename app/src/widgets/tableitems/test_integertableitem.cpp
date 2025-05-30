#include "src/widgets/tableitems/integertableitem.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_IntegerTableItem, Test_constructor_and_destructor)
{
    const IntegerTableItem item;
}

TEST(Test_IntegerTableItem, Test_setValue_and_value)
{
    IntegerTableItem item;

    item.setValue(1500000000);
    ASSERT_EQ(item.value(), 1500000000);
    ASSERT_EQ(item.data(Qt::DisplayRole), QString("1500000000"));

    item.setValue(1500000);
    ASSERT_EQ(item.value(), 1500000);
    ASSERT_EQ(item.data(Qt::DisplayRole), QString("1500000"));

    item.setValue(1500);
    ASSERT_EQ(item.value(), 1500);
    ASSERT_EQ(item.data(Qt::DisplayRole), QString("1500"));
}

TEST(Test_IntegerTableItem, Test_less)
{
    IntegerTableItem item;
    IntegerTableItem item2;

    item.setValue(10);
    item2.setValue(20);

    ASSERT_LT(item, item2);

    item.setValue(30);

    ASSERT_LT(item2, item);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
