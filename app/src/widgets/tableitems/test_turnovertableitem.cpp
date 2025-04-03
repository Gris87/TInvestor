#include "src/widgets/tableitems/turnovertableitem.h"

#include <gtest/gtest.h>



TEST(Test_TurnoverTableItem, Test_constructor_and_destructor)
{
    TurnoverTableItem item;
}

TEST(Test_TurnoverTableItem, Test_setValue_and_getValue)
{
    TurnoverTableItem item;

    item.setValue(1500000000);
    ASSERT_EQ(item.getValue(), 1500000000);
    ASSERT_EQ(item.data(Qt::DisplayRole), QString("1.50B ") + QChar(0x20BD));
    ASSERT_EQ(item.foreground().color(), QColor("#2BD793"));

    item.setValue(1500000);
    ASSERT_EQ(item.getValue(), 1500000);
    ASSERT_EQ(item.data(Qt::DisplayRole), QString("1.50M ") + QChar(0x20BD));
    ASSERT_EQ(item.foreground().color(), QColor("#97AEC4"));

    item.setValue(1500);
    ASSERT_EQ(item.getValue(), 1500);
    ASSERT_EQ(item.data(Qt::DisplayRole), QString("1.50K ") + QChar(0x20BD));
    ASSERT_EQ(item.foreground().color(), QColor("#ED6F7E"));
}

TEST(Test_TurnoverTableItem, Test_less)
{
    TurnoverTableItem item;
    TurnoverTableItem item2;

    item.setValue(10);
    item2.setValue(20);

    ASSERT_LT(item, item2);

    item.setValue(30);

    ASSERT_LT(item2, item);
}
