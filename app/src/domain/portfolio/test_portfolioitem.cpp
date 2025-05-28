#include "src/domain/portfolio/portfolioitem.h"

#include <QDebug>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_PortfolioItem, Test_constructor_and_destructor)
{
    const PortfolioItem item;

    // clang-format off
    ASSERT_EQ(item.showPrices,           false);
    ASSERT_NEAR(item.available,          0, 0.0001);
    ASSERT_NEAR(item.price,              0, 0.0001f);
    ASSERT_NEAR(item.avgPrice,           0, 0.0001f);
    ASSERT_NEAR(item.cost,               0, 0.0001);
    ASSERT_NEAR(item.part,               0, 0.0001f);
    ASSERT_NEAR(item.yield,              0, 0.0001f);
    ASSERT_NEAR(item.yieldPercent,       0, 0.0001f);
    ASSERT_NEAR(item.priceForDailyYield, 0, 0.0001f);
    ASSERT_NEAR(item.dailyYieldPercent,  0, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioItem, Test_copy_constructor)
{
    PortfolioItem item;

    item.showPrices         = true;
    item.available          = 1.0;
    item.price              = 2.0f;
    item.avgPrice           = 3.0f;
    item.cost               = 4.0;
    item.part               = 5.0f;
    item.yield              = 6.0f;
    item.yieldPercent       = 7.0f;
    item.priceForDailyYield = 8.0f;
    item.dailyYieldPercent  = 9.0f;

    const PortfolioItem item2(item);

    // clang-format off
    ASSERT_EQ(item2.showPrices,           true);
    ASSERT_NEAR(item2.available,          1.0,  0.0001);
    ASSERT_NEAR(item2.price,              2.0f, 0.0001f);
    ASSERT_NEAR(item2.avgPrice,           3.0f, 0.0001f);
    ASSERT_NEAR(item2.cost,               4.0,  0.0001);
    ASSERT_NEAR(item2.part,               5.0f, 0.0001f);
    ASSERT_NEAR(item2.yield,              6.0f, 0.0001f);
    ASSERT_NEAR(item2.yieldPercent,       7.0f, 0.0001f);
    ASSERT_NEAR(item2.priceForDailyYield, 8.0f, 0.0001f);
    ASSERT_NEAR(item2.dailyYieldPercent,  9.0f, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioItem, Test_assign)
{
    PortfolioItem item;
    PortfolioItem item2;

    item.showPrices         = true;
    item.available          = 1.0;
    item.price              = 2.0f;
    item.avgPrice           = 3.0f;
    item.cost               = 4.0;
    item.part               = 5.0f;
    item.yield              = 6.0f;
    item.yieldPercent       = 7.0f;
    item.priceForDailyYield = 8.0f;
    item.dailyYieldPercent  = 9.0f;

    item2 = item;

    // clang-format off
    ASSERT_EQ(item2.showPrices,           true);
    ASSERT_NEAR(item2.available,          1.0,  0.0001);
    ASSERT_NEAR(item2.price,              2.0f, 0.0001f);
    ASSERT_NEAR(item2.avgPrice,           3.0f, 0.0001f);
    ASSERT_NEAR(item2.cost,               4.0,  0.0001);
    ASSERT_NEAR(item2.part,               5.0f, 0.0001f);
    ASSERT_NEAR(item2.yield,              6.0f, 0.0001f);
    ASSERT_NEAR(item2.yieldPercent,       7.0f, 0.0001f);
    ASSERT_NEAR(item2.priceForDailyYield, 8.0f, 0.0001f);
    ASSERT_NEAR(item2.dailyYieldPercent,  9.0f, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioItem, Test_equals)
{
    PortfolioItem item;
    PortfolioItem item2;

    item.showPrices         = true;
    item.available          = 1.0;
    item.price              = 2.0f;
    item.avgPrice           = 3.0f;
    item.cost               = 4.0;
    item.part               = 5.0f;
    item.yield              = 6.0f;
    item.yieldPercent       = 7.0f;
    item.priceForDailyYield = 8.0f;
    item.dailyYieldPercent  = 9.0f;

    item2.showPrices         = true;
    item2.available          = 1.0;
    item2.price              = 2.0f;
    item2.avgPrice           = 3.0f;
    item2.cost               = 4.0;
    item2.part               = 5.0f;
    item2.yield              = 6.0f;
    item2.yieldPercent       = 7.0f;
    item2.priceForDailyYield = 8.0f;
    item2.dailyYieldPercent  = 9.0f;

    ASSERT_EQ(item, item2);

    item2.showPrices = false;
    ASSERT_NE(item, item2);
    item2.showPrices = true;
    ASSERT_EQ(item, item2);

    item2.available = 1000.0;
    ASSERT_NE(item, item2);
    item2.available = 1.0;
    ASSERT_EQ(item, item2);

    item2.price = 2000.0f;
    ASSERT_NE(item, item2);
    item2.price = 2.0f;
    ASSERT_EQ(item, item2);

    item2.avgPrice = 3000.0f;
    ASSERT_NE(item, item2);
    item2.avgPrice = 3.0f;
    ASSERT_EQ(item, item2);

    item2.cost = 4000.0;
    ASSERT_NE(item, item2);
    item2.cost = 4.0;
    ASSERT_EQ(item, item2);

    item2.part = 5000.0f;
    ASSERT_NE(item, item2);
    item2.part = 5.0f;
    ASSERT_EQ(item, item2);

    item2.yield = 6000.0f;
    ASSERT_NE(item, item2);
    item2.yield = 6.0f;
    ASSERT_EQ(item, item2);

    item2.yieldPercent = 7000.0f;
    ASSERT_NE(item, item2);
    item2.yieldPercent = 7.0f;
    ASSERT_EQ(item, item2);

    item2.priceForDailyYield = 8000.0f;
    ASSERT_NE(item, item2);
    item2.priceForDailyYield = 8.0f;
    ASSERT_EQ(item, item2);

    item2.dailyYieldPercent = 9000.0f;
    ASSERT_NE(item, item2);
    item2.dailyYieldPercent = 9.0f;
    ASSERT_EQ(item, item2);
}
// NOLINTEND(readability-magic-numbers)
