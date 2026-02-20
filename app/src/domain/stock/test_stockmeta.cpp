#include "src/domain/stock/stockmeta.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_StockMeta : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_StockMeta, Test_constructor_and_destructor)
{
    const StockMeta stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta.instrumentId,                "");
    ASSERT_EQ(stockMeta.instrumentLogo,              nullptr);
    ASSERT_EQ(stockMeta.instrumentTicker,            "");
    ASSERT_EQ(stockMeta.instrumentName,              "");
    ASSERT_EQ(stockMeta.forQualInvestorFlag,         false);
    ASSERT_EQ(stockMeta.lot,                         0);
    ASSERT_EQ(stockMeta.pricePrecision,              0);
    ASSERT_NEAR(stockMeta.minPriceIncrement,         0.0f, 0.0001f);
    ASSERT_EQ(stockMeta.ignore,                      false);
    ASSERT_EQ(stockMeta.lastTradeTime,               QTime(0, 0));
    ASSERT_EQ(stockMeta.turnover,                    0);
    ASSERT_EQ(stockMeta.dividends.createTimestamp,   0);
    ASSERT_EQ(stockMeta.dividends.paymentTimestamp,  0);
    ASSERT_EQ(stockMeta.dividends.yield,             0);
    ASSERT_EQ(stockMeta.shorts.enabled,              0);
    ASSERT_EQ(stockMeta.shorts.lastEnabledTimestamp, 0);
    // clang-format on
}

TEST_F(Test_StockMeta, Test_copy_constructor)
{
    StockMeta stockMeta;

    stockMeta.instrumentId                = "a";
    stockMeta.instrumentTicker            = "b";
    stockMeta.instrumentName              = "c";
    stockMeta.forQualInvestorFlag         = true;
    stockMeta.lot                         = 1;
    stockMeta.pricePrecision              = 2;
    stockMeta.minPriceIncrement           = 3.0f;
    stockMeta.ignore                      = true;
    stockMeta.lastTradeTime               = QTime(4, 5);
    stockMeta.turnover                    = 6;
    stockMeta.dividends.createTimestamp   = 7;
    stockMeta.dividends.paymentTimestamp  = 8;
    stockMeta.dividends.yield             = 9.0f;
    stockMeta.shorts.enabled              = true;
    stockMeta.shorts.lastEnabledTimestamp = 10;

    const StockMeta stockMeta2(stockMeta);

    // clang-format off
    ASSERT_EQ(stockMeta2.instrumentId,                "a");
    ASSERT_EQ(stockMeta2.instrumentLogo,              nullptr);
    ASSERT_EQ(stockMeta2.instrumentTicker,            "b");
    ASSERT_EQ(stockMeta2.instrumentName,              "c");
    ASSERT_EQ(stockMeta2.forQualInvestorFlag,         true);
    ASSERT_EQ(stockMeta2.lot,                         1);
    ASSERT_EQ(stockMeta2.pricePrecision,              2);
    ASSERT_NEAR(stockMeta2.minPriceIncrement,         3.0f, 0.0001f);
    ASSERT_EQ(stockMeta2.ignore,                      true);
    ASSERT_EQ(stockMeta2.lastTradeTime,               QTime(4, 5));
    ASSERT_EQ(stockMeta2.turnover,                    6);
    ASSERT_EQ(stockMeta2.dividends.createTimestamp,   7);
    ASSERT_EQ(stockMeta2.dividends.paymentTimestamp,  8);
    ASSERT_NEAR(stockMeta2.dividends.yield,           9.0f, 0.0001f);
    ASSERT_EQ(stockMeta2.shorts.enabled,              true);
    ASSERT_EQ(stockMeta2.shorts.lastEnabledTimestamp, 10);
    // clang-format on
}

