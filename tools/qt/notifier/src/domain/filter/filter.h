#pragma once



#include "src/domain/notification/notificationinfo.h"



struct Filter
{
    explicit Filter();
    Filter(const Filter& another) = default;
    ~Filter()                     = default;

    Filter& operator=(const Filter& another) = default;

    void setMessageTypeMask(const QList<MessageType>& types);

    [[nodiscard]]
    bool isActive() const;

    [[nodiscard]]
    bool isFiltered(const NotificationInfo& notification) const;

    qint64 messageTypeMask;
};



bool operator==(const Filter& lhs, const Filter& rhs);
