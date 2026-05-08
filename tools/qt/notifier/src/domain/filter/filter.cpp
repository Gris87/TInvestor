#include "src/domain/filter/filter.h"



Filter::Filter() :
    messageTypeMask((1ULL << MESSAGE_TYPE_COUNT) - 1)
{
}

void Filter::setMessageTypeMask(const QList<MessageType>& types)
{
    messageTypeMask = 1; // MESSAGE_TYPE_NONE always enabled

    for (const MessageType& messageType : types)
    {
        messageTypeMask |= 1ULL << messageType;
    }
}

bool Filter::isActive() const
{
    return messageTypeMask != (1ULL << MESSAGE_TYPE_COUNT) - 1;
}

bool Filter::isFiltered(const NotificationInfo& notification) const
{
    return ((messageTypeMask & (1ULL << notification.messageType)) != 0);
}

bool operator==(const Filter& lhs, const Filter& rhs)
{
    return lhs.messageTypeMask == rhs.messageTypeMask;
}