TEST_F(Test_StockMeta, Test_assign)
{
    StockMeta stockMeta;
    StockMeta stockMeta2;

    stockMeta.instrumentId                = "a";
    stockMeta.instrumentTicker            = "b";
    stockMeta.instrumentName              = "c";
    stockMeta.forQualInvestorFlag         = true;
    stockMeta.lot                         = 1;
    stockMeta.pricePrecision              = 2;
    stockMeta.minPriceIncrement           = 3.0f;
    stockMeta.ignore                      = true;
    stockMeta.lastTradeTime               = QTime(4, 5);
    stockMeta.turnover                    = 6;
    stockMeta.dividends.createTimestamp   = 7;
    stockMeta.dividends.paymentTimestamp  = 8;
    stockMeta.dividends.yield             = 9.0f;
    stockMeta.shorts.enabled              = true;
    stockMeta.shorts.lastEnabledTimestamp = 10;

    stockMeta2 = stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta2.instrumentId,                "a");
    ASSERT_EQ(stockMeta2.instrumentLogo,              nullptr);
    ASSERT_EQ(stockMeta2.instrumentTicker,            "b");
    ASSERT_EQ(stockMeta2.instrumentName,              "c");
    ASSERT_EQ(stockMeta2.forQualInvestorFlag,         true);
    ASSERT_EQ(stockMeta2.lot,                         1);
    ASSERT_EQ(stockMeta2.pricePrecision,              2);
    ASSERT_NEAR(stockMeta2.minPriceIncrement,         3.0f, 0.0001f);
    ASSERT_EQ(stockMeta2.ignore,                      true);
    ASSERT_EQ(stockMeta2.lastTradeTime,               QTime(4, 5));
    ASSERT_EQ(stockMeta2.turnover,                    6);
    ASSERT_EQ(stockMeta2.dividends.createTimestamp,   7);
    ASSERT_EQ(stockMeta2.dividends.paymentTimestamp,  8);
    ASSERT_NEAR(stockMeta2.dividends.yield,           9.0f, 0.0001f);
    ASSERT_EQ(stockMeta2.shorts.enabled,              true);
    ASSERT_EQ(stockMeta2.shorts.lastEnabledTimestamp, 10);
    // clang-format on
}

