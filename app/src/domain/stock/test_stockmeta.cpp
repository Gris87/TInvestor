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
    ASSERT_EQ(stockMeta.instrumentId,        "");
    ASSERT_EQ(stockMeta.instrumentLogo,      nullptr);
    ASSERT_EQ(stockMeta.instrumentTicker,    "");
    ASSERT_EQ(stockMeta.instrumentName,      "");
    ASSERT_EQ(stockMeta.forQualInvestorFlag, false);
    ASSERT_NEAR(stockMeta.minPriceIncrement, 0.0f, 0.0001f);
    ASSERT_EQ(stockMeta.pricePrecision,      0);
    ASSERT_EQ(stockMeta.lastTradeTime,       QTime(0, 0));
    ASSERT_EQ(stockMeta.turnover,            0);
    ASSERT_NEAR(stockMeta.rsiMonth,          0.0f, 0.0001f);
    ASSERT_NEAR(stockMeta.rsiWeek,           0.0f, 0.0001f);
    ASSERT_NEAR(stockMeta.rsiDay,            0.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StockMeta, Test_copy_constructor)
{
    StockMeta stockMeta;

    stockMeta.instrumentId        = "a";
    stockMeta.instrumentTicker    = "b";
    stockMeta.instrumentName      = "c";
    stockMeta.forQualInvestorFlag = true;
    stockMeta.minPriceIncrement   = 1.0f;
    stockMeta.pricePrecision      = 2;
    stockMeta.lastTradeTime       = QTime(3, 4);
    stockMeta.turnover            = 5;
    stockMeta.rsiMonth            = 6;
    stockMeta.rsiWeek             = 7;
    stockMeta.rsiDay              = 8;

    const StockMeta stockMeta2(stockMeta);

    // clang-format off
    ASSERT_EQ(stockMeta2.instrumentId,        "a");
    ASSERT_EQ(stockMeta2.instrumentLogo,      nullptr);
    ASSERT_EQ(stockMeta2.instrumentTicker,    "b");
    ASSERT_EQ(stockMeta2.instrumentName,      "c");
    ASSERT_EQ(stockMeta2.forQualInvestorFlag, true);
    ASSERT_NEAR(stockMeta2.minPriceIncrement, 1.0f, 0.0001f);
    ASSERT_EQ(stockMeta2.pricePrecision,      2);
    ASSERT_EQ(stockMeta2.lastTradeTime,       QTime(3, 4));
    ASSERT_EQ(stockMeta2.turnover,            5);
    ASSERT_NEAR(stockMeta2.rsiMonth,          6.0f, 0.0001f);
    ASSERT_NEAR(stockMeta2.rsiWeek,           7.0f, 0.0001f);
    ASSERT_NEAR(stockMeta2.rsiDay,            8.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StockMeta, Test_assign)
{
    StockMeta stockMeta;
    StockMeta stockMeta2;

    stockMeta.instrumentId        = "a";
    stockMeta.instrumentTicker    = "b";
    stockMeta.instrumentName      = "c";
    stockMeta.forQualInvestorFlag = true;
    stockMeta.minPriceIncrement   = 1.0f;
    stockMeta.pricePrecision      = 2;
    stockMeta.lastTradeTime       = QTime(3, 4);
    stockMeta.turnover            = 5;
    stockMeta.rsiMonth            = 6;
    stockMeta.rsiWeek             = 7;
    stockMeta.rsiDay              = 8;

    stockMeta2 = stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta2.instrumentId,        "a");
    ASSERT_EQ(stockMeta2.instrumentLogo,      nullptr);
    ASSERT_EQ(stockMeta2.instrumentTicker,    "b");
    ASSERT_EQ(stockMeta2.instrumentName,      "c");
    ASSERT_EQ(stockMeta2.forQualInvestorFlag, true);
    ASSERT_NEAR(stockMeta2.minPriceIncrement, 1.0f, 0.0001f);
    ASSERT_EQ(stockMeta2.pricePrecision,      2);
    ASSERT_EQ(stockMeta2.lastTradeTime,       QTime(3, 4));
    ASSERT_EQ(stockMeta2.turnover,            5);
    ASSERT_NEAR(stockMeta2.rsiMonth,          6.0f, 0.0001f);
    ASSERT_NEAR(stockMeta2.rsiWeek,           7.0f, 0.0001f);
    ASSERT_NEAR(stockMeta2.rsiDay,            8.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StockMeta, Test_fromJsonObject)
{
    StockMeta stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta.instrumentId,        "");
    ASSERT_EQ(stockMeta.instrumentLogo,      nullptr);
    ASSERT_EQ(stockMeta.instrumentTicker,    "");
    ASSERT_EQ(stockMeta.instrumentName,      "");
    ASSERT_EQ(stockMeta.forQualInvestorFlag, false);
    ASSERT_NEAR(stockMeta.minPriceIncrement, 0.0f, 0.0001f);
    ASSERT_EQ(stockMeta.pricePrecision,      0);
    ASSERT_EQ(stockMeta.lastTradeTime,       QTime(0, 0));
    ASSERT_EQ(stockMeta.turnover,            0);
    ASSERT_NEAR(stockMeta.rsiMonth,          0.0f, 0.0001f);
    ASSERT_NEAR(stockMeta.rsiWeek,           0.0f, 0.0001f);
    ASSERT_NEAR(stockMeta.rsiDay,            0.0f, 0.0001f);
    // clang-format on

    const QString content =
        R"({"forQualInvestorFlag":true,"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","lastTradeTime":"03:04:00","minPriceIncrement":"1.00","pricePrecision":2,"rsiDay":"8.00","rsiMonth":"6.00","rsiWeek":"7.00","turnover":5})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    stockMeta.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(stockMeta.instrumentId,        "a");
    ASSERT_EQ(stockMeta.instrumentLogo,      nullptr);
    ASSERT_EQ(stockMeta.instrumentTicker,    "b");
    ASSERT_EQ(stockMeta.instrumentName,      "c");
    ASSERT_EQ(stockMeta.forQualInvestorFlag, true);
    ASSERT_NEAR(stockMeta.minPriceIncrement, 1.0f, 0.0001f);
    ASSERT_EQ(stockMeta.pricePrecision,      2);
    ASSERT_EQ(stockMeta.lastTradeTime,       QTime(3, 4));
    ASSERT_EQ(stockMeta.turnover,            5);
    ASSERT_NEAR(stockMeta.rsiMonth,          6.0f, 0.0001f);
    ASSERT_NEAR(stockMeta.rsiWeek,           7.0f, 0.0001f);
    ASSERT_NEAR(stockMeta.rsiDay,            8.0f, 0.0001f);
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

    stockMeta.instrumentId        = "a";
    stockMeta.instrumentTicker    = "b";
    stockMeta.instrumentName      = "c";
    stockMeta.forQualInvestorFlag = true;
    stockMeta.minPriceIncrement   = 1.0f;
    stockMeta.pricePrecision      = 2;
    stockMeta.lastTradeTime       = QTime(3, 4);
    stockMeta.turnover            = 5;
    stockMeta.rsiMonth            = 6;
    stockMeta.rsiWeek             = 7;
    stockMeta.rsiDay              = 8;

    const QJsonObject   jsonObject = stockMeta.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"forQualInvestorFlag":true,"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","lastTradeTime":"03:04:00","minPriceIncrement":"1.00","pricePrecision":2,"rsiDay":"8.00","rsiMonth":"6.00","rsiWeek":"7.00","turnover":5})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_StockMeta, Test_equals)
{
    StockMeta stockMeta;
    StockMeta stockMeta2;

    stockMeta.instrumentId        = "a";
    stockMeta.instrumentTicker    = "b";
    stockMeta.instrumentName      = "c";
    stockMeta.forQualInvestorFlag = true;
    stockMeta.minPriceIncrement   = 1.0f;
    stockMeta.pricePrecision      = 2;
    stockMeta.lastTradeTime       = QTime(3, 4);
    stockMeta.turnover            = 5;
    stockMeta.rsiMonth            = 6;
    stockMeta.rsiWeek             = 7;
    stockMeta.rsiDay              = 8;

    stockMeta2.instrumentId        = "a";
    stockMeta2.instrumentTicker    = "b";
    stockMeta2.instrumentName      = "c";
    stockMeta2.forQualInvestorFlag = true;
    stockMeta2.minPriceIncrement   = 1.0f;
    stockMeta2.pricePrecision      = 2;
    stockMeta2.lastTradeTime       = QTime(3, 4);
    stockMeta2.turnover            = 5;
    stockMeta2.rsiMonth            = 6;
    stockMeta2.rsiWeek             = 7;
    stockMeta2.rsiDay              = 8;

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

    stockMeta2.minPriceIncrement = 1000.0f;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.minPriceIncrement = 1.0f;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.pricePrecision = -2;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.pricePrecision = 2;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.lastTradeTime = QTime(4, 3);
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.lastTradeTime = QTime(3, 4);
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.turnover = 1005;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.turnover = 5;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.rsiMonth = 6000.0f;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.rsiMonth = 6.0f;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.rsiWeek = 7000.0f;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.rsiWeek = 7.0f;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.rsiDay = 8000.0f;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.rsiDay = 8.0f;
    ASSERT_EQ(stockMeta, stockMeta2);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
