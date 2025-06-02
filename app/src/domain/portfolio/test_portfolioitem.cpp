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
    ASSERT_NEAR(item.avgPriceFifo,       0, 0.0001f);
    ASSERT_NEAR(item.avgPriceWavg,       0, 0.0001f);
    ASSERT_NEAR(item.cost,               0, 0.0001);
    ASSERT_NEAR(item.part,               0, 0.0001f);
    ASSERT_NEAR(item.yield,              0, 0.0001f);
    ASSERT_NEAR(item.yieldPercent,       0, 0.0001f);
    ASSERT_NEAR(item.dailyYield,         0, 0.0001f);
    ASSERT_NEAR(item.priceForDailyYield, 0, 0.0001f);
    ASSERT_NEAR(item.costForDailyYield,  0, 0.0001);
    ASSERT_NEAR(item.dailyYieldPercent,  0, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioItem, Test_copy_constructor)
{
    PortfolioItem item;

    item.showPrices         = true;
    item.available          = 1.0;
    item.price              = 2.0f;
    item.avgPriceFifo       = 3.0f;
    item.avgPriceWavg       = 4.0f;
    item.cost               = 5.0;
    item.part               = 6.0f;
    item.yield              = 7.0f;
    item.yieldPercent       = 8.0f;
    item.dailyYield         = 9.0f;
    item.priceForDailyYield = 10.0f;
    item.costForDailyYield  = 11.0;
    item.dailyYieldPercent  = 12.0f;

    const PortfolioItem item2(item);

    // clang-format off
    ASSERT_EQ(item2.showPrices,           true);
    ASSERT_NEAR(item2.available,          1.0,   0.0001);
    ASSERT_NEAR(item2.price,              2.0f,  0.0001f);
    ASSERT_NEAR(item2.avgPriceFifo,       3.0f,  0.0001f);
    ASSERT_NEAR(item2.avgPriceWavg,       4.0f,  0.0001f);
    ASSERT_NEAR(item2.cost,               5.0,   0.0001);
    ASSERT_NEAR(item2.part,               6.0f,  0.0001f);
    ASSERT_NEAR(item2.yield,              7.0f,  0.0001f);
    ASSERT_NEAR(item2.yieldPercent,       8.0f,  0.0001f);
    ASSERT_NEAR(item2.dailyYield,         9.0f,  0.0001f);
    ASSERT_NEAR(item2.priceForDailyYield, 10.0f, 0.0001f);
    ASSERT_NEAR(item2.costForDailyYield,  11.0,  0.0001);
    ASSERT_NEAR(item2.dailyYieldPercent,  12.0f, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioItem, Test_assign)
{
    PortfolioItem item;
    PortfolioItem item2;

    item.showPrices         = true;
    item.available          = 1.0;
    item.price              = 2.0f;
    item.avgPriceFifo       = 3.0f;
    item.avgPriceWavg       = 4.0f;
    item.cost               = 5.0;
    item.part               = 6.0f;
    item.yield              = 7.0f;
    item.yieldPercent       = 8.0f;
    item.dailyYield         = 9.0f;
    item.priceForDailyYield = 10.0f;
    item.costForDailyYield  = 11.0;
    item.dailyYieldPercent  = 12.0f;

    item2 = item;

    // clang-format off
    ASSERT_EQ(item2.showPrices,           true);
    ASSERT_NEAR(item2.available,          1.0,   0.0001);
    ASSERT_NEAR(item2.price,              2.0f,  0.0001f);
    ASSERT_NEAR(item2.avgPriceFifo,       3.0f,  0.0001f);
    ASSERT_NEAR(item2.avgPriceWavg,       4.0f,  0.0001f);
    ASSERT_NEAR(item2.cost,               5.0,   0.0001);
    ASSERT_NEAR(item2.part,               6.0f,  0.0001f);
    ASSERT_NEAR(item2.yield,              7.0f,  0.0001f);
    ASSERT_NEAR(item2.yieldPercent,       8.0f,  0.0001f);
    ASSERT_NEAR(item2.dailyYield,         9.0f,  0.0001f);
    ASSERT_NEAR(item2.priceForDailyYield, 10.0f, 0.0001f);
    ASSERT_NEAR(item2.costForDailyYield,  11.0,  0.0001);
    ASSERT_NEAR(item2.dailyYieldPercent,  12.0f, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioItem, Test_equals)
{
    PortfolioItem item;
    PortfolioItem item2;

    item.showPrices         = true;
    item.available          = 1.0;
    item.price              = 2.0f;
    item.avgPriceFifo       = 3.0f;
    item.avgPriceWavg       = 4.0f;
    item.cost               = 5.0;
    item.part               = 6.0f;
    item.yield              = 7.0f;
    item.yieldPercent       = 8.0f;
    item.dailyYield         = 9.0f;
    item.priceForDailyYield = 10.0f;
    item.costForDailyYield  = 11.0;
    item.dailyYieldPercent  = 12.0f;

    item2.showPrices         = true;
    item2.available          = 1.0;
    item2.price              = 2.0f;
    item2.avgPriceFifo       = 3.0f;
    item2.avgPriceWavg       = 4.0f;
    item2.cost               = 5.0;
    item2.part               = 6.0f;
    item2.yield              = 7.0f;
    item2.yieldPercent       = 8.0f;
    item2.dailyYield         = 9.0f;
    item2.priceForDailyYield = 10.0f;
    item2.costForDailyYield  = 11.0;
    item2.dailyYieldPercent  = 12.0f;

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

    item2.avgPriceFifo = 3000.0f;
    ASSERT_NE(item, item2);
    item2.avgPriceFifo = 3.0f;
    ASSERT_EQ(item, item2);

    item2.avgPriceWavg = 4000.0f;
    ASSERT_NE(item, item2);
    item2.avgPriceWavg = 4.0f;
    ASSERT_EQ(item, item2);

    item2.cost = 5000.0;
    ASSERT_NE(item, item2);
    item2.cost = 5.0;
    ASSERT_EQ(item, item2);

    item2.part = 6000.0f;
    ASSERT_NE(item, item2);
    item2.part = 6.0f;
    ASSERT_EQ(item, item2);

    item2.yield = 7000.0f;
    ASSERT_NE(item, item2);
    item2.yield = 7.0f;
    ASSERT_EQ(item, item2);

    item2.yieldPercent = 8000.0f;
    ASSERT_NE(item, item2);
    item2.yieldPercent = 8.0f;
    ASSERT_EQ(item, item2);

    item2.dailyYield = 9000.0f;
    ASSERT_NE(item, item2);
    item2.dailyYield = 9.0f;
    ASSERT_EQ(item, item2);

    item2.priceForDailyYield = 10000.0f;
    ASSERT_NE(item, item2);
    item2.priceForDailyYield = 10.0f;
    ASSERT_EQ(item, item2);

    item2.costForDailyYield = 11000.0;
    ASSERT_NE(item, item2);
    item2.costForDailyYield = 11.0;
    ASSERT_EQ(item, item2);

    item2.dailyYieldPercent = 12000.0f;
    ASSERT_NE(item, item2);
    item2.dailyYieldPercent = 12.0f;
    ASSERT_EQ(item, item2);
}
// NOLINTEND(readability-magic-numbers)
