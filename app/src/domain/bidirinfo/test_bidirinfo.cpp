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
    ASSERT_NEAR(bidirInfo.spread,     0.0f, 0.0001f);
    ASSERT_NEAR(bidirInfo.minYield,   0.0f, 0.0001f);
    ASSERT_NEAR(bidirInfo.totalYield, 0.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_BidirInfo, Test_copy_constructor)
{
    BidirInfo bidirInfo;

    bidirInfo.spread     = 1.0f;
    bidirInfo.minYield   = 2.0f;
    bidirInfo.totalYield = 3.0f;

    const BidirInfo bidirInfo2(bidirInfo);

    // clang-format off
    ASSERT_NEAR(bidirInfo2.spread,     1.0f, 0.0001f);
    ASSERT_NEAR(bidirInfo2.minYield,   2.0f, 0.0001f);
    ASSERT_NEAR(bidirInfo2.totalYield, 3.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_BidirInfo, Test_assign)
{
    BidirInfo bidirInfo;
    BidirInfo bidirInfo2;

    bidirInfo.spread     = 1.0f;
    bidirInfo.minYield   = 2.0f;
    bidirInfo.totalYield = 3.0f;

    bidirInfo2 = bidirInfo;

    // clang-format off
    ASSERT_NEAR(bidirInfo2.spread,     1.0f, 0.0001f);
    ASSERT_NEAR(bidirInfo2.minYield,   2.0f, 0.0001f);
    ASSERT_NEAR(bidirInfo2.totalYield, 3.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_BidirInfo, Test_fromJsonObject)
{
    BidirInfo bidirInfo;

    // clang-format off
    ASSERT_NEAR(bidirInfo.spread,     0.0f, 0.0001f);
    ASSERT_NEAR(bidirInfo.minYield,   0.0f, 0.0001f);
    ASSERT_NEAR(bidirInfo.totalYield, 0.0f, 0.0001f);
    // clang-format on

    const QString content = R"({"minYield":2,"spread":1,"totalYield":3})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    bidirInfo.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_NEAR(bidirInfo.spread,     1.0f, 0.0001f);
    ASSERT_NEAR(bidirInfo.minYield,   2.0f, 0.0001f);
    ASSERT_NEAR(bidirInfo.totalYield, 3.0f, 0.0001f);
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

    bidirInfo.spread     = 1.0f;
    bidirInfo.minYield   = 2.0f;
    bidirInfo.totalYield = 3.0f;

    const QJsonObject   jsonObject = bidirInfo.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"minYield":2,"spread":1,"totalYield":3})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_BidirInfo, Test_equals)
{
    BidirInfo bidirInfo;
    BidirInfo bidirInfo2;

    bidirInfo.spread     = 1.0f;
    bidirInfo.minYield   = 2.0f;
    bidirInfo.totalYield = 3.0f;

    bidirInfo2.spread     = 1.0f;
    bidirInfo2.minYield   = 2.0f;
    bidirInfo2.totalYield = 3.0f;

    ASSERT_EQ(bidirInfo, bidirInfo2);

    bidirInfo2.spread = -1.0f;
    ASSERT_NE(bidirInfo, bidirInfo2);
    bidirInfo2.spread = 1.0f;
    ASSERT_EQ(bidirInfo, bidirInfo2);

    bidirInfo2.minYield = -2.0f;
    ASSERT_NE(bidirInfo, bidirInfo2);
    bidirInfo2.minYield = 2.0f;
    ASSERT_EQ(bidirInfo, bidirInfo2);

    bidirInfo2.totalYield = -3.0f;
    ASSERT_NE(bidirInfo, bidirInfo2);
    bidirInfo2.totalYield = 3.0f;
    ASSERT_EQ(bidirInfo, bidirInfo2);
}
