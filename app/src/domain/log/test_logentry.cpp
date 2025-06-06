#include "src/domain/log/logentry.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_LogEntry, Test_constructor_and_destructor)
{
    const LogEntry entry;

    // clang-format off
    ASSERT_EQ(entry.timestamp, 0);
    ASSERT_EQ(entry.level,     LOG_LEVEL_VERBOSE);
    ASSERT_EQ(entry.message,   "");
    // clang-format on
}

TEST(Test_LogEntry, Test_copy_constructor)
{
    LogEntry entry;

    entry.timestamp = 1;
    entry.level     = LOG_LEVEL_INFO;
    entry.message   = "a";

    const LogEntry entry2(entry);

    // clang-format off
    ASSERT_EQ(entry2.timestamp, 1);
    ASSERT_EQ(entry2.level,     LOG_LEVEL_INFO);
    ASSERT_EQ(entry2.message,   "a");
    // clang-format on
}

TEST(Test_LogEntry, Test_assign)
{
    LogEntry entry;
    LogEntry entry2;

    entry.timestamp = 1;
    entry.level     = LOG_LEVEL_INFO;
    entry.message   = "a";

    entry2 = entry;

    // clang-format off
    ASSERT_EQ(entry2.timestamp, 1);
    ASSERT_EQ(entry2.level,     LOG_LEVEL_INFO);
    ASSERT_EQ(entry2.message,   "a");
    // clang-format on
}

TEST(Test_LogEntry, Test_fromJsonObject)
{
    LogEntry entry;

    // clang-format off
    ASSERT_EQ(entry.timestamp, 0);
    ASSERT_EQ(entry.level,     LOG_LEVEL_VERBOSE);
    ASSERT_EQ(entry.message,   "");
    // clang-format on

    const QString content = R"({"level":2,"message":"a","timestamp":1})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    entry.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(entry.timestamp, 1);
    ASSERT_EQ(entry.level,     LOG_LEVEL_INFO);
    ASSERT_EQ(entry.message,   "a");
    // clang-format on
}

TEST(Test_LogEntry, Test_toJsonObject)
{
    LogEntry entry;

    entry.timestamp = 1;
    entry.level     = LOG_LEVEL_INFO;
    entry.message   = "a";

    const QJsonObject   jsonObject = entry.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content         = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent = R"({"level":2,"message":"a","timestamp":1})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_LogEntry, Test_equals)
{
    LogEntry entry;
    LogEntry entry2;

    entry.timestamp = 1;
    entry.level     = LOG_LEVEL_INFO;
    entry.message   = "a";

    entry2.timestamp = 1;
    entry2.level     = LOG_LEVEL_INFO;
    entry2.message   = "a";

    ASSERT_EQ(entry, entry2);

    entry2.timestamp = 1000;
    ASSERT_NE(entry, entry2);
    entry2.timestamp = 1;
    ASSERT_EQ(entry, entry2);

    entry2.level = LOG_LEVEL_ERROR;
    ASSERT_NE(entry, entry2);
    entry2.level = LOG_LEVEL_INFO;
    ASSERT_EQ(entry, entry2);

    entry2.message = "aaaa";
    ASSERT_NE(entry, entry2);
    entry2.message = "a";
    ASSERT_EQ(entry, entry2);
}
// NOLINTEND(readability-magic-numbers)
