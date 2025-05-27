#include "src/domain/portfolio/portfolioitem.h"

#include <QDebug>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_PortfolioItem, Test_constructor_and_destructor)
{
    const PortfolioItem item;

    // clang-format off
    ASSERT_EQ(item.quantity,   0);
    ASSERT_NEAR(item.price,    0, 0.0001f);
    ASSERT_NEAR(item.avgPrice, 0, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioItem, Test_copy_constructor)
{
    PortfolioItem item;

    item.quantity = 1;
    item.price    = 2.0f;
    item.avgPrice = 3.0f;

    const PortfolioItem item2(item);

    // clang-format off
    ASSERT_EQ(item2.quantity,   1);
    ASSERT_NEAR(item2.price,    2, 0.0001f);
    ASSERT_NEAR(item2.avgPrice, 3, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioItem, Test_assign)
{
    PortfolioItem item;
    PortfolioItem item2;

    item.quantity = 1;
    item.price    = 2.0f;
    item.avgPrice = 3.0f;

    item2 = item;

    // clang-format off
    ASSERT_EQ(item2.quantity,   1);
    ASSERT_NEAR(item2.price,    2, 0.0001f);
    ASSERT_NEAR(item2.avgPrice, 3, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioItem, Test_equals)
{
    PortfolioItem item;
    PortfolioItem item2;

    item.quantity = 1;
    item.price    = 2.0f;
    item.avgPrice = 3.0f;

    item2.quantity = 1;
    item2.price    = 2.0f;
    item2.avgPrice = 3.0f;

    ASSERT_EQ(item, item2);

    item2.quantity = 1000;
    ASSERT_NE(item, item2);
    item2.quantity = 1;
    ASSERT_EQ(item, item2);

    item2.price = 2000.0f;
    ASSERT_NE(item, item2);
    item2.price = 2.0f;
    ASSERT_EQ(item, item2);

    item2.avgPrice = 3000.0f;
    ASSERT_NE(item, item2);
    item2.avgPrice = 3.0f;
    ASSERT_EQ(item, item2);
}
// NOLINTEND(readability-magic-numbers)
