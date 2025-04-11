#include "src/widgets/tableitems/pricechangetableitem.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_PriceChangeTableItem, Test_constructor_and_destructor)
{
    const PriceChangeTableItem item;
}

TEST(Test_PriceChangeTableItem, Test_setValue_and_getValue)
{
    PriceChangeTableItem item;

    item.setValue(-1, 100, 2);
    ASSERT_EQ(item.getValue(), -1);
    ASSERT_EQ(item.data(Qt::DisplayRole), "-1.00%");
    ASSERT_EQ(item.toolTip(), QString("From price: 100.00 ") + QChar(0x20BD));
    ASSERT_EQ(item.foreground().color(), QColor("#ED6F7E"));

    item.setValue(0, -1, 2);
    ASSERT_EQ(item.getValue(), 0);
    ASSERT_EQ(item.data(Qt::DisplayRole), "0.00%");
    ASSERT_EQ(item.toolTip(), "");
    ASSERT_EQ(item.foreground().color(), QColor("#97AEC4"));

    item.setValue(1, 100, 2);
    ASSERT_EQ(item.getValue(), 1);
    ASSERT_EQ(item.data(Qt::DisplayRole), "+1.00%");
    ASSERT_EQ(item.toolTip(), QString("From price: 100.00 ") + QChar(0x20BD));
    ASSERT_EQ(item.foreground().color(), QColor("#2BD793"));
}

TEST(Test_PriceChangeTableItem, Test_less)
{
    PriceChangeTableItem item;
    PriceChangeTableItem item2;

    item.setValue(10, 100, 2);
    item2.setValue(20, 100, 2);

    ASSERT_LT(item, item2);

    item.setValue(30, 100, 2);

    ASSERT_LT(item2, item);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
