#include "src/domain/notification/notificationinfo.h"

#include <QJsonDocument>
#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"



class Test_NotificationInfo : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_NotificationInfo, Test_constructor_and_destructor)
{
    const NotificationInfo info;

    // clang-format off
    ASSERT_EQ(info.requestTimestamp, 0);
    ASSERT_EQ(info.timestamp,        0);
    ASSERT_EQ(info.messageType,      MESSAGE_TYPE_NONE);
    ASSERT_EQ(info.text,             "");
    // clang-format on
}

TEST_F(Test_NotificationInfo, Test_copy_constructor)
{
    NotificationInfo info;

    info.requestTimestamp = 1;
    info.timestamp        = 2;
    info.messageType      = MESSAGE_TYPE_PORTFOLIO;
    info.text             = "a";

    const NotificationInfo info2(info);

    // clang-format off
    ASSERT_EQ(info2.requestTimestamp, 1);
    ASSERT_EQ(info2.timestamp,        2);
    ASSERT_EQ(info2.messageType,      MESSAGE_TYPE_PORTFOLIO);
    ASSERT_EQ(info2.text,             "a");
    // clang-format on
}

TEST_F(Test_NotificationInfo, Test_assign)
{
    NotificationInfo info;
    NotificationInfo info2;

    info.requestTimestamp = 1;
    info.timestamp        = 2;
    info.messageType      = MESSAGE_TYPE_PORTFOLIO;
    info.text             = "a";

    info2 = info;

    // clang-format off
    ASSERT_EQ(info2.requestTimestamp, 1);
    ASSERT_EQ(info2.timestamp,        2);
    ASSERT_EQ(info2.messageType,      MESSAGE_TYPE_PORTFOLIO);
    ASSERT_EQ(info2.text,             "a");
    // clang-format on
}
TEST_F(Test_NotificationInfo, Test_fromJsonObject)
{
    NotificationInfo info;

    // clang-format off
    ASSERT_EQ(info.requestTimestamp, 0);
    ASSERT_EQ(info.timestamp,        0);
    ASSERT_EQ(info.messageType,      MESSAGE_TYPE_NONE);
    ASSERT_EQ(info.text,             "");
    // clang-format on

    const QString content = R"({"requestTimestamp":1,"text":"a","timestamp":2,"type":"portfolio"})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    info.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(info.requestTimestamp, 1);
    ASSERT_EQ(info.timestamp,        2);
    ASSERT_EQ(info.messageType,      MESSAGE_TYPE_PORTFOLIO);
    ASSERT_EQ(info.text,             "a");
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    info.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_NotificationInfo, Test_toJsonObject)
{
    NotificationInfo info;

    info.requestTimestamp = 1;
    info.timestamp        = 2;
    info.messageType      = MESSAGE_TYPE_PORTFOLIO;
    info.text             = "a";

    const QJsonObject   jsonObject = info.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"requestTimestamp":1,"text":"a","timestamp":2,"type":"portfolio"})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_NotificationInfo, Test_equals)
{
    NotificationInfo info;
    NotificationInfo info2;

    info.requestTimestamp = 1;
    info.timestamp        = 2;
    info.messageType      = MESSAGE_TYPE_PORTFOLIO;
    info.text             = "a";

    info2.requestTimestamp = 1;
    info2.timestamp        = 2;
    info2.messageType      = MESSAGE_TYPE_PORTFOLIO;
    info2.text             = "a";

    ASSERT_EQ(info, info2);

    info2.requestTimestamp = -1;
    ASSERT_NE(info, info2);
    info2.requestTimestamp = 1;
    ASSERT_EQ(info, info2);

    info2.timestamp = -2;
    ASSERT_NE(info, info2);
    info2.timestamp = 2;
    ASSERT_EQ(info, info2);

    info2.messageType = MESSAGE_TYPE_SYSTEM;
    ASSERT_NE(info, info2);
    info2.messageType = MESSAGE_TYPE_PORTFOLIO;
    ASSERT_EQ(info, info2);

    info2.text = "b";
    ASSERT_NE(info, info2);
    info2.text = "a";
    ASSERT_EQ(info, info2);
}
