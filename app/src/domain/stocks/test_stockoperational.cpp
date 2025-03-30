#include "src/domain/stocks/stockoperational.h"

#include <gtest/gtest.h>



TEST(Test_StockOperational, Test_constructor_and_destructor)
{
    StockOperational stockOperational;

    // clang-format off
    ASSERT_EQ(stockOperational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stockOperational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stockOperational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stockOperational.turnover,             0);
    ASSERT_NEAR(stockOperational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stockOperational.detailedData.size(),  0);
    // clang-format on
}

TEST(Test_StockOperational, Test_copy_constructor)
{
    StockOperational stockOperational;

    StockOperationalData stockData;
    stockData.timestamp = 6;
    stockData.price     = 7.0f;

    stockOperational.lastStoredTimestamp = 1;
    stockOperational.dayStartPrice       = 2.0f;
    stockOperational.specifiedDatePrice  = 3.0f;
    stockOperational.turnover            = 4;
    stockOperational.payback             = 5.0f;
    stockOperational.detailedData.append(stockData);

    StockOperational stockOperational2(stockOperational);

    // clang-format off
    ASSERT_EQ(stockOperational2.lastStoredTimestamp,          1);
    ASSERT_NEAR(stockOperational2.dayStartPrice,              2.0f, 0.0001f);
    ASSERT_NEAR(stockOperational2.specifiedDatePrice,         3.0f, 0.0001f);
    ASSERT_EQ(stockOperational2.turnover,                     4);
    ASSERT_NEAR(stockOperational2.payback,                    5.0f, 0.0001f);
    ASSERT_EQ(stockOperational2.detailedData.size(),          1);
    ASSERT_EQ(stockOperational2.detailedData.at(0).timestamp, 6);
    ASSERT_NEAR(stockOperational2.detailedData.at(0).price,   7.0f, 0.0001f);
    // clang-format on
}

TEST(Test_StockOperational, Test_assign)
{
    StockOperational stockOperational;
    StockOperational stockOperational2;

    StockOperationalData stockData;
    stockData.timestamp = 5;
    stockData.price     = 6.0f;

    stockOperational.lastStoredTimestamp = 1;
    stockOperational.dayStartPrice       = 2.0f;
    stockOperational.specifiedDatePrice  = 3.0f;
    stockOperational.turnover            = 4;
    stockOperational.payback             = 5.0f;
    stockOperational.detailedData.append(stockData);

    stockOperational2 = stockOperational;

    // clang-format off
    ASSERT_EQ(stockOperational2.lastStoredTimestamp,          1);
    ASSERT_NEAR(stockOperational2.dayStartPrice,              2.0f, 0.0001f);
    ASSERT_NEAR(stockOperational2.specifiedDatePrice,         3.0f, 0.0001f);
    ASSERT_EQ(stockOperational2.turnover,                     4);
    ASSERT_NEAR(stockOperational2.payback,                    5.0f, 0.0001f);
    ASSERT_EQ(stockOperational2.detailedData.size(),          1);
    ASSERT_EQ(stockOperational2.detailedData.at(0).timestamp, 6);
    ASSERT_NEAR(stockOperational2.detailedData.at(0).price,   7.0f, 0.0001f);
    // clang-format on
}
