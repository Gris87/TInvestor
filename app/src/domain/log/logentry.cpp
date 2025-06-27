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

static void logTimestampParse(LogEntry* entry, simdjson::ondemand::value value)
{
    entry->timestamp = value.get_int64();
}

static void logLevelParse(LogEntry* entry, simdjson::ondemand::value value)
{
    entry->level = static_cast<LogLevel>(value.get_int64().value());
}

static void logInstrumentIdParse(LogEntry* entry, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    entry->instrumentId             = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void logInstrumentTickerParse(LogEntry* entry, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    entry->instrumentTicker         = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void logInstrumentNameParse(LogEntry* entry, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    entry->instrumentName           = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void logMessageParse(LogEntry* entry, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    entry->message                  = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void logThrowParseException(
    LogEntry* /*entry*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throw std::runtime_error("Unknown parameter");
}

using ParseHandler = void (*)(LogEntry* entry, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"timestamp",        logTimestampParse       },
    {"level",            logLevelParse           },
    {"instrumentId",     logInstrumentIdParse    },
    {"instrumentTicker", logInstrumentTickerParse},
    {"instrumentName",   logInstrumentNameParse  },
    {"message",          logMessageParse         }
};
// clang-format on

void LogEntry::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, logThrowParseException);

        parseHandler(this, field.value());
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
