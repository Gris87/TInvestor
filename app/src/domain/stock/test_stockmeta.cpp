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
    stockMeta.pricePrecision      = 1;

    const StockMeta stockMeta2(stockMeta);

    // clang-format off
    ASSERT_EQ(stockMeta2.instrumentId,        "a");
    ASSERT_EQ(stockMeta2.instrumentTicker,    "b");
    ASSERT_EQ(stockMeta2.instrumentName,      "c");
    ASSERT_EQ(stockMeta2.forQualInvestorFlag, true);
    ASSERT_EQ(stockMeta2.pricePrecision,      1);
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
    stockMeta.pricePrecision      = 1;

    stockMeta2 = stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta2.instrumentId,        "a");
    ASSERT_EQ(stockMeta2.instrumentTicker,    "b");
    ASSERT_EQ(stockMeta2.instrumentName,      "c");
    ASSERT_EQ(stockMeta2.forQualInvestorFlag, true);
    ASSERT_EQ(stockMeta2.pricePrecision,      1);
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
    ASSERT_EQ(stockMeta.pricePrecision,      0);
    // clang-format on

    const QString content =
        R"({"forQualInvestorFlag":true,"lot":1,"minPriceIncrement":{"nano":3,"units":2},"name":"c","ticker":"b","uid":"a"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    stockMeta.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(stockMeta.instrumentId,        "a");
    ASSERT_EQ(stockMeta.instrumentTicker,    "b");
    ASSERT_EQ(stockMeta.instrumentName,      "c");
    ASSERT_EQ(stockMeta.forQualInvestorFlag, true);
    ASSERT_EQ(stockMeta.pricePrecision,      1);
    // clang-format on
}

TEST(Test_StockMeta, Test_toJsonObject)
{
    StockMeta stockMeta;

    stockMeta.instrumentId        = "a";
    stockMeta.instrumentTicker    = "b";
    stockMeta.instrumentName      = "c";
    stockMeta.forQualInvestorFlag = true;
    stockMeta.pricePrecision      = 1;

    const QJsonObject   jsonObject = stockMeta.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"forQualInvestorFlag":true,"lot":1,"minPriceIncrement":{"nano":3,"units":2},"name":"c","ticker":"b","uid":"a"})";

    qInfo() << content;

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
    stockMeta.pricePrecision      = 1;

    stockMeta2.instrumentId        = "a";
    stockMeta2.instrumentTicker    = "b";
    stockMeta2.instrumentName      = "c";
    stockMeta2.forQualInvestorFlag = true;
    stockMeta2.pricePrecision      = 1;

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

    stockMeta2.pricePrecision = -1;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.pricePrecision = 1;
    ASSERT_EQ(stockMeta, stockMeta2);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
