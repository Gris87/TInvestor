#pragma once



#include "src/domain/notification/notificationinfo.h"



class NotificationsTableTimeLessThan
{
public:
    explicit NotificationsTableTimeLessThan(QList<NotificationInfo>* _entries) :
        entries(_entries)
    {
    }

    QList<NotificationInfo>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).timestamp < entries->at(r).timestamp;
    }
};

class NotificationsTableTypeLessThan
{
public:
    explicit NotificationsTableTypeLessThan(QList<NotificationInfo>* _entries) :
        entries(_entries)
    {
    }

    QList<NotificationInfo>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).messageType < entries->at(r).messageType;
    }
};

class NotificationsTableTextLessThan
{
public:
    explicit NotificationsTableTextLessThan(QList<NotificationInfo>* _entries) :
        entries(_entries)
    {
    }

    QList<NotificationInfo>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).text < entries->at(r).text;
    }
};

class NotificationsTableTimeGreaterThan
{
public:
    explicit NotificationsTableTimeGreaterThan(QList<NotificationInfo>* _entries) :
        entries(_entries)
    {
    }

    QList<NotificationInfo>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).timestamp > entries->at(r).timestamp;
    }
};

class NotificationsTableTypeGreaterThan
{
public:
    explicit NotificationsTableTypeGreaterThan(QList<NotificationInfo>* _entries) :
        entries(_entries)
    {
    }

    QList<NotificationInfo>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).messageType > entries->at(r).messageType;
    }
};

class NotificationsTableTextGreaterThan
{
public:
    explicit NotificationsTableTextGreaterThan(QList<NotificationInfo>* _entries) :
        entries(_entries)
    {
    }

    QList<NotificationInfo>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).text > entries->at(r).text;
    }
};

inline bool notificationsTypeLess(const NotificationInfo& l, const NotificationInfo& r)
{
    return l.messageType < r.messageType;
}

inline bool notificationsTextLess(const NotificationInfo& l, const NotificationInfo& r)
{
    return l.text < r.text;
}

inline bool notificationsTypeGreater(const NotificationInfo& l, const NotificationInfo& r)
{
    return l.messageType > r.messageType;
}

inline bool notificationsTextGreater(const NotificationInfo& l, const NotificationInfo& r)
{
    return l.text > r.text;
}
