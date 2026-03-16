#include "src/domain/stock/stock.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
class Test_Stock : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_Stock, Test_constructor_and_destructor)
{
    const Stock stock;

    // clang-format off
    ASSERT_EQ(stock.meta.instrumentId,                "");
    ASSERT_EQ(stock.meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stock.meta.instrumentTicker,            "");
    ASSERT_EQ(stock.meta.instrumentName,              "");
    ASSERT_EQ(stock.meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stock.meta.lot,                         0);
    ASSERT_EQ(stock.meta.pricePrecision,              0);
    ASSERT_NEAR(stock.meta.minPriceIncrement,         0.0f, 0.0001f);
    ASSERT_EQ(stock.meta.ignore,                      false);
    ASSERT_EQ(stock.meta.lastTradeTime,               QTime(0, 0));
    ASSERT_EQ(stock.meta.turnover,                    0);
    ASSERT_EQ(stock.meta.dividends.createTimestamp,   0);
    ASSERT_EQ(stock.meta.dividends.paymentTimestamp,  0);
    ASSERT_NEAR(stock.meta.dividends.price,           0.0f, 0.0001f);
    ASSERT_NEAR(stock.meta.dividends.yield,           0.0f, 0.0001f);
    ASSERT_EQ(stock.meta.shorts.enabled,              false);
    ASSERT_EQ(stock.meta.shorts.lastEnabledTimestamp, 0);
    ASSERT_EQ(stock.operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stock.operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stock.operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stock.operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stock.operational.detailedData.size(),  0);
    ASSERT_EQ(stock.data.size(),                      0);
    // clang-format on
}

