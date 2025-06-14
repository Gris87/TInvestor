#pragma once



#include "src/domain/log/logentry.h"



class LogsTableTimeLessThan
{
public:
    explicit LogsTableTimeLessThan()
    {
    }

    ~LogsTableTimeLessThan()
    {
    }

    LogsTableTimeLessThan(const LogsTableTimeLessThan& another)            = delete;
    LogsTableTimeLessThan& operator=(const LogsTableTimeLessThan& another) = delete;

    inline bool operator()(const LogEntry& l, const LogEntry& r) const
    {
        return l.timestamp < r.timestamp;
    }
};

class LogsTableLevelLessThan
{
public:
    explicit LogsTableLevelLessThan()
    {
    }

    ~LogsTableLevelLessThan()
    {
    }

    LogsTableLevelLessThan(const LogsTableLevelLessThan& another)            = delete;
    LogsTableLevelLessThan& operator=(const LogsTableLevelLessThan& another) = delete;

    inline bool operator()(const LogEntry& l, const LogEntry& r) const
    {
        return l.level < r.level;
    }
};

class LogsTableNameLessThan
{
public:
    explicit LogsTableNameLessThan()
    {
    }

    ~LogsTableNameLessThan()
    {
    }

    LogsTableNameLessThan(const LogsTableNameLessThan& another)            = delete;
    LogsTableNameLessThan& operator=(const LogsTableNameLessThan& another) = delete;

    inline bool operator()(const LogEntry& l, const LogEntry& r) const
    {
        return l.instrumentTicker < r.instrumentTicker;
    }
};

class LogsTableMessageLessThan
{
public:
    explicit LogsTableMessageLessThan()
    {
    }

    ~LogsTableMessageLessThan()
    {
    }

    LogsTableMessageLessThan(const LogsTableMessageLessThan& another)            = delete;
    LogsTableMessageLessThan& operator=(const LogsTableMessageLessThan& another) = delete;

    inline bool operator()(const LogEntry& l, const LogEntry& r) const
    {
        return l.message < r.message;
    }
};

class LogsTableTimeGreaterThan
{
public:
    explicit LogsTableTimeGreaterThan()
    {
    }

    ~LogsTableTimeGreaterThan()
    {
    }

    LogsTableTimeGreaterThan(const LogsTableTimeGreaterThan& another)            = delete;
    LogsTableTimeGreaterThan& operator=(const LogsTableTimeGreaterThan& another) = delete;

    inline bool operator()(const LogEntry& l, const LogEntry& r) const
    {
        return l.timestamp > r.timestamp;
    }
};

class LogsTableLevelGreaterThan
{
public:
    explicit LogsTableLevelGreaterThan()
    {
    }

    ~LogsTableLevelGreaterThan()
    {
    }

    LogsTableLevelGreaterThan(const LogsTableLevelGreaterThan& another)            = delete;
    LogsTableLevelGreaterThan& operator=(const LogsTableLevelGreaterThan& another) = delete;

    inline bool operator()(const LogEntry& l, const LogEntry& r) const
    {
        return l.level > r.level;
    }
};

class LogsTableNameGreaterThan
{
public:
    explicit LogsTableNameGreaterThan()
    {
    }

    ~LogsTableNameGreaterThan()
    {
    }

    LogsTableNameGreaterThan(const LogsTableNameGreaterThan& another)            = delete;
    LogsTableNameGreaterThan& operator=(const LogsTableNameGreaterThan& another) = delete;

    inline bool operator()(const LogEntry& l, const LogEntry& r) const
    {
        return l.instrumentTicker > r.instrumentTicker;
    }
};

class LogsTableMessageGreaterThan
{
public:
    explicit LogsTableMessageGreaterThan()
    {
    }

    ~LogsTableMessageGreaterThan()
    {
    }

    LogsTableMessageGreaterThan(const LogsTableMessageGreaterThan& another)            = delete;
    LogsTableMessageGreaterThan& operator=(const LogsTableMessageGreaterThan& another) = delete;

    inline bool operator()(const LogEntry& l, const LogEntry& r) const
    {
        return l.message > r.message;
    }
};
