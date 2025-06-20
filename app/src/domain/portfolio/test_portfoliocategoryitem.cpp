#include "src/domain/portfolio/portfoliocategoryitem.h"

#include <QDebug>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_PortfolioCategoryItem, Test_constructor_and_destructor)
{
    const PortfolioCategoryItem category;

    // clang-format off
    ASSERT_EQ(category.name,         "");
    ASSERT_NEAR(category.cost,       0, 0.0001);
    ASSERT_NEAR(category.part,       0, 0.0001f);
    ASSERT_EQ(category.items.size(), 0);
    // clang-format on
}

TEST(Test_PortfolioCategoryItem, Test_copy_constructor)
{
    PortfolioCategoryItem category;

    category.name = "a";
    category.cost = 1.0;
    category.part = 2.0f;

    PortfolioItem item;

    item.instrumentId       = "b";
    item.instrumentTicker   = "c";
    item.instrumentName     = "d";
    item.showPrices         = true;
    item.available          = 3.0;
    item.price              = 4.0f;
    item.avgPriceFifo       = 5.0f;
    item.avgPriceWavg       = 6.0f;
    item.cost               = 7.0;
    item.part               = 8.0f;
    item.yield              = 9.0f;
    item.yieldPercent       = 10.0f;
    item.dailyYield         = 11.0f;
    item.priceForDailyYield = 12.0f;
    item.costForDailyYield  = 13.0;
    item.dailyYieldPercent  = 14.0f;

    category.items.append(item);

    const PortfolioCategoryItem category2(category);

    // clang-format off
    ASSERT_EQ(category2.name,                             "a");
    ASSERT_NEAR(category2.cost,                           1.0, 0.0001);
    ASSERT_NEAR(category2.part,                           2.0, 0.0001f);
    ASSERT_EQ(category2.items.size(),                     1);
    ASSERT_EQ(category2.items.at(0).instrumentId,         "b");
    ASSERT_EQ(category2.items.at(0).instrumentLogo,       nullptr);
    ASSERT_EQ(category2.items.at(0).instrumentTicker,     "c");
    ASSERT_EQ(category2.items.at(0).instrumentName,       "d");
    ASSERT_EQ(category2.items.at(0).showPrices,           true);
    ASSERT_NEAR(category2.items.at(0).available,          3.0,   0.0001);
    ASSERT_NEAR(category2.items.at(0).price,              4.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).avgPriceFifo,       5.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).avgPriceWavg,       6.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).cost,               7.0,   0.0001);
    ASSERT_NEAR(category2.items.at(0).part,               8.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).yield,              9.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).yieldPercent,       10.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).dailyYield,         11.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).priceForDailyYield, 12.0f, 0.0001f);
    ASSERT_NEAR(category2.items.at(0).costForDailyYield,  13.0,  0.0001);
    ASSERT_NEAR(category2.items.at(0).dailyYieldPercent,  14.0f, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioCategoryItem, Test_assign)
{
    PortfolioCategoryItem category;
    PortfolioCategoryItem category2;

    category.name = "a";
    category.cost = 1.0;
    category.part = 2.0f;

    PortfolioItem item;

    item.instrumentId       = "b";
    item.instrumentTicker   = "c";
    item.instrumentName     = "d";
    item.showPrices         = true;
    item.available          = 3.0;
    item.price              = 4.0f;
    item.avgPriceFifo       = 5.0f;
    item.avgPriceWavg       = 6.0f;
    item.cost               = 7.0;
    item.part               = 8.0f;
    item.yield              = 9.0f;
    item.yieldPercent       = 10.0f;
    item.dailyYield         = 11.0f;
    item.priceForDailyYield = 12.0f;
    item.costForDailyYield  = 13.0;
    item.dailyYieldPercent  = 14.0f;

    category.items.append(item);

    category2 = category;

    // clang-format off
    ASSERT_EQ(category2.name,                             "a");
    ASSERT_NEAR(category2.cost,                           1.0, 0.0001);
    ASSERT_NEAR(category2.part,                           2.0, 0.0001f);
    ASSERT_EQ(category2.items.size(),                     1);
    ASSERT_EQ(category2.items.at(0).instrumentId,         "b");
    ASSERT_EQ(category2.items.at(0).instrumentLogo,       nullptr);
    ASSERT_EQ(category2.items.at(0).instrumentTicker,     "c");
    ASSERT_EQ(category2.items.at(0).instrumentName,       "d");
    ASSERT_EQ(category2.items.at(0).showPrices,           true);
    ASSERT_NEAR(category2.items.at(0).available,          3.0,   0.0001);
    ASSERT_NEAR(category2.items.at(0).price,              4.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).avgPriceFifo,       5.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).avgPriceWavg,       6.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).cost,               7.0,   0.0001);
    ASSERT_NEAR(category2.items.at(0).part,               8.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).yield,              9.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).yieldPercent,       10.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).dailyYield,         11.0f,  0.0001f);
    ASSERT_NEAR(category2.items.at(0).priceForDailyYield, 12.0f, 0.0001f);
    ASSERT_NEAR(category2.items.at(0).costForDailyYield,  13.0,  0.0001);
    ASSERT_NEAR(category2.items.at(0).dailyYieldPercent,  14.0f, 0.0001f);
    // clang-format on
}

