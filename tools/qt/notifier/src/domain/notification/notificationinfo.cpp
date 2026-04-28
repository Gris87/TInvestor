#include "src/domain/notification/notificationinfo.h"



NotificationInfo::NotificationInfo() :
    timestamp(),
    messageType(),
    text()
{
}

NotificationInfo::NotificationInfo(qint64 _timestamp, MessageType _messageType, const QString& _text) :
    timestamp(_timestamp),
    messageType(_messageType),
    text(_text)
{
}
