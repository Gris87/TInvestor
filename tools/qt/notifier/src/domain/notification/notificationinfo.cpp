#include "src/domain/notification/notificationinfo.h"

#include "src/utils/exception/exception.h"



// clang-format off
static const QMap<QString, MessageType> STRING_TO_MESSAGE_TYPE{ // clazy:exclude=non-pod-global-static
    {"system",        MESSAGE_TYPE_SYSTEM},
    {"portfolio",     MESSAGE_TYPE_PORTFOLIO},
    {"huge_sell",     MESSAGE_TYPE_HUGE_SELL},
    {"dividends",     MESSAGE_TYPE_DIVIDENDS},
    {"pulse_neutral", MESSAGE_TYPE_PULSE_NEUTRAL},
    {"pulse_buy",     MESSAGE_TYPE_PULSE_BUY},
    {"pulse_sell",    MESSAGE_TYPE_PULSE_SELL}
};

static const QMap<MessageType, QString> MESSAGE_TYPE_TO_STRING{ // clazy:exclude=non-pod-global-static
    {MESSAGE_TYPE_SYSTEM,        "system"},
    {MESSAGE_TYPE_PORTFOLIO,     "portfolio"},
    {MESSAGE_TYPE_HUGE_SELL,     "huge_sell"},
    {MESSAGE_TYPE_DIVIDENDS,     "dividends"},
    {MESSAGE_TYPE_PULSE_NEUTRAL, "pulse_neutral"},
    {MESSAGE_TYPE_PULSE_BUY,     "pulse_buy"},
    {MESSAGE_TYPE_PULSE_SELL,    "pulse_sell"}
};
// clang-format on



NotificationInfo::NotificationInfo() :
    requestTimestamp(),
    timestamp(),
    messageType(),
    text()
{
}

static void notificationInfoRequestTimestampParse(NotificationInfo* notificationInfo, simdjson::ondemand::value value)
{
    notificationInfo->requestTimestamp = value.get_int64();
}

static void notificationInfoTimestampParse(NotificationInfo* notificationInfo, simdjson::ondemand::value value)
{
    notificationInfo->timestamp = value.get_int64();
}

static void notificationInfoTypeParse(NotificationInfo* notificationInfo, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    const QString          typeStr  = QString::fromUtf8(valueStr.data(), valueStr.size());

    notificationInfo->messageType = STRING_TO_MESSAGE_TYPE.value(typeStr, MESSAGE_TYPE_NONE);
}

static void notificationInfoTextParse(NotificationInfo* notificationInfo, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    notificationInfo->text          = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void notificationInfoThrowParseException(
    NotificationInfo* /*notificationInfo*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(NotificationInfo* notificationInfo, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"requestTimestamp", notificationInfoRequestTimestampParse},
    {"timestamp",        notificationInfoTimestampParse       },
    {"type",             notificationInfoTypeParse            },
    {"text",             notificationInfoTextParse            }
};
// clang-format on

void NotificationInfo::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, notificationInfoThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject NotificationInfo::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("requestTimestamp", requestTimestamp);
    res.insert("timestamp",        timestamp);
    res.insert("type",             MESSAGE_TYPE_TO_STRING.value(messageType));
    res.insert("text",             text);
    // clang-format on

    return res;
}

bool operator==(const NotificationInfo& lhs, const NotificationInfo& rhs)
{
    return lhs.requestTimestamp == rhs.requestTimestamp && lhs.timestamp == rhs.timestamp && lhs.messageType == rhs.messageType &&
           lhs.text == rhs.text;
}