TEST_F(Test_Stock, Test_copy_constructor)
{
    Stock stock;

    StockOperationalData stockData1;
    StockData            stockData2;

    stockData1.timestamp = 16;
    stockData1.price     = 17.0f;
    stockData2.timestamp = 18;
    stockData2.quantity  = 19;
    stockData2.price     = 20.0f;

    stock.meta.instrumentId                = "a";
    stock.meta.instrumentTicker            = "b";
    stock.meta.instrumentName              = "c";
    stock.meta.forQualInvestorFlag         = true;
    stock.meta.lot                         = 1;
    stock.meta.pricePrecision              = 2;
    stock.meta.minPriceIncrement           = 3.0f;
    stock.meta.ignore                      = true;
    stock.meta.lastTradeTime               = QTime(4, 5);
    stock.meta.turnover                    = 6;
    stock.meta.dividends.createTimestamp   = 7;
    stock.meta.dividends.paymentTimestamp  = 8;
    stock.meta.dividends.price             = 9;
    stock.meta.dividends.yield             = 10;
    stock.meta.shorts.enabled              = true;
    stock.meta.shorts.lastEnabledTimestamp = 11;
    stock.operational.lastStoredTimestamp  = 12;
    stock.operational.dayStartPrice        = 13.0f;
    stock.operational.specifiedDatePrice   = 14.0f;
    stock.operational.payback              = 15.0f;
    stock.operational.detailedData << stockData1;
    stock.data << stockData2;

    const Stock stock2(stock);

    // clang-format off
    ASSERT_EQ(stock2.meta.instrumentId,                        "a");
    ASSERT_EQ(stock2.meta.instrumentLogo,                      nullptr);
    ASSERT_EQ(stock2.meta.instrumentTicker,                    "b");
    ASSERT_EQ(stock2.meta.instrumentName,                      "c");
    ASSERT_EQ(stock2.meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stock2.meta.lot,                                 1);
    ASSERT_EQ(stock2.meta.pricePrecision,                      2);
    ASSERT_NEAR(stock2.meta.minPriceIncrement,                 3.0f, 0.0001f);
    ASSERT_EQ(stock2.meta.ignore,                              true);
    ASSERT_EQ(stock2.meta.lastTradeTime,                       QTime(4, 5));
    ASSERT_EQ(stock2.meta.turnover,                            6);
    ASSERT_EQ(stock2.meta.dividends.createTimestamp,           7);
    ASSERT_EQ(stock2.meta.dividends.paymentTimestamp,          8);
    ASSERT_NEAR(stock2.meta.dividends.price,                   9.0f, 0.0001f);
    ASSERT_NEAR(stock2.meta.dividends.yield,                   10.0f, 0.0001f);
    ASSERT_EQ(stock2.meta.shorts.enabled,                      true);
    ASSERT_EQ(stock2.meta.shorts.lastEnabledTimestamp,         11);
    ASSERT_EQ(stock2.operational.lastStoredTimestamp,          12);
    ASSERT_NEAR(stock2.operational.dayStartPrice,              13.0f, 0.0001f);
    ASSERT_NEAR(stock2.operational.specifiedDatePrice,         14.0f, 0.0001f);
    ASSERT_NEAR(stock2.operational.payback,                    15.0f, 0.0001f);
    ASSERT_EQ(stock2.operational.detailedData.size(),          1);
    ASSERT_EQ(stock2.operational.detailedData.at(0).timestamp, 16);
    ASSERT_NEAR(stock2.operational.detailedData.at(0).price,   17.0f, 0.0001f);
    ASSERT_EQ(stock2.data.size(),                              1);
    ASSERT_EQ(stock2.data.at(0).timestamp,                     18);
    ASSERT_EQ(stock2.data.at(0).quantity,                      19);
    ASSERT_NEAR(stock2.data.at(0).price,                       20.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_Stock, Test_assign)
{
    Stock stock;
    Stock stock2;

    StockOperationalData stockData1;
    StockData            stockData2;

    stockData1.timestamp = 16;
    stockData1.price     = 17.0f;
    stockData2.timestamp = 18;
    stockData2.quantity  = 19;
    stockData2.price     = 20.0f;

    stock.meta.instrumentId                = "a";
    stock.meta.instrumentTicker            = "b";
    stock.meta.instrumentName              = "c";
    stock.meta.forQualInvestorFlag         = true;
    stock.meta.lot                         = 1;
    stock.meta.pricePrecision              = 2;
    stock.meta.minPriceIncrement           = 3.0f;
    stock.meta.ignore                      = true;
    stock.meta.lastTradeTime               = QTime(4, 5);
    stock.meta.turnover                    = 6;
    stock.meta.dividends.createTimestamp   = 7;
    stock.meta.dividends.paymentTimestamp  = 8;
    stock.meta.dividends.price             = 9;
    stock.meta.dividends.yield             = 10;
    stock.meta.shorts.enabled              = true;
    stock.meta.shorts.lastEnabledTimestamp = 11;
    stock.operational.lastStoredTimestamp  = 12;
    stock.operational.dayStartPrice        = 13.0f;
    stock.operational.specifiedDatePrice   = 14.0f;
    stock.operational.payback              = 15.0f;
    stock.operational.detailedData << stockData1;
    stock.data << stockData2;

    stock2 = stock;

    // clang-format off
    ASSERT_EQ(stock2.meta.instrumentId,                        "a");
    ASSERT_EQ(stock2.meta.instrumentLogo,                      nullptr);
    ASSERT_EQ(stock2.meta.instrumentTicker,                    "b");
    ASSERT_EQ(stock2.meta.instrumentName,                      "c");
    ASSERT_EQ(stock2.meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stock2.meta.lot,                                 1);
    ASSERT_EQ(stock2.meta.pricePrecision,                      2);
    ASSERT_NEAR(stock2.meta.minPriceIncrement,                 3.0f, 0.0001f);
    ASSERT_EQ(stock2.meta.ignore,                              true);
    ASSERT_EQ(stock2.meta.lastTradeTime,                       QTime(4, 5));
    ASSERT_EQ(stock2.meta.turnover,                            6);
    ASSERT_EQ(stock2.meta.dividends.createTimestamp,           7);
    ASSERT_EQ(stock2.meta.dividends.paymentTimestamp,          8);
    ASSERT_NEAR(stock2.meta.dividends.price,                   9.0f, 0.0001f);
    ASSERT_NEAR(stock2.meta.dividends.yield,                   10.0f, 0.0001f);
    ASSERT_EQ(stock2.meta.shorts.enabled,                      true);
    ASSERT_EQ(stock2.meta.shorts.lastEnabledTimestamp,         11);
    ASSERT_EQ(stock2.operational.lastStoredTimestamp,          12);
    ASSERT_NEAR(stock2.operational.dayStartPrice,              13.0f, 0.0001f);
    ASSERT_NEAR(stock2.operational.specifiedDatePrice,         14.0f, 0.0001f);
    ASSERT_NEAR(stock2.operational.payback,                    15.0f, 0.0001f);
    ASSERT_EQ(stock2.operational.detailedData.size(),          1);
    ASSERT_EQ(stock2.operational.detailedData.at(0).timestamp, 16);
    ASSERT_NEAR(stock2.operational.detailedData.at(0).price,   17.0f, 0.0001f);
    ASSERT_EQ(stock2.data.size(),                              1);
    ASSERT_EQ(stock2.data.at(0).timestamp,                     18);
    ASSERT_EQ(stock2.data.at(0).quantity,                      19);
    ASSERT_NEAR(stock2.data.at(0).price,                       20.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_Stock, Test_lock_and_unlock)
{
    Stock stock;

    stock.writeLock();
    stock.writeUnlock();
    stock.readLock();
    stock.readUnlock();
}

TEST_F(Test_Stock, Test_lastPrice)
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

    stock.data << stockData1;
    ASSERT_NEAR(stock.lastPrice(), 3.0f, 0.0001f);

    stock.operational.detailedData << stockData2;
    ASSERT_NEAR(stock.lastPrice(), 5.0f, 0.0001f);
}

TEST_F(Test_Stock, Test_lastOperationalPrice)
{
    Stock stock;

    ASSERT_NEAR(stock.lastOperationalPrice(), 0.0f, 0.0001f);

    StockOperationalData stockData;

    stockData.timestamp = 1;
    stockData.price     = 2.0f;

    stock.operational.detailedData << stockData;
    ASSERT_NEAR(stock.lastOperationalPrice(), 2.0f, 0.0001f);
}

// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
