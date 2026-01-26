#include "src/domain/stock/stockshortsmeta.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_StockShortsMeta : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_StockShortsMeta, Test_constructor_and_destructor)
{
    const StockShortsMeta stockShortsMeta;

    // clang-format off
    ASSERT_EQ(stockShortsMeta.enabled,              false);
    ASSERT_EQ(stockShortsMeta.lastEnabledTimestamp, 0);
    // clang-format on
}

TEST_F(Test_StockShortsMeta, Test_copy_constructor)
{
    StockShortsMeta stockShortsMeta;

    stockShortsMeta.enabled              = true;
    stockShortsMeta.lastEnabledTimestamp = 1;

    const StockShortsMeta stockShortsMeta2(stockShortsMeta);

    // clang-format off
    ASSERT_EQ(stockShortsMeta2.enabled,              true);
    ASSERT_EQ(stockShortsMeta2.lastEnabledTimestamp, 1);
    // clang-format on
}

TEST_F(Test_StockShortsMeta, Test_assign)
{
    StockShortsMeta stockShortsMeta;
    StockShortsMeta stockShortsMeta2;

    stockShortsMeta.enabled              = true;
    stockShortsMeta.lastEnabledTimestamp = 1;

    stockShortsMeta2 = stockShortsMeta;

    // clang-format off
    ASSERT_EQ(stockShortsMeta2.enabled,              true);
    ASSERT_EQ(stockShortsMeta2.lastEnabledTimestamp, 1);
    // clang-format on
}

TEST_F(Test_StockShortsMeta, Test_fromJsonObject)
{
    StockShortsMeta stockShortsMeta;

    // clang-format off
    ASSERT_EQ(stockShortsMeta.enabled,              false);
    ASSERT_EQ(stockShortsMeta.lastEnabledTimestamp, 0);
    // clang-format on

    const QString content = R"({"enabled":true,"lastEnabledTimestamp":1})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    stockShortsMeta.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(stockShortsMeta.enabled,              true);
    ASSERT_EQ(stockShortsMeta.lastEnabledTimestamp, 1);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    stockShortsMeta.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_StockShortsMeta, Test_toJsonObject)
{
    StockShortsMeta stockShortsMeta;

    stockShortsMeta.enabled              = true;
    stockShortsMeta.lastEnabledTimestamp = 1;

    const QJsonObject   jsonObject = stockShortsMeta.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"enabled":true,"lastEnabledTimestamp":1})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_StockShortsMeta, Test_equals)
{
    StockShortsMeta stockShortsMeta;
    StockShortsMeta stockShortsMeta2;

    stockShortsMeta.enabled              = true;
    stockShortsMeta.lastEnabledTimestamp = 1;

    stockShortsMeta2.enabled              = true;
    stockShortsMeta2.lastEnabledTimestamp = 1;

    ASSERT_EQ(stockShortsMeta, stockShortsMeta2);

    stockShortsMeta2.enabled = false;
    ASSERT_NE(stockShortsMeta, stockShortsMeta2);
    stockShortsMeta2.enabled = true;
    ASSERT_EQ(stockShortsMeta, stockShortsMeta2);

    stockShortsMeta2.lastEnabledTimestamp = -1;
    ASSERT_NE(stockShortsMeta, stockShortsMeta2);
    stockShortsMeta2.lastEnabledTimestamp = 1;
    ASSERT_EQ(stockShortsMeta, stockShortsMeta2);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
