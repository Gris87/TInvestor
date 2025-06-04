#include "src/domain/log/logentry.h"



LogEntry::LogEntry() :
    timestamp(),
    level(),
    message()
{
}

void LogEntry::fromJsonObject(const QJsonObject& jsonObject)
{
    timestamp = jsonObject.value("timestamp").toInteger();
    level     = static_cast<LogLevel>(jsonObject.value("level").toInt());
    message   = jsonObject.value("message").toString();
}

QJsonObject LogEntry::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("timestamp", timestamp);
    res.insert("level",     level);
    res.insert("message",   message);
    // clang-format on

    return res;
}

bool operator==(const LogEntry& lhs, const LogEntry& rhs)
{
    return lhs.timestamp == rhs.timestamp && lhs.level == rhs.level && lhs.message == rhs.message;
}
