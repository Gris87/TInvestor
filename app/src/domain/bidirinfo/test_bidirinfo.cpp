#include "src/domain/bidirinfo/bidirinfo.h"

#include <QJsonDocument>
#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"



class Test_BidirInfo : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_BidirInfo, Test_constructor_and_destructor)
{
    const BidirInfo bidirInfo;

    // clang-format off
    ASSERT_EQ(bidirInfo.ticker,            "");
    ASSERT_EQ(bidirInfo.name,              "");
    ASSERT_EQ(bidirInfo.lot,               0);
    ASSERT_EQ(bidirInfo.pricePrecision,    0);
    ASSERT_EQ(bidirInfo.minPriceIncrement, Quotation(0, 0));
    // clang-format on
}

TEST_F(Test_BidirInfo, Test_copy_constructor)
{
    BidirInfo bidirInfo;

    bidirInfo.ticker            = "a";
    bidirInfo.name              = "b";
    bidirInfo.lot               = 1;
    bidirInfo.pricePrecision    = 2;
    bidirInfo.minPriceIncrement = Quotation(3, 4);

    const BidirInfo bidirInfo2(bidirInfo);

    // clang-format off
    ASSERT_EQ(bidirInfo2.ticker,            "a");
    ASSERT_EQ(bidirInfo2.name,              "b");
    ASSERT_EQ(bidirInfo2.lot,               1);
    ASSERT_EQ(bidirInfo2.pricePrecision,    2);
    ASSERT_EQ(bidirInfo2.minPriceIncrement, Quotation(3, 4));
    // clang-format on
}

TEST_F(Test_BidirInfo, Test_assign)
{
    BidirInfo bidirInfo;
    BidirInfo bidirInfo2;

    bidirInfo.ticker            = "a";
    bidirInfo.name              = "b";
    bidirInfo.lot               = 1;
    bidirInfo.pricePrecision    = 2;
    bidirInfo.minPriceIncrement = Quotation(3, 4);

    bidirInfo2 = bidirInfo;

    // clang-format off
    ASSERT_EQ(bidirInfo2.ticker,            "a");
    ASSERT_EQ(bidirInfo2.name,              "b");
    ASSERT_EQ(bidirInfo2.lot,               1);
    ASSERT_EQ(bidirInfo2.pricePrecision,    2);
    ASSERT_EQ(bidirInfo2.minPriceIncrement, Quotation(3, 4));
    // clang-format on
}

TEST_F(Test_BidirInfo, Test_fromJsonObject)
{
    BidirInfo bidirInfo;

    // clang-format off
    ASSERT_EQ(bidirInfo.ticker,            "");
    ASSERT_EQ(bidirInfo.name,              "");
    ASSERT_EQ(bidirInfo.lot,               0);
    ASSERT_EQ(bidirInfo.pricePrecision,    0);
    ASSERT_EQ(bidirInfo.minPriceIncrement, Quotation(0, 0));
    // clang-format on

    const QString content = R"({"lot":1,"minPriceIncrement":{"nano":4,"units":3},"name":"b","pricePrecision":2,"ticker":"a"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    bidirInfo.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(bidirInfo.ticker,            "a");
    ASSERT_EQ(bidirInfo.name,              "b");
    ASSERT_EQ(bidirInfo.lot,               1);
    ASSERT_EQ(bidirInfo.pricePrecision,    2);
    ASSERT_EQ(bidirInfo.minPriceIncrement, Quotation(3, 4));
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    bidirInfo.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_BidirInfo, Test_toJsonObject)
{
    BidirInfo bidirInfo;

    bidirInfo.ticker            = "a";
    bidirInfo.name              = "b";
    bidirInfo.lot               = 1;
    bidirInfo.pricePrecision    = 2;
    bidirInfo.minPriceIncrement = Quotation(3, 4);

    const QJsonObject   jsonObject = bidirInfo.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"lot":1,"minPriceIncrement":{"nano":4,"units":3},"name":"b","pricePrecision":2,"ticker":"a"})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_BidirInfo, Test_equals)
{
    BidirInfo bidirInfo;
    BidirInfo bidirInfo2;

    bidirInfo.ticker            = "a";
    bidirInfo.name              = "b";
    bidirInfo.lot               = 1;
    bidirInfo.pricePrecision    = 2;
    bidirInfo.minPriceIncrement = Quotation(3, 4);

    bidirInfo2.ticker            = "a";
    bidirInfo2.name              = "b";
    bidirInfo2.lot               = 1;
    bidirInfo2.pricePrecision    = 2;
    bidirInfo2.minPriceIncrement = Quotation(3, 4);

    ASSERT_EQ(bidirInfo, bidirInfo2);

    bidirInfo2.ticker = "aaaa";
    ASSERT_NE(bidirInfo, bidirInfo2);
    bidirInfo2.ticker = "a";
    ASSERT_EQ(bidirInfo, bidirInfo2);

    bidirInfo2.name = "bbbb";
    ASSERT_NE(bidirInfo, bidirInfo2);
    bidirInfo2.name = "b";
    ASSERT_EQ(bidirInfo, bidirInfo2);

    bidirInfo2.lot = -1;
    ASSERT_NE(bidirInfo, bidirInfo2);
    bidirInfo2.lot = 1;
    ASSERT_EQ(bidirInfo, bidirInfo2);

    bidirInfo2.pricePrecision = -2;
    ASSERT_NE(bidirInfo, bidirInfo2);
    bidirInfo2.pricePrecision = 2;
    ASSERT_EQ(bidirInfo, bidirInfo2);

    bidirInfo2.minPriceIncrement = Quotation(-3, -4);
    ASSERT_NE(bidirInfo, bidirInfo2);
    bidirInfo2.minPriceIncrement = Quotation(3, 4);
    ASSERT_EQ(bidirInfo, bidirInfo2);
}
