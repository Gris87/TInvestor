#include "src/domain/log/logentry.h"



const char* const LOG_LEVEL_NAMES[LOG_LEVEL_COUNT] = {
    "VERBOSE",
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
};

const char* const LOG_LEVEL_NAMES_LOWERCASE[LOG_LEVEL_COUNT] = {
    "verbose",
    "debug",
    "info",
    "warning",
    "error",
};



LogEntry::LogEntry() :
    timestamp(),
    level(),
    instrumentId(),
    instrumentLogo(),
    instrumentTicker(),
    instrumentName(),
    message()
{
}

void LogEntry::fromJsonObject(const QJsonObject& jsonObject)
{
    timestamp        = jsonObject.value("timestamp").toInteger();
    level            = static_cast<LogLevel>(jsonObject.value("level").toInt());
    instrumentId     = jsonObject.value("instrumentId").toString();
    instrumentTicker = jsonObject.value("instrumentTicker").toString();
    instrumentName   = jsonObject.value("instrumentName").toString();
    message          = jsonObject.value("message").toString();
}

QJsonObject LogEntry::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("timestamp",        timestamp);
    res.insert("level",            level);
    res.insert("instrumentId",     instrumentId);
    res.insert("instrumentTicker", instrumentTicker);
    res.insert("instrumentName",   instrumentName);
    res.insert("message",          message);
    // clang-format on

    return res;
}

bool operator==(const LogEntry& lhs, const LogEntry& rhs)
{
    return lhs.timestamp == rhs.timestamp && lhs.level == rhs.level && lhs.instrumentId == rhs.instrumentId &&
           lhs.instrumentTicker == rhs.instrumentTicker && lhs.instrumentName == rhs.instrumentName && lhs.message == rhs.message;
}
