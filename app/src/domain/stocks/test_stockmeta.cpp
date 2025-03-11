#include "src/domain/stocks/stockmeta.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



TEST(Test_StockMeta, Test_constructor_and_destructor)
{
    StockMeta stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta.uid,                     "");
    ASSERT_EQ(stockMeta.ticker,                  "");
    ASSERT_EQ(stockMeta.name,                    "");
    ASSERT_EQ(stockMeta.forQualInvestorFlag,     false);
    ASSERT_EQ(stockMeta.lot,                     0);
    ASSERT_EQ(stockMeta.minPriceIncrement.units, 0);
    ASSERT_EQ(stockMeta.minPriceIncrement.nano,  0);
    // clang-format on
}

TEST(Test_StockMeta, Test_copy_constructor)
{
    StockMeta stockMeta;

    stockMeta.uid                     = "a";
    stockMeta.ticker                  = "b";
    stockMeta.name                    = "c";
    stockMeta.forQualInvestorFlag     = true;
    stockMeta.lot                     = 1;
    stockMeta.minPriceIncrement.units = 2;
    stockMeta.minPriceIncrement.nano  = 3;

    StockMeta stockMeta2(stockMeta);

    // clang-format off
    ASSERT_EQ(stockMeta2.uid,                     "a");
    ASSERT_EQ(stockMeta2.ticker,                  "b");
    ASSERT_EQ(stockMeta2.name,                    "c");
    ASSERT_EQ(stockMeta2.forQualInvestorFlag,     true);
    ASSERT_EQ(stockMeta2.lot,                     1);
    ASSERT_EQ(stockMeta2.minPriceIncrement.units, 2);
    ASSERT_EQ(stockMeta2.minPriceIncrement.nano,  3);
    // clang-format on
}

TEST(Test_StockMeta, Test_assign)
{
    StockMeta stockMeta;
    StockMeta stockMeta2;

    stockMeta.uid                     = "a";
    stockMeta.ticker                  = "b";
    stockMeta.name                    = "c";
    stockMeta.forQualInvestorFlag     = true;
    stockMeta.lot                     = 1;
    stockMeta.minPriceIncrement.units = 2;
    stockMeta.minPriceIncrement.nano  = 3;

    stockMeta2 = stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta2.uid,                     "a");
    ASSERT_EQ(stockMeta2.ticker,                  "b");
    ASSERT_EQ(stockMeta2.name,                    "c");
    ASSERT_EQ(stockMeta2.forQualInvestorFlag,     true);
    ASSERT_EQ(stockMeta2.lot,                     1);
    ASSERT_EQ(stockMeta2.minPriceIncrement.units, 2);
    ASSERT_EQ(stockMeta2.minPriceIncrement.nano,  3);
    // clang-format on
}

TEST(Test_StockMeta, Test_fromJsonObject)
{
    StockMeta stockMeta;

    // clang-format off
    ASSERT_EQ(stockMeta.uid,                     "");
    ASSERT_EQ(stockMeta.ticker,                  "");
    ASSERT_EQ(stockMeta.name,                    "");
    ASSERT_EQ(stockMeta.forQualInvestorFlag,     false);
    ASSERT_EQ(stockMeta.lot,                     0);
    ASSERT_EQ(stockMeta.minPriceIncrement.units, 0);
    ASSERT_EQ(stockMeta.minPriceIncrement.nano,  0);
    // clang-format on

    QString content =
        "{\"forQualInvestorFlag\":true,\"lot\":1,\"minPriceIncrement\":{\"nano\":3,\"units\":2},\"name\":\"c\",\"ticker\":\"b\","
        "\"uid\":\"a\"}";

    QJsonParseError parseError;
    QJsonDocument   jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    stockMeta.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(stockMeta.uid,                     "a");
    ASSERT_EQ(stockMeta.ticker,                  "b");
    ASSERT_EQ(stockMeta.name,                    "c");
    ASSERT_EQ(stockMeta.forQualInvestorFlag,     true);
    ASSERT_EQ(stockMeta.lot,                     1);
    ASSERT_EQ(stockMeta.minPriceIncrement.units, 2);
    ASSERT_EQ(stockMeta.minPriceIncrement.nano,  3);
    // clang-format on
}

TEST(Test_StockMeta, Test_toJsonObject)
{
    StockMeta stockMeta;

    stockMeta.uid                     = "a";
    stockMeta.ticker                  = "b";
    stockMeta.name                    = "c";
    stockMeta.forQualInvestorFlag     = true;
    stockMeta.lot                     = 1;
    stockMeta.minPriceIncrement.units = 2;
    stockMeta.minPriceIncrement.nano  = 3;

    QJsonObject   jsonObject = stockMeta.toJsonObject();
    QJsonDocument jsonDoc(jsonObject);

    QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    QString expectedContent =
        "{\"forQualInvestorFlag\":true,\"lot\":1,\"minPriceIncrement\":{\"nano\":3,\"units\":2},\"name\":\"c\",\"ticker\":\"b\","
        "\"uid\":\"a\"}";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_StockMeta, Test_equals)
{
    StockMeta stockMeta;
    StockMeta stockMeta2;

    stockMeta.uid                     = "a";
    stockMeta.ticker                  = "b";
    stockMeta.name                    = "c";
    stockMeta.forQualInvestorFlag     = true;
    stockMeta.lot                     = 1;
    stockMeta.minPriceIncrement.units = 2;
    stockMeta.minPriceIncrement.nano  = 3;

    stockMeta2.uid                     = "a";
    stockMeta2.ticker                  = "b";
    stockMeta2.name                    = "c";
    stockMeta2.forQualInvestorFlag     = true;
    stockMeta2.lot                     = 1;
    stockMeta2.minPriceIncrement.units = 2;
    stockMeta2.minPriceIncrement.nano  = 3;

    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.uid = "aaaa";
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.uid = "a";
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.ticker = "bbbb";
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.ticker = "b";
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.name = "cccc";
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.name = "c";
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.forQualInvestorFlag = false;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.forQualInvestorFlag = true;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.lot = 1000;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.lot = 1;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.minPriceIncrement.units = 2000;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.minPriceIncrement.units = 2;
    ASSERT_EQ(stockMeta, stockMeta2);

    stockMeta2.minPriceIncrement.nano = 3000;
    ASSERT_NE(stockMeta, stockMeta2);
    stockMeta2.minPriceIncrement.nano = 3;
    ASSERT_EQ(stockMeta, stockMeta2);
}