TEST_F(Test_StockMeta, Test_fromJsonObject)
{
    StockMeta stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta.instrumentId,                "");
    ASSERT_EQ(stockMeta.instrumentLogo,              nullptr);
    ASSERT_EQ(stockMeta.instrumentTicker,            "");
    ASSERT_EQ(stockMeta.instrumentName,              "");
    ASSERT_EQ(stockMeta.forQualInvestorFlag,         false);
    ASSERT_EQ(stockMeta.lot,                         0);
    ASSERT_EQ(stockMeta.pricePrecision,              0);
    ASSERT_NEAR(stockMeta.minPriceIncrement,         0.0f, 0.0001f);
    ASSERT_EQ(stockMeta.ignore,                      false);
    ASSERT_EQ(stockMeta.lastTradeTime,               QTime(0, 0));
    ASSERT_EQ(stockMeta.turnover,                    0);
    ASSERT_EQ(stockMeta.dividends.createTimestamp,   0);
    ASSERT_EQ(stockMeta.dividends.paymentTimestamp,  0);
    ASSERT_EQ(stockMeta.dividends.yield,             0);
    ASSERT_EQ(stockMeta.shorts.enabled,              0);
    ASSERT_EQ(stockMeta.shorts.lastEnabledTimestamp, 0);
    // clang-format on

    const QString content =
        R"({"dividends":{"createTimestamp":7,"paymentTimestamp":8,"yield":"9.00"},"forQualInvestorFlag":true,"ignore":true,"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","lastTradeTime":"04:05:00","lot":1,"minPriceIncrement":"3.00","pricePrecision":2,"shorts":{"enabled":true,"lastEnabledTimestamp":10},"turnover":6})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    stockMeta.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(stockMeta.instrumentId,                "a");
    ASSERT_EQ(stockMeta.instrumentLogo,              nullptr);
    ASSERT_EQ(stockMeta.instrumentTicker,            "b");
    ASSERT_EQ(stockMeta.instrumentName,              "c");
    ASSERT_EQ(stockMeta.forQualInvestorFlag,         true);
    ASSERT_EQ(stockMeta.lot,                         1);
    ASSERT_EQ(stockMeta.pricePrecision,              2);
    ASSERT_NEAR(stockMeta.minPriceIncrement,         3.0f, 0.0001f);
    ASSERT_EQ(stockMeta.ignore,                      true);
    ASSERT_EQ(stockMeta.lastTradeTime,               QTime(4, 5));
    ASSERT_EQ(stockMeta.turnover,                    6);
    ASSERT_EQ(stockMeta.dividends.createTimestamp,   7);
    ASSERT_EQ(stockMeta.dividends.paymentTimestamp,  8);
    ASSERT_NEAR(stockMeta.dividends.yield,           9.0f, 0.0001f);
    ASSERT_EQ(stockMeta.shorts.enabled,              true);
    ASSERT_EQ(stockMeta.shorts.lastEnabledTimestamp, 10);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    stockMeta.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_StockMeta, Test_toJsonObject)
{
    StockMeta stockMeta;

    stockMeta.instrumentId                = "a";
    stockMeta.instrumentTicker            = "b";
    stockMeta.instrumentName              = "c";
    stockMeta.forQualInvestorFlag         = true;
    stockMeta.lot                         = 1;
    stockMeta.pricePrecision              = 2;
    stockMeta.minPriceIncrement           = 3.0f;
    stockMeta.ignore                      = true;
    stockMeta.lastTradeTime               = QTime(4, 5);
    stockMeta.turnover                    = 6;
    stockMeta.dividends.createTimestamp   = 7;
    stockMeta.dividends.paymentTimestamp  = 8;
    stockMeta.dividends.yield             = 9.0f;
    stockMeta.shorts.enabled              = true;
    stockMeta.shorts.lastEnabledTimestamp = 10;

    const QJsonObject   jsonObject = stockMeta.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"dividends":{"createTimestamp":7,"paymentTimestamp":8,"yield":"9.00"},"forQualInvestorFlag":true,"ignore":true,"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","lastTradeTime":"04:05:00","lot":1,"minPriceIncrement":"3.00","pricePrecision":2,"shorts":{"enabled":true,"lastEnabledTimestamp":10},"turnover":6})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_StockMeta, Test_equals)
{
    StockMeta stockMeta;
    StockMeta stockMeta2;

    stockMeta.instrumentId                = "a";
    stockMeta.instrumentTicker            = "b";
    stockMeta.instrumentName              = "c";
    stockMeta.forQualInvestorFlag         = true;
    stockMeta.lot                         = 1;
    stockMeta.pricePrecision              = 2;
    stockMeta.minPriceIncrement           = 3.0f;
    stockMeta.ignore                      = true;
    stockMeta.lastTradeTime               = QTime(4, 5);
    stockMeta.turnover                    = 6;
    stockMeta.dividends.createTimestamp   = 7;
    stockMeta.dividends.paymentTimestamp  = 8;
    stockMeta.dividends.yield             = 9.0f;
    stockMeta.shorts.enabled              = true;
    stockMeta.shorts.lastEnabledTimestamp = 10;

    stockMeta2.instrumentId                = "a";
    stockMeta2.instrumentTicker            = "b";
    stockMeta2.instrumentName              = "c";
    stockMeta2.forQualInvestorFlag         = true;
    stockMeta2.lot                         = 1;
    stockMeta2.pricePrecision              = 2;
    stockMeta2.minPriceIncrement           = 3.0f;
    stockMeta2.ignore                      = true;
    stockMeta2.lastTradeTime               = QTime(4, 5);
    stockMeta2.turnover                    = 6;
    stockMeta2.dividends.createTimestamp   = 7;
    stockMeta2.dividends.paymentTimestamp  = 8;
    stockMeta2.dividends.yield             = 9.0f;
    stockMeta2.shorts.enabled              = true;
    stockMeta2.shorts.lastEnabledTimestamp = 10;

    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.instrumentId = "aaaa";
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.instrumentId = "a";
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.instrumentTicker = "bbbb";
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.instrumentTicker = "b";
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.instrumentName = "cccc";
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.instrumentName = "c";
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.forQualInvestorFlag = false;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.forQualInvestorFlag = true;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.lot = -1;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.lot = 1;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.pricePrecision = -2;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.pricePrecision = 2;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.minPriceIncrement = 3000.0f;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.minPriceIncrement = 3.0f;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.ignore = false;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.ignore = true;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.lastTradeTime = QTime(5, 4);
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.lastTradeTime = QTime(4, 5);
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.turnover = -6;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.turnover = 6;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.dividends.createTimestamp = -7;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.dividends.createTimestamp = 7;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.shorts.enabled = false;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.shorts.enabled = true;
    ASSERT_EQ(stockMeta, stockMeta2);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
