#include "src/domain/stock/stock.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_Stock, Test_constructor_and_destructor)
{
    const Stock stock;

    // clang-format off
    ASSERT_NE(stock.mutex,                            nullptr);
    ASSERT_EQ(stock.meta.instrumentId,                "");
    ASSERT_EQ(stock.meta.instrumentTicker,            "");
    ASSERT_EQ(stock.meta.instrumentName,              "");
    ASSERT_EQ(stock.meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stock.meta.pricePrecision,              0);
    ASSERT_EQ(stock.operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stock.operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stock.operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stock.operational.turnover,             0);
    ASSERT_NEAR(stock.operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stock.operational.detailedData.size(),  0);
    ASSERT_EQ(stock.data.size(),                      0);
    // clang-format on
}

TEST(Test_Stock, Test_copy_constructor)
{
    Stock stock;

    StockOperationalData stockData1;
    StockData            stockData2;

    stockData1.timestamp = 7;
    stockData1.price     = 8.0f;
    stockData2.timestamp = 9;
    stockData2.quantity  = 10;
    stockData2.price     = 11.0f;

    stock.meta.instrumentId               = "a";
    stock.meta.instrumentTicker           = "b";
    stock.meta.instrumentName             = "c";
    stock.meta.forQualInvestorFlag        = true;
    stock.meta.pricePrecision             = 1;
    stock.operational.lastStoredTimestamp = 2;
    stock.operational.dayStartPrice       = 3.0f;
    stock.operational.specifiedDatePrice  = 4.0f;
    stock.operational.turnover            = 5;
    stock.operational.payback             = 6.0f;
    stock.operational.detailedData.append(stockData1);
    stock.data.append(stockData2);

    const Stock stock2(stock);

    // clang-format off
    ASSERT_NE(stock2.mutex,                                    stock.mutex);
    ASSERT_NE(stock2.mutex,                                    nullptr);
    ASSERT_EQ(stock2.meta.instrumentId,                        "a");
    ASSERT_EQ(stock2.meta.instrumentTicker,                    "b");
    ASSERT_EQ(stock2.meta.instrumentName,                      "c");
    ASSERT_EQ(stock2.meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stock2.meta.pricePrecision,                      1);
    ASSERT_EQ(stock2.operational.lastStoredTimestamp,          2);
    ASSERT_NEAR(stock2.operational.dayStartPrice,              3.0f, 0.0001f);
    ASSERT_NEAR(stock2.operational.specifiedDatePrice,         4.0f, 0.0001f);
    ASSERT_EQ(stock2.operational.turnover,                     5);
    ASSERT_NEAR(stock2.operational.payback,                    6.0f, 0.0001f);
    ASSERT_EQ(stock2.operational.detailedData.size(),          1);
    ASSERT_EQ(stock2.operational.detailedData.at(0).timestamp, 7);
    ASSERT_NEAR(stock2.operational.detailedData.at(0).price,   8.0f, 0.0001f);
    ASSERT_EQ(stock2.data.size(),                              1);
    ASSERT_EQ(stock2.data.at(0).timestamp,                     9);
    ASSERT_EQ(stock2.data.at(0).quantity,                      10);
    ASSERT_NEAR(stock2.data.at(0).price,                       11.0f, 0.0001f);
    // clang-format on
}

TEST(Test_Stock, Test_assign)
{
    Stock stock;
    Stock stock2;

    StockOperationalData stockData1;
    StockData            stockData2;

    stockData1.timestamp = 7;
    stockData1.price     = 8.0f;
    stockData2.timestamp = 9;
    stockData2.quantity  = 10;
    stockData2.price     = 11.0f;

    stock.meta.instrumentId               = "a";
    stock.meta.instrumentTicker           = "b";
    stock.meta.instrumentName             = "c";
    stock.meta.forQualInvestorFlag        = true;
    stock.meta.pricePrecision             = 1;
    stock.operational.lastStoredTimestamp = 2;
    stock.operational.dayStartPrice       = 3.0f;
    stock.operational.specifiedDatePrice  = 4.0f;
    stock.operational.turnover            = 5;
    stock.operational.payback             = 6.0f;
    stock.operational.detailedData.append(stockData1);
    stock.data.append(stockData2);

    stock2 = stock;

    // clang-format off
    ASSERT_NE(stock2.mutex,                                    stock.mutex);
    ASSERT_NE(stock2.mutex,                                    nullptr);
    ASSERT_EQ(stock2.meta.instrumentId,                        "a");
    ASSERT_EQ(stock2.meta.instrumentTicker,                    "b");
    ASSERT_EQ(stock2.meta.instrumentName,                      "c");
    ASSERT_EQ(stock2.meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stock2.meta.pricePrecision,                      1);
    ASSERT_EQ(stock2.operational.lastStoredTimestamp,          2);
    ASSERT_NEAR(stock2.operational.dayStartPrice,              3.0f, 0.0001f);
    ASSERT_NEAR(stock2.operational.specifiedDatePrice,         4.0f, 0.0001f);
    ASSERT_EQ(stock2.operational.turnover,                     5);
    ASSERT_NEAR(stock2.operational.payback,                    6.0f, 0.0001f);
    ASSERT_EQ(stock2.operational.detailedData.size(),          1);
    ASSERT_EQ(stock2.operational.detailedData.at(0).timestamp, 7);
    ASSERT_NEAR(stock2.operational.detailedData.at(0).price,   8.0f, 0.0001f);
    ASSERT_EQ(stock2.data.size(),                              1);
    ASSERT_EQ(stock2.data.at(0).timestamp,                     9);
    ASSERT_EQ(stock2.data.at(0).quantity,                      10);
    ASSERT_NEAR(stock2.data.at(0).price,                       11.0f, 0.0001f);
    // clang-format on
}

TEST(Test_Stock, Test_lastPrice)
{
    Stock stock;

    ASSERT_NEAR(stock.lastPrice(), 0.0f, 0.0001f);

    StockData            stockData1;
    StockOperationalData stockData2;

    stockData1.timestamp = 1;
    stockData1.quantity  = 2;
    stockData1.price     = 3.0f;
    stockData2.timestamp = 4;
    stockData2.price     = 5.0f;

    stock.data.append(stockData1);
    ASSERT_NEAR(stock.lastPrice(), 3.0f, 0.0001f);

    stock.operational.detailedData.append(stockData2);
    ASSERT_NEAR(stock.lastPrice(), 5.0f, 0.0001f);
}

// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
