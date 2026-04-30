#include "src/domain/filter/filter.h"



Filter::Filter() :
    messageTypeMask((1LL << MESSAGE_TYPE_COUNT) - 1)
{
}

void Filter::setMessageTypeMask(const QList<MessageType>& types)
{
    messageTypeMask = 0;

    for (MessageType messageType : types)
    {
        messageTypeMask |= 1LL << messageType;
    }
}

bool Filter::isActive() const
{
    return messageTypeMask != (1LL << MESSAGE_TYPE_COUNT) - 1;
}

bool Filter::isFiltered(const NotificationInfo& notification) const
{
    if ((messageTypeMask & (1LL << notification.messageType)) == 0)
    {
        return false;
    }

    return true;
}

bool operator==(const Filter& lhs, const Filter& rhs)
{
    return lhs.messageTypeMask == rhs.messageTypeMask;
}
