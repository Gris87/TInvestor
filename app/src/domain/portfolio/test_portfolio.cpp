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

    PortfolioCategoryItem category;

    category.id   = 1;
    category.name = "a";
    category.cost = 2.0;
    category.part = 3.0f;

    PortfolioItem item;

    item.instrumentId       = "b";
    item.instrumentTicker   = "c";
    item.instrumentName     = "d";
    item.showPrices         = true;
    item.available          = 4.0;
    item.price              = 5.0f;
    item.avgPriceFifo       = 6.0f;
    item.avgPriceWavg       = 7.0f;
    item.cost               = 8.0;
    item.part               = 9.0f;
    item.yield              = 10.0f;
    item.yieldPercent       = 11.0f;
    item.dailyYield         = 12.0f;
    item.priceForDailyYield = 13.0f;
    item.costForDailyYield  = 14.0;
    item.dailyYieldPercent  = 15.0f;
    item.pricePrecision     = 16;

    category.items.append(item);
    portfolio.positions.append(category);

    const Portfolio portfolio2(portfolio);

    // clang-format off
    ASSERT_EQ(portfolio2.positions.size(),                                 1);
    ASSERT_EQ(portfolio2.positions.at(0).id,                               1);
    ASSERT_EQ(portfolio2.positions.at(0).name,                             "a");
    ASSERT_NEAR(portfolio2.positions.at(0).cost,                           2.0, 0.0001);
    ASSERT_NEAR(portfolio2.positions.at(0).part,                           3.0, 0.0001f);
    ASSERT_EQ(portfolio2.positions.at(0).items.size(),                     1);
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).instrumentId,         "b");
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).instrumentLogo,       nullptr);
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).instrumentTicker,     "c");
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).instrumentName,       "d");
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).showPrices,           true);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).available,          4.0,   0.0001);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).price,              5.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).avgPriceFifo,       6.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).avgPriceWavg,       7.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).cost,               8.0,   0.0001);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).part,               9.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).yield,              10.0f, 0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).yieldPercent,       11.0f, 0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).dailyYield,         12.0f, 0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).priceForDailyYield, 13.0f, 0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).costForDailyYield,  14.0,  0.0001);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).dailyYieldPercent,  15.0f, 0.0001f);
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).pricePrecision,       16);
    // clang-format on
}

TEST(Test_Portfolio, Test_assign)
{
    Portfolio portfolio;
    Portfolio portfolio2;

    PortfolioCategoryItem category;

    category.id   = 1;
    category.name = "a";
    category.cost = 2.0;
    category.part = 3.0f;

    PortfolioItem item;

    item.instrumentId       = "b";
    item.instrumentTicker   = "c";
    item.instrumentName     = "d";
    item.showPrices         = true;
    item.available          = 4.0;
    item.price              = 5.0f;
    item.avgPriceFifo       = 6.0f;
    item.avgPriceWavg       = 7.0f;
    item.cost               = 8.0;
    item.part               = 9.0f;
    item.yield              = 10.0f;
    item.yieldPercent       = 11.0f;
    item.dailyYield         = 12.0f;
    item.priceForDailyYield = 13.0f;
    item.costForDailyYield  = 14.0;
    item.dailyYieldPercent  = 15.0f;
    item.pricePrecision     = 16;

    category.items.append(item);
    portfolio.positions.append(category);

    portfolio2 = portfolio;

    // clang-format off
    ASSERT_EQ(portfolio2.positions.size(),                                 1);
    ASSERT_EQ(portfolio2.positions.at(0).id,                               1);
    ASSERT_EQ(portfolio2.positions.at(0).name,                             "a");
    ASSERT_NEAR(portfolio2.positions.at(0).cost,                           2.0, 0.0001);
    ASSERT_NEAR(portfolio2.positions.at(0).part,                           3.0, 0.0001f);
    ASSERT_EQ(portfolio2.positions.at(0).items.size(),                     1);
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).instrumentId,         "b");
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).instrumentLogo,       nullptr);
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).instrumentTicker,     "c");
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).instrumentName,       "d");
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).showPrices,           true);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).available,          4.0,   0.0001);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).price,              5.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).avgPriceFifo,       6.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).avgPriceWavg,       7.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).cost,               8.0,   0.0001);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).part,               9.0f,  0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).yield,              10.0f, 0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).yieldPercent,       11.0f, 0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).dailyYield,         12.0f, 0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).priceForDailyYield, 13.0f, 0.0001f);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).costForDailyYield,  14.0,  0.0001);
    ASSERT_NEAR(portfolio2.positions.at(0).items.at(0).dailyYieldPercent,  15.0f, 0.0001f);
    ASSERT_EQ(portfolio2.positions.at(0).items.at(0).pricePrecision,       16);
    // clang-format on
}

TEST(Test_Portfolio, Test_equals)
{
    Portfolio portfolio;
    Portfolio portfolio2;

    PortfolioCategoryItem category;

    category.id   = 1;
    category.name = "a";
    category.cost = 2.0;
    category.part = 3.0f;

    PortfolioItem item;

    item.instrumentId       = "b";
    item.instrumentTicker   = "c";
    item.instrumentName     = "d";
    item.showPrices         = true;
    item.available          = 4.0;
    item.price              = 5.0f;
    item.avgPriceFifo       = 6.0f;
    item.avgPriceWavg       = 7.0f;
    item.cost               = 8.0;
    item.part               = 9.0f;
    item.yield              = 10.0f;
    item.yieldPercent       = 11.0f;
    item.dailyYield         = 12.0f;
    item.priceForDailyYield = 13.0f;
    item.costForDailyYield  = 14.0;
    item.dailyYieldPercent  = 15.0f;
    item.pricePrecision     = 16;

    category.items.append(item);
    portfolio.positions.append(category);

    PortfolioCategoryItem category2;

    category2.id   = 1;
    category2.name = "a";
    category2.cost = 2.0;
    category2.part = 3.0f;

    PortfolioItem item2;

    item2.instrumentId       = "b";
    item2.instrumentTicker   = "c";
    item2.instrumentName     = "d";
    item2.showPrices         = true;
    item2.available          = 4.0;
    item2.price              = 5.0f;
    item2.avgPriceFifo       = 6.0f;
    item2.avgPriceWavg       = 7.0f;
    item2.cost               = 8.0;
    item2.part               = 9.0f;
    item2.yield              = 10.0f;
    item2.yieldPercent       = 11.0f;
    item2.dailyYield         = 12.0f;
    item2.priceForDailyYield = 13.0f;
    item2.costForDailyYield  = 14.0;
    item2.dailyYieldPercent  = 15.0f;
    item2.pricePrecision     = 16;

    category2.items.append(item2);
    portfolio2.positions.append(category2);

    ASSERT_EQ(portfolio, portfolio2);

    portfolio2.positions[0].id = 1000;
    ASSERT_NE(portfolio, portfolio2);
    portfolio2.positions[0].id = 1;
    ASSERT_EQ(portfolio, portfolio2);
}
// NOLINTEND(readability-magic-numbers)
