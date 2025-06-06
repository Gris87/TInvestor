#include "src/domain/portfolio/portfoliominitem.h"

#include <QDebug>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_PortfolioMinItem, Test_constructor_and_destructor)
{
    const PortfolioMinItem item;

    // clang-format off
    ASSERT_NEAR(item.price, 0, 0.0001f);
    ASSERT_NEAR(item.cost,  0, 0.0001);
    // clang-format on
}

TEST(Test_PortfolioMinItem, Test_copy_constructor)
{
    PortfolioMinItem item;

    item.price = 1.0f;
    item.cost  = 2.0;

    const PortfolioMinItem item2(item);

    // clang-format off
    ASSERT_NEAR(item2.price, 1.0f, 0.0001f);
    ASSERT_NEAR(item2.cost,  2.0,  0.0001);
    // clang-format on
}

TEST(Test_PortfolioMinItem, Test_assign)
{
    PortfolioMinItem item;
    PortfolioMinItem item2;

    item.price = 1.0f;
    item.cost  = 2.0;

    item2 = item;

    // clang-format off
    ASSERT_NEAR(item2.price, 1.0f, 0.0001f);
    ASSERT_NEAR(item2.cost,  2.0,  0.0001);
    // clang-format on
}

TEST(Test_PortfolioMinItem, Test_equals)
{
    PortfolioMinItem item;
    PortfolioMinItem item2;

    item.price = 1.0f;
    item.cost  = 2.0;

    item2.price = 1.0f;
    item2.cost  = 2.0;

    ASSERT_EQ(item, item2);

    item2.price = 1000.0f;
    ASSERT_NE(item, item2);
    item2.price = 1.0f;
    ASSERT_EQ(item, item2);

    item2.cost = 2000.0;
    ASSERT_NE(item, item2);
    item2.cost = 2.0;
    ASSERT_EQ(item, item2);
}
// NOLINTEND(readability-magic-numbers)
