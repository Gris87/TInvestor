#pragma once



#include "src/domain/log/logentry.h"



class LogsTableTimeLessThan
{
public:
    explicit LogsTableTimeLessThan(QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<LogEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).timestamp < entries->at(r).timestamp;
    }
};

class LogsTableLevelLessThan
{
public:
    explicit LogsTableLevelLessThan(QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<LogEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).level < entries->at(r).level;
    }
};

class LogsTableNameLessThan
{
public:
    explicit LogsTableNameLessThan(QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<LogEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).instrumentTicker < entries->at(r).instrumentTicker;
    }
};

class LogsTableMessageLessThan
{
public:
    explicit LogsTableMessageLessThan(QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<LogEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).message < entries->at(r).message;
    }
};

class LogsTableTimeGreaterThan
{
public:
    explicit LogsTableTimeGreaterThan(QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<LogEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).timestamp > entries->at(r).timestamp;
    }
};

class LogsTableLevelGreaterThan
{
public:
    explicit LogsTableLevelGreaterThan(QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<LogEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).level > entries->at(r).level;
    }
};

class LogsTableNameGreaterThan
{
public:
    explicit LogsTableNameGreaterThan(QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<LogEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).instrumentTicker > entries->at(r).instrumentTicker;
    }
};

class LogsTableMessageGreaterThan
{
public:
    explicit LogsTableMessageGreaterThan(QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<LogEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).message > entries->at(r).message;
    }
};

inline bool logsTimeLess(const LogEntry& l, const LogEntry& r)
{
    return l.timestamp < r.timestamp;
}

inline bool logsLevelLess(const LogEntry& l, const LogEntry& r)
{
    return l.level < r.level;
}

inline bool logsNameLess(const LogEntry& l, const LogEntry& r)
{
    return l.instrumentTicker < r.instrumentTicker;
}

inline bool logsMessageLess(const LogEntry& l, const LogEntry& r)
{
    return l.message < r.message;
}

inline bool logsTimeGreater(const LogEntry& l, const LogEntry& r)
{
    return l.timestamp > r.timestamp;
}

inline bool logsLevelGreater(const LogEntry& l, const LogEntry& r)
{
    return l.level > r.level;
}

inline bool logsNameGreater(const LogEntry& l, const LogEntry& r)
{
    return l.instrumentTicker > r.instrumentTicker;
}

inline bool logsMessageGreater(const LogEntry& l, const LogEntry& r)
{
    return l.message > r.message;
}
