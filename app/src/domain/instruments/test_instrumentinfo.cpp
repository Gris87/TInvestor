#include "src/domain/instruments/instrumentinfo.h"

#include <QJsonDocument>
#include <gtest/gtest.h>



TEST(Test_InstrumentInfo, Test_constructor_and_destructor)
{
    const InstrumentInfo info;

    // clang-format off
    ASSERT_EQ(info.ticker, "");
    ASSERT_EQ(info.name,   "");
    // clang-format on
}

TEST(Test_InstrumentInfo, Test_copy_constructor)
{
    InstrumentInfo info;

    info.ticker = "a";
    info.name   = "b";

    const InstrumentInfo info2(info);

    // clang-format off
    ASSERT_EQ(info2.ticker, "a");
    ASSERT_EQ(info2.name,   "b");
    // clang-format on
}

TEST(Test_InstrumentInfo, Test_assign)
{
    InstrumentInfo info;
    InstrumentInfo info2;

    info.ticker = "a";
    info.name   = "b";

    info2 = info;

    // clang-format off
    ASSERT_EQ(info2.ticker, "a");
    ASSERT_EQ(info2.name,   "b");
    // clang-format on
}

TEST(Test_InstrumentInfo, Test_fromJsonObject)
{
    InstrumentInfo info;

    // clang-format off
    ASSERT_EQ(info.ticker, "");
    ASSERT_EQ(info.name,   "");
    // clang-format on

    const QString content = R"({"name":"b","ticker":"a"})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    info.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(info.ticker, "a");
    ASSERT_EQ(info.name,   "b");
    // clang-format on
}

TEST(Test_InstrumentInfo, Test_toJsonObject)
{
    InstrumentInfo info;

    info.ticker = "a";
    info.name   = "b";

    const QJsonObject   jsonObject = info.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"name":"b","ticker":"a"})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_InstrumentInfo, Test_equals)
{
    InstrumentInfo info;
    InstrumentInfo info2;

    info.ticker = "a";
    info.name   = "b";

    info2.ticker = "a";
    info2.name   = "b";

    ASSERT_EQ(info, info2);

    info2.ticker = "aaaa";
    ASSERT_NE(info, info2);
    info2.ticker = "a";
    ASSERT_EQ(info, info2);

    info2.name = "bbbb";
    ASSERT_NE(info, info2);
    info2.name = "b";
    ASSERT_EQ(info, info2);
}
