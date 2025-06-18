#include "src/domain/log/logentry.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_LogEntry, Test_constructor_and_destructor)
{
    const LogEntry entry;

    // clang-format off
    ASSERT_EQ(entry.timestamp,        0);
    ASSERT_EQ(entry.level,            LOG_LEVEL_VERBOSE);
    ASSERT_EQ(entry.instrumentId,     "");
    ASSERT_EQ(entry.instrumentLogo,   nullptr);
    ASSERT_EQ(entry.instrumentTicker, "");
    ASSERT_EQ(entry.instrumentName,   "");
    ASSERT_EQ(entry.message,          "");
    // clang-format on
}

TEST(Test_LogEntry, Test_copy_constructor)
{
    LogEntry entry;

    entry.timestamp        = 1;
    entry.level            = LOG_LEVEL_INFO;
    entry.instrumentId     = "a";
    entry.instrumentTicker = "b";
    entry.instrumentName   = "c";
    entry.message          = "d";

    const LogEntry entry2(entry);

    // clang-format off
    ASSERT_EQ(entry2.timestamp,        1);
    ASSERT_EQ(entry2.level,            LOG_LEVEL_INFO);
    ASSERT_EQ(entry2.instrumentId,     "a");
    ASSERT_EQ(entry2.instrumentLogo,   nullptr);
    ASSERT_EQ(entry2.instrumentTicker, "b");
    ASSERT_EQ(entry2.instrumentName,   "c");
    ASSERT_EQ(entry2.message,          "d");
    // clang-format on
}

TEST(Test_LogEntry, Test_assign)
{
    LogEntry entry;
    LogEntry entry2;

    entry.timestamp        = 1;
    entry.level            = LOG_LEVEL_INFO;
    entry.instrumentId     = "a";
    entry.instrumentTicker = "b";
    entry.instrumentName   = "c";
    entry.message          = "d";

    entry2 = entry;

    // clang-format off
    ASSERT_EQ(entry2.timestamp,        1);
    ASSERT_EQ(entry2.level,            LOG_LEVEL_INFO);
    ASSERT_EQ(entry2.instrumentId,     "a");
    ASSERT_EQ(entry2.instrumentLogo,   nullptr);
    ASSERT_EQ(entry2.instrumentTicker, "b");
    ASSERT_EQ(entry2.instrumentName,   "c");
    ASSERT_EQ(entry2.message,          "d");
    // clang-format on
}

TEST(Test_LogEntry, Test_fromJsonObject)
{
    LogEntry entry;

    // clang-format off
    ASSERT_EQ(entry.timestamp,        0);
    ASSERT_EQ(entry.level,            LOG_LEVEL_VERBOSE);
    ASSERT_EQ(entry.instrumentId,     "");
    ASSERT_EQ(entry.instrumentLogo,   nullptr);
    ASSERT_EQ(entry.instrumentTicker, "");
    ASSERT_EQ(entry.instrumentName,   "");
    ASSERT_EQ(entry.message,          "");
    // clang-format on

    const QString content =
        R"({"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","level":2,"message":"d","timestamp":1})";

    simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    entry.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(entry.timestamp,        1);
    ASSERT_EQ(entry.level,            LOG_LEVEL_INFO);
    ASSERT_EQ(entry.instrumentId,     "a");
    ASSERT_EQ(entry.instrumentLogo,   nullptr);
    ASSERT_EQ(entry.instrumentTicker, "b");
    ASSERT_EQ(entry.instrumentName,   "c");
    ASSERT_EQ(entry.message,          "d");
    // clang-format on
}

TEST(Test_LogEntry, Test_toJsonObject)
{
    LogEntry entry;

    entry.timestamp        = 1;
    entry.level            = LOG_LEVEL_INFO;
    entry.instrumentId     = "a";
    entry.instrumentTicker = "b";
    entry.instrumentName   = "c";
    entry.message          = "d";

    const QJsonObject   jsonObject = entry.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","level":2,"message":"d","timestamp":1})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_LogEntry, Test_equals)
{
    LogEntry entry;
    LogEntry entry2;

    entry.timestamp        = 1;
    entry.level            = LOG_LEVEL_INFO;
    entry.instrumentId     = "a";
    entry.instrumentTicker = "b";
    entry.instrumentName   = "c";
    entry.message          = "d";

    entry2.timestamp        = 1;
    entry2.level            = LOG_LEVEL_INFO;
    entry2.instrumentId     = "a";
    entry2.instrumentTicker = "b";
    entry2.instrumentName   = "c";
    entry2.message          = "d";

    ASSERT_EQ(entry, entry2);

    entry2.timestamp = 1000;
    ASSERT_NE(entry, entry2);
    entry2.timestamp = 1;
    ASSERT_EQ(entry, entry2);

    entry2.level = LOG_LEVEL_ERROR;
    ASSERT_NE(entry, entry2);
    entry2.level = LOG_LEVEL_INFO;
    ASSERT_EQ(entry, entry2);

    entry2.instrumentId = "aaaa";
    ASSERT_NE(entry, entry2);
    entry2.instrumentId = "a";
    ASSERT_EQ(entry, entry2);

    entry2.instrumentTicker = "bbbb";
    ASSERT_NE(entry, entry2);
    entry2.instrumentTicker = "b";
    ASSERT_EQ(entry, entry2);

    entry2.instrumentName = "cccc";
    ASSERT_NE(entry, entry2);
    entry2.instrumentName = "c";
    ASSERT_EQ(entry, entry2);

    entry2.message = "dddd";
    ASSERT_NE(entry, entry2);
    entry2.message = "d";
    ASSERT_EQ(entry, entry2);
}
// NOLINTEND(readability-magic-numbers)
