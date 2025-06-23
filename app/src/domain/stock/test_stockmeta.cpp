#include "src/domain/stock/stockmeta.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_StockMeta, Test_constructor_and_destructor)
{
    const StockMeta stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta.instrumentId,        "");
    ASSERT_EQ(stockMeta.instrumentTicker,    "");
    ASSERT_EQ(stockMeta.instrumentName,      "");
    ASSERT_EQ(stockMeta.forQualInvestorFlag, false);
    ASSERT_NEAR(stockMeta.minPriceIncrement, 0.0f, 0.0001f);
    ASSERT_EQ(stockMeta.pricePrecision,      0);
    // clang-format on
}

TEST(Test_StockMeta, Test_copy_constructor)
{
    StockMeta stockMeta;

    stockMeta.instrumentId        = "a";
    stockMeta.instrumentTicker    = "b";
    stockMeta.instrumentName      = "c";
    stockMeta.forQualInvestorFlag = true;
    stockMeta.minPriceIncrement   = 1.0f;
    stockMeta.pricePrecision      = 2;

    const StockMeta stockMeta2(stockMeta);

    // clang-format off
    ASSERT_EQ(stockMeta2.instrumentId,        "a");
    ASSERT_EQ(stockMeta2.instrumentTicker,    "b");
    ASSERT_EQ(stockMeta2.instrumentName,      "c");
    ASSERT_EQ(stockMeta2.forQualInvestorFlag, true);
    ASSERT_NEAR(stockMeta2.minPriceIncrement, 1.0f, 0.0001f);
    ASSERT_EQ(stockMeta2.pricePrecision,      2);
    // clang-format on
}

TEST(Test_StockMeta, Test_assign)
{
    StockMeta stockMeta;
    StockMeta stockMeta2;

    stockMeta.instrumentId        = "a";
    stockMeta.instrumentTicker    = "b";
    stockMeta.instrumentName      = "c";
    stockMeta.forQualInvestorFlag = true;
    stockMeta.minPriceIncrement   = 1.0f;
    stockMeta.pricePrecision      = 2;

    stockMeta2 = stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta2.instrumentId,        "a");
    ASSERT_EQ(stockMeta2.instrumentTicker,    "b");
    ASSERT_EQ(stockMeta2.instrumentName,      "c");
    ASSERT_EQ(stockMeta2.forQualInvestorFlag, true);
    ASSERT_NEAR(stockMeta2.minPriceIncrement, 1.0f, 0.0001f);
    ASSERT_EQ(stockMeta2.pricePrecision,      2);
    // clang-format on
}

TEST(Test_StockMeta, Test_fromJsonObject)
{
    StockMeta stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta.instrumentId,        "");
    ASSERT_EQ(stockMeta.instrumentTicker,    "");
    ASSERT_EQ(stockMeta.instrumentName,      "");
    ASSERT_EQ(stockMeta.forQualInvestorFlag, false);
    ASSERT_NEAR(stockMeta.minPriceIncrement, 0.0f, 0.0001f);
    ASSERT_EQ(stockMeta.pricePrecision,      0);
    // clang-format on

    const QString content =
        R"({"forQualInvestorFlag":true,"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","minPriceIncrement":"1.00","pricePrecision":2})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    stockMeta.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(stockMeta.instrumentId,        "a");
    ASSERT_EQ(stockMeta.instrumentTicker,    "b");
    ASSERT_EQ(stockMeta.instrumentName,      "c");
    ASSERT_EQ(stockMeta.forQualInvestorFlag, true);
    ASSERT_NEAR(stockMeta.minPriceIncrement, 1.0f, 0.0001f);
    ASSERT_EQ(stockMeta.pricePrecision,      2);
    // clang-format on
}

TEST(Test_StockMeta, Test_toJsonObject)
{
    StockMeta stockMeta;

    stockMeta.instrumentId        = "a";
    stockMeta.instrumentTicker    = "b";
    stockMeta.instrumentName      = "c";
    stockMeta.forQualInvestorFlag = true;
    stockMeta.minPriceIncrement   = 1.0f;
    stockMeta.pricePrecision      = 2;

    const QJsonObject   jsonObject = stockMeta.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"forQualInvestorFlag":true,"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","minPriceIncrement":"1.00","pricePrecision":2})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_StockMeta, Test_equals)
{
    StockMeta stockMeta;
    StockMeta stockMeta2;

    stockMeta.instrumentId        = "a";
    stockMeta.instrumentTicker    = "b";
    stockMeta.instrumentName      = "c";
    stockMeta.forQualInvestorFlag = true;
    stockMeta.minPriceIncrement   = 1.0f;
    stockMeta.pricePrecision      = 2;

    stockMeta2.instrumentId        = "a";
    stockMeta2.instrumentTicker    = "b";
    stockMeta2.instrumentName      = "c";
    stockMeta2.forQualInvestorFlag = true;
    stockMeta2.minPriceIncrement   = 1.0f;
    stockMeta2.pricePrecision      = 2;

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
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
