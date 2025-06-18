#include "src/domain/log/logentry.h"



const char* const LOG_LEVEL_NAMES[LOG_LEVEL_COUNT] = {
    "VERBOSE",
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
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

void LogEntry::fromJsonObject(simdjson::ondemand::object jsonObject)
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        std::string_view key = field.escaped_key();

        if (key == "timestamp")
        {
            timestamp = field.value().get_int64();
        }
        else if (key == "level")
        {
            level = static_cast<LogLevel>(field.value().get_int64().value());
        }
        else if (key == "instrumentId")
        {
            std::string_view value = field.value().get_string();
            instrumentId           = QString::fromUtf8(value.data(), value.size());
        }
        else if (key == "instrumentTicker")
        {
            std::string_view value = field.value().get_string();
            instrumentTicker       = QString::fromUtf8(value.data(), value.size());
        }
        else if (key == "instrumentName")
        {
            std::string_view value = field.value().get_string();
            instrumentName         = QString::fromUtf8(value.data(), value.size());
        }
        else if (key == "message")
        {
            std::string_view value = field.value().get_string();
            message                = QString::fromUtf8(value.data(), value.size());
        }
    }
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
