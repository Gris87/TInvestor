#include "src/domain/stock/stockdividendsmeta.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_StockDividendsMeta : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_StockDividendsMeta, Test_constructor_and_destructor)
{
    const StockDividendsMeta stockDividendsMeta;

    // clang-format off
    ASSERT_EQ(stockDividendsMeta.createTimestamp,  0);
    ASSERT_EQ(stockDividendsMeta.paymentTimestamp, 0);
    ASSERT_NEAR(stockDividendsMeta.price,          0.0f, 0.0001f);
    ASSERT_NEAR(stockDividendsMeta.yield,          0.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StockDividendsMeta, Test_copy_constructor)
{
    StockDividendsMeta stockDividendsMeta;

    stockDividendsMeta.createTimestamp  = 1;
    stockDividendsMeta.paymentTimestamp = 2;
    stockDividendsMeta.price            = 3;
    stockDividendsMeta.yield            = 4;

    const StockDividendsMeta stockDividendsMeta2(stockDividendsMeta);

    // clang-format off
    ASSERT_EQ(stockDividendsMeta2.createTimestamp,  1);
    ASSERT_EQ(stockDividendsMeta2.paymentTimestamp, 2);
    ASSERT_NEAR(stockDividendsMeta2.price,          3.0f, 0.0001f);
    ASSERT_NEAR(stockDividendsMeta2.yield,          4.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StockDividendsMeta, Test_assign)
{
    StockDividendsMeta stockDividendsMeta;
    StockDividendsMeta stockDividendsMeta2;

    stockDividendsMeta.createTimestamp  = 1;
    stockDividendsMeta.paymentTimestamp = 2;
    stockDividendsMeta.price            = 3;
    stockDividendsMeta.yield            = 4;

    stockDividendsMeta2 = stockDividendsMeta;

    // clang-format off
    ASSERT_EQ(stockDividendsMeta2.createTimestamp,  1);
    ASSERT_EQ(stockDividendsMeta2.paymentTimestamp, 2);
    ASSERT_NEAR(stockDividendsMeta2.price,          3.0f, 0.0001f);
    ASSERT_NEAR(stockDividendsMeta2.yield,          4.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StockDividendsMeta, Test_fromJsonObject)
{
    StockDividendsMeta stockDividendsMeta;

    // clang-format off
    ASSERT_EQ(stockDividendsMeta.createTimestamp,  0);
    ASSERT_EQ(stockDividendsMeta.paymentTimestamp, 0);
    ASSERT_NEAR(stockDividendsMeta.price,          0.0f, 0.0001f);
    ASSERT_NEAR(stockDividendsMeta.yield,          0.0f, 0.0001f);
    // clang-format on

    const QString content = R"({"createTimestamp":1,"paymentTimestamp":2,"price":"3.00","yield":"4.00"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    stockDividendsMeta.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(stockDividendsMeta.createTimestamp,  1);
    ASSERT_EQ(stockDividendsMeta.paymentTimestamp, 2);
    ASSERT_NEAR(stockDividendsMeta.price,          3.0f, 0.0001f);
    ASSERT_NEAR(stockDividendsMeta.yield,          4.0f, 0.0001f);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    stockDividendsMeta.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_StockDividendsMeta, Test_toJsonObject)
{
    StockDividendsMeta stockDividendsMeta;

    stockDividendsMeta.createTimestamp  = 1;
    stockDividendsMeta.paymentTimestamp = 2;
    stockDividendsMeta.price            = 3;
    stockDividendsMeta.yield            = 4;

    const QJsonObject   jsonObject = stockDividendsMeta.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"createTimestamp":1,"paymentTimestamp":2,"price":"3.00","yield":"4.00"})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_StockDividendsMeta, Test_equals)
{
    StockDividendsMeta stockDividendsMeta;
    StockDividendsMeta stockDividendsMeta2;

    stockDividendsMeta.createTimestamp  = 1;
    stockDividendsMeta.paymentTimestamp = 2;
    stockDividendsMeta.price            = 3;
    stockDividendsMeta.yield            = 4;

    stockDividendsMeta2.createTimestamp  = 1;
    stockDividendsMeta2.paymentTimestamp = 2;
    stockDividendsMeta2.price            = 3;
    stockDividendsMeta2.yield            = 4;

    ASSERT_EQ(stockDividendsMeta, stockDividendsMeta2);

    stockDividendsMeta2.createTimestamp = -1;
    ASSERT_NE(stockDividendsMeta, stockDividendsMeta2);
    stockDividendsMeta2.createTimestamp = 1;
    ASSERT_EQ(stockDividendsMeta, stockDividendsMeta2);

    stockDividendsMeta2.paymentTimestamp = -2;
    ASSERT_NE(stockDividendsMeta, stockDividendsMeta2);
    stockDividendsMeta2.paymentTimestamp = 2;
    ASSERT_EQ(stockDividendsMeta, stockDividendsMeta2);

    stockDividendsMeta2.price = 3000.0f;
    ASSERT_NE(stockDividendsMeta, stockDividendsMeta2);
    stockDividendsMeta2.price = 3.0f;
    ASSERT_EQ(stockDividendsMeta, stockDividendsMeta2);

    stockDividendsMeta2.yield = 4000.0f;
    ASSERT_NE(stockDividendsMeta, stockDividendsMeta2);
    stockDividendsMeta2.yield = 4.0f;
    ASSERT_EQ(stockDividendsMeta, stockDividendsMeta2);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
