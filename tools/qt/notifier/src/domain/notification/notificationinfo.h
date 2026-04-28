#pragma once



#include <QString>



enum MessageType : quint8
{
    MESSAGE_TYPE_NONE,
    MESSAGE_TYPE_SYSTEM,
    MESSAGE_TYPE_PORTFOLIO,
    MESSAGE_TYPE_HUGE_SELL,
    MESSAGE_TYPE_DIVIDENDS,
    MESSAGE_TYPE_PULSE
};



struct NotificationInfo
{
    explicit NotificationInfo();
    explicit NotificationInfo(qint64 _timestamp, MessageType _messageType, const QString& _text);
    NotificationInfo(const NotificationInfo& another) = default;
    ~NotificationInfo()                               = default;

    NotificationInfo& operator=(const NotificationInfo& another) = default;

    qint64      timestamp;
    MessageType messageType;
    QString     text;
};
