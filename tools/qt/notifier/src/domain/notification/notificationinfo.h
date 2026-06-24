#pragma once



#include <QJsonObject>

#include "src/simdjson/simdjson_wrapped.h"



enum MessageType : quint8
{
    MESSAGE_TYPE_NONE,
    MESSAGE_TYPE_SYSTEM,
    MESSAGE_TYPE_PORTFOLIO,
    MESSAGE_TYPE_HUGE_SELL,
    MESSAGE_TYPE_DIVIDENDS,
    MESSAGE_TYPE_PULSE_NEUTRAL,
    MESSAGE_TYPE_PULSE_BUY,
    MESSAGE_TYPE_PULSE_SELL,
    MESSAGE_TYPE_COUNT
};



struct NotificationInfo
{
    explicit NotificationInfo();
    NotificationInfo(const NotificationInfo& another) = default;
    ~NotificationInfo()                               = default;

    NotificationInfo& operator=(const NotificationInfo& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    qint64      requestTimestamp;
    qint64      timestamp;
    MessageType messageType;
    QString     text;
    QString     data;
};



bool operator==(const NotificationInfo& lhs, const NotificationInfo& rhs);
