#include "src/domain/portfolio/portfolio.h"

#include <QDebug>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_Portfolio, Test_constructor_and_destructor)
{
    const Portfolio portfolio;

    ASSERT_EQ(portfolio.positions.size(), 0);
}

TEST(Test_Portfolio, Test_copy_constructor)
{
    Portfolio portfolio;

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

    portfolio.positions["share"]["aaaaa"] = item;

    const Portfolio portfolio2(portfolio);

    // clang-format off
    ASSERT_EQ(portfolio2.positions.size(),                                 1);
    ASSERT_EQ(portfolio2.positions["share"].size(),                        1);
    ASSERT_EQ(portfolio2.positions["share"]["aaaaa"].showPrices,           true);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].available,          1.0,   0.0001);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].price,              2.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].avgPriceFifo,       3.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].avgPriceWavg,       4.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].cost,               5.0,   0.0001);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].part,               6.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].yield,              7.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].yieldPercent,       8.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].dailyYield,         9.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].priceForDailyYield, 10.0f, 0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].costForDailyYield,  11.0,  0.0001);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].dailyYieldPercent,  12.0f, 0.0001f);
    // clang-format on
}

TEST(Test_Portfolio, Test_assign)
{
    Portfolio portfolio;
    Portfolio portfolio2;

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

    portfolio.positions["share"]["aaaaa"] = item;

    portfolio2 = portfolio;

    // clang-format off
    ASSERT_EQ(portfolio2.positions.size(),                                 1);
    ASSERT_EQ(portfolio2.positions["share"].size(),                        1);
    ASSERT_EQ(portfolio2.positions["share"]["aaaaa"].showPrices,           true);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].available,          1.0,   0.0001);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].price,              2.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].avgPriceFifo,       3.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].avgPriceWavg,       4.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].cost,               5.0,   0.0001);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].part,               6.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].yield,              7.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].yieldPercent,       8.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].dailyYield,         9.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].priceForDailyYield, 10.0f, 0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].costForDailyYield,  11.0,  0.0001);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].dailyYieldPercent,  12.0f, 0.0001f);
    // clang-format on
}

TEST(Test_Portfolio, Test_equals)
{
    Portfolio portfolio;
    Portfolio portfolio2;

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

    portfolio.positions["share"]["aaaaa"]  = item;
    portfolio2.positions["share"]["aaaaa"] = item2;

    ASSERT_EQ(portfolio, portfolio2);

    portfolio2.positions["share"]["aaaaa"].showPrices = false;
    ASSERT_NE(portfolio, portfolio2);
    portfolio2.positions["share"]["aaaaa"].showPrices = true;
    ASSERT_EQ(portfolio, portfolio2);
}
// NOLINTEND(readability-magic-numbers)
