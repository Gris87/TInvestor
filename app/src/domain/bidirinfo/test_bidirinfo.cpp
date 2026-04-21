#include "src/domain/bidirinfo/bidirinfo.h"

#include <QJsonDocument>
#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"



class Test_BiDirInfo : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_BiDirInfo, Test_constructor_and_destructor)
{
    const BiDirInfo biDirInfo;

    // clang-format off
    ASSERT_NEAR(biDirInfo.spread,     0.0f, 0.0001f);
    ASSERT_NEAR(biDirInfo.minYield,   0.0f, 0.0001f);
    ASSERT_NEAR(biDirInfo.totalYield, 0.0f, 0.0001f);
    ASSERT_EQ(biDirInfo.priority,     BIDIR_PRIORITY_LOW);
    // clang-format on
}

TEST_F(Test_BiDirInfo, Test_copy_constructor)
{
    BiDirInfo biDirInfo;

    biDirInfo.spread     = 1.0f;
    biDirInfo.minYield   = 2.0f;
    biDirInfo.totalYield = 3.0f;
    biDirInfo.priority   = BIDIR_PRIORITY_HIGH;

    const BiDirInfo biDirInfo2(biDirInfo);

    // clang-format off
    ASSERT_NEAR(biDirInfo2.spread,     1.0f, 0.0001f);
    ASSERT_NEAR(biDirInfo2.minYield,   2.0f, 0.0001f);
    ASSERT_NEAR(biDirInfo2.totalYield, 3.0f, 0.0001f);
    ASSERT_EQ(biDirInfo2.priority,     BIDIR_PRIORITY_HIGH);
    // clang-format on
}

TEST_F(Test_BiDirInfo, Test_assign)
{
    BiDirInfo biDirInfo;
    BiDirInfo biDirInfo2;

    biDirInfo.spread     = 1.0f;
    biDirInfo.minYield   = 2.0f;
    biDirInfo.totalYield = 3.0f;
    biDirInfo.priority   = BIDIR_PRIORITY_HIGH;

    biDirInfo2 = biDirInfo;

    // clang-format off
    ASSERT_NEAR(biDirInfo2.spread,     1.0f, 0.0001f);
    ASSERT_NEAR(biDirInfo2.minYield,   2.0f, 0.0001f);
    ASSERT_NEAR(biDirInfo2.totalYield, 3.0f, 0.0001f);
    ASSERT_EQ(biDirInfo2.priority,     BIDIR_PRIORITY_HIGH);
    // clang-format on
}

TEST_F(Test_BiDirInfo, Test_fromJsonObject)
{
    BiDirInfo biDirInfo;

    // clang-format off
    ASSERT_NEAR(biDirInfo.spread,     0.0f, 0.0001f);
    ASSERT_NEAR(biDirInfo.minYield,   0.0f, 0.0001f);
    ASSERT_NEAR(biDirInfo.totalYield, 0.0f, 0.0001f);
    ASSERT_EQ(biDirInfo.priority,     BIDIR_PRIORITY_LOW);
    // clang-format on

    const QString content = R"({"minYield":2,"priority":"high","spread":1,"totalYield":3})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    biDirInfo.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_NEAR(biDirInfo.spread,     1.0f, 0.0001f);
    ASSERT_NEAR(biDirInfo.minYield,   2.0f, 0.0001f);
    ASSERT_NEAR(biDirInfo.totalYield, 3.0f, 0.0001f);
    ASSERT_EQ(biDirInfo.priority,     BIDIR_PRIORITY_HIGH);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    biDirInfo.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_BiDirInfo, Test_toJsonObject)
{
    BiDirInfo biDirInfo;

    biDirInfo.spread     = 1.0f;
    biDirInfo.minYield   = 2.0f;
    biDirInfo.totalYield = 3.0f;
    biDirInfo.priority   = BIDIR_PRIORITY_HIGH;

    const QJsonObject   jsonObject = biDirInfo.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"minYield":2,"priority":"high","spread":1,"totalYield":3})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_BiDirInfo, Test_equals)
{
    BiDirInfo biDirInfo;
    BiDirInfo biDirInfo2;

    biDirInfo.spread     = 1.0f;
    biDirInfo.minYield   = 2.0f;
    biDirInfo.totalYield = 3.0f;
    biDirInfo.priority   = BIDIR_PRIORITY_HIGH;

    biDirInfo2.spread     = 1.0f;
    biDirInfo2.minYield   = 2.0f;
    biDirInfo2.totalYield = 3.0f;
    biDirInfo2.priority   = BIDIR_PRIORITY_HIGH;

    ASSERT_EQ(biDirInfo, biDirInfo2);

    biDirInfo2.spread = -1.0f;
    ASSERT_NE(biDirInfo, biDirInfo2);
    biDirInfo2.spread = 1.0f;
    ASSERT_EQ(biDirInfo, biDirInfo2);

    biDirInfo2.minYield = -2.0f;
    ASSERT_NE(biDirInfo, biDirInfo2);
    biDirInfo2.minYield = 2.0f;
    ASSERT_EQ(biDirInfo, biDirInfo2);

    biDirInfo2.totalYield = -3.0f;
    ASSERT_NE(biDirInfo, biDirInfo2);
    biDirInfo2.totalYield = 3.0f;
    ASSERT_EQ(biDirInfo, biDirInfo2);

    biDirInfo2.priority = BIDIR_PRIORITY_NORMAL;
    ASSERT_NE(biDirInfo, biDirInfo2);
    biDirInfo2.priority = BIDIR_PRIORITY_HIGH;
    ASSERT_EQ(biDirInfo, biDirInfo2);
}
