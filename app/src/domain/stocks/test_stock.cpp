#include "src/domain/stocks/stock.h"

#include <gtest/gtest.h>



TEST(Test_Stock, Test_constructor_and_destructor)
{
    Stock stock;

    // clang-format off
    ASSERT_NE(stock.mutex,                            nullptr);
    ASSERT_EQ(stock.meta.uid,                         "");
    ASSERT_EQ(stock.meta.ticker,                      "");
    ASSERT_EQ(stock.meta.name,                        "");
    ASSERT_EQ(stock.meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stock.meta.lot,                         0);
    ASSERT_EQ(stock.meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stock.meta.minPriceIncrement.nano,      0);
    ASSERT_EQ(stock.operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stock.operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stock.operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stock.operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stock.operational.detailedData.size(),  0);
    ASSERT_EQ(stock.data.size(),                      0);
    // clang-format on
}

TEST(Test_Stock, Test_copy_constructor)
{
    Stock stock;

    StockData stockData1;
    StockData stockData2;

    stockData1.timestamp = 8;
    stockData1.price     = 9.0f;
    stockData2.timestamp = 10;
    stockData2.price     = 11.0f;

    stock.meta.uid                        = "a";
    stock.meta.ticker                     = "b";
    stock.meta.name                       = "c";
    stock.meta.forQualInvestorFlag        = true;
    stock.meta.lot                        = 1;
    stock.meta.minPriceIncrement.units    = 2;
    stock.meta.minPriceIncrement.nano     = 3;
    stock.operational.lastStoredTimestamp = 4;
    stock.operational.dayStartPrice       = 5.0f;
    stock.operational.specifiedDatePrice  = 6.0f;
    stock.operational.payback             = 7.0f;
    stock.operational.detailedData.append(stockData1);
    stock.data.append(stockData2);

    Stock stock2(stock);

    // clang-format off
    ASSERT_NE(stock2.mutex,                                    stock.mutex);
    ASSERT_NE(stock2.mutex,                                    nullptr);
    ASSERT_EQ(stock2.meta.uid,                                 "a");
    ASSERT_EQ(stock2.meta.ticker,                              "b");
    ASSERT_EQ(stock2.meta.name,                                "c");
    ASSERT_EQ(stock2.meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stock2.meta.lot,                                 1);
    ASSERT_EQ(stock2.meta.minPriceIncrement.units,             2);
    ASSERT_EQ(stock2.meta.minPriceIncrement.nano,              3);
    ASSERT_EQ(stock2.operational.lastStoredTimestamp,          4);
    ASSERT_NEAR(stock2.operational.dayStartPrice,              5.0f, 0.0001f);
    ASSERT_NEAR(stock2.operational.specifiedDatePrice,         6.0f, 0.0001f);
    ASSERT_NEAR(stock2.operational.payback,                    7.0f, 0.0001f);
    ASSERT_EQ(stock2.operational.detailedData.size(),          1);
    ASSERT_EQ(stock2.operational.detailedData.at(0).timestamp, 8);
    ASSERT_NEAR(stock2.operational.detailedData.at(0).price,   9.0f, 0.0001f);
    ASSERT_EQ(stock2.data.size(),                              1);
    ASSERT_EQ(stock2.data.at(0).timestamp,                     10);
    ASSERT_NEAR(stock2.data.at(0).price,                       11.0f, 0.0001f);
    // clang-format on
}

TEST(Test_Stock, Test_assign)
{
    Stock stock;
    Stock stock2;

    StockData stockData1;
    StockData stockData2;

    stockData1.timestamp = 8;
    stockData1.price     = 9.0f;
    stockData2.timestamp = 10;
    stockData2.price     = 11.0f;

    stock.meta.uid                        = "a";
    stock.meta.ticker                     = "b";
    stock.meta.name                       = "c";
    stock.meta.forQualInvestorFlag        = true;
    stock.meta.lot                        = 1;
    stock.meta.minPriceIncrement.units    = 2;
    stock.meta.minPriceIncrement.nano     = 3;
    stock.operational.lastStoredTimestamp = 4;
    stock.operational.dayStartPrice       = 5.0f;
    stock.operational.specifiedDatePrice  = 6.0f;
    stock.operational.payback             = 7.0f;
    stock.operational.detailedData.append(stockData1);
    stock.data.append(stockData2);

    stock2 = stock;

    // clang-format off
    ASSERT_NE(stock2.mutex,                                    stock.mutex);
    ASSERT_NE(stock2.mutex,                                    nullptr);
    ASSERT_EQ(stock2.meta.uid,                                 "a");
    ASSERT_EQ(stock2.meta.ticker,                              "b");
    ASSERT_EQ(stock2.meta.name,                                "c");
    ASSERT_EQ(stock2.meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stock2.meta.lot,                                 1);
    ASSERT_EQ(stock2.meta.minPriceIncrement.units,             2);
    ASSERT_EQ(stock2.meta.minPriceIncrement.nano,              3);
    ASSERT_EQ(stock2.operational.lastStoredTimestamp,          4);
    ASSERT_NEAR(stock2.operational.dayStartPrice,              5.0f, 0.0001f);
    ASSERT_NEAR(stock2.operational.specifiedDatePrice,         6.0f, 0.0001f);
    ASSERT_NEAR(stock2.operational.payback,                    7.0f, 0.0001f);
    ASSERT_EQ(stock2.operational.detailedData.size(),          1);
    ASSERT_EQ(stock2.operational.detailedData.at(0).timestamp, 8);
    ASSERT_NEAR(stock2.operational.detailedData.at(0).price,   9.0f, 0.0001f);
    ASSERT_EQ(stock2.data.size(),                              1);
    ASSERT_EQ(stock2.data.at(0).timestamp,                     10);
    ASSERT_NEAR(stock2.data.at(0).price,                       11.0f, 0.0001f);
    // clang-format on
}