TEST(Test_PortfolioCategoryItem, Test_equals)
{
    PortfolioCategoryItem category;
    PortfolioCategoryItem category2;

    category.name = "a";
    category.cost = 1.0;
    category.part = 2.0f;

    PortfolioItem item;

    item.instrumentId       = "b";
    item.instrumentTicker   = "c";
    item.instrumentName     = "d";
    item.showPrices         = true;
    item.available          = 3.0;
    item.price              = 4.0f;
    item.avgPriceFifo       = 5.0f;
    item.avgPriceWavg       = 6.0f;
    item.cost               = 7.0;
    item.part               = 8.0f;
    item.yield              = 9.0f;
    item.yieldPercent       = 10.0f;
    item.dailyYield         = 11.0f;
    item.priceForDailyYield = 12.0f;
    item.costForDailyYield  = 13.0;
    item.dailyYieldPercent  = 14.0f;

    category.items.append(item);

    category2.name = "a";
    category2.cost = 1.0;
    category2.part = 2.0f;

    PortfolioItem item2;

    item2.instrumentId       = "b";
    item2.instrumentTicker   = "c";
    item2.instrumentName     = "d";
    item2.showPrices         = true;
    item2.available          = 3.0;
    item2.price              = 4.0f;
    item2.avgPriceFifo       = 5.0f;
    item2.avgPriceWavg       = 6.0f;
    item2.cost               = 7.0;
    item2.part               = 8.0f;
    item2.yield              = 9.0f;
    item2.yieldPercent       = 10.0f;
    item2.dailyYield         = 11.0f;
    item2.priceForDailyYield = 12.0f;
    item2.costForDailyYield  = 13.0;
    item2.dailyYieldPercent  = 14.0f;

    category2.items.append(item2);

    ASSERT_EQ(category, category2);

    category2.name = "aaaa";
    ASSERT_NE(category, category2);
    category2.name = "a";
    ASSERT_EQ(category, category2);

    category2.cost = 1000.0;
    ASSERT_NE(category, category2);
    category2.cost = 1.0;
    ASSERT_EQ(category, category2);

    category2.part = 2000.0f;
    ASSERT_NE(category, category2);
    category2.part = 2.0f;
    ASSERT_EQ(category, category2);

    category2.items[0].instrumentId = "bbbb";
    ASSERT_NE(category, category2);
    category2.items[0].instrumentId = "b";
    ASSERT_EQ(category, category2);
}
// NOLINTEND(readability-magic-numbers)
