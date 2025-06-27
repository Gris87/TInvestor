#pragma once



#include "src/domain/log/logentry.h"



class ILogsDatabase
{
public:
    explicit ILogsDatabase() = default;
    virtual ~ILogsDatabase() = default;

    ILogsDatabase(const ILogsDatabase& another)            = delete;
    ILogsDatabase& operator=(const ILogsDatabase& another) = delete;

    virtual void            setAccount(const QString& account)  = 0;
    virtual QList<LogEntry> readLogs()                          = 0;
    virtual void            writeLogs(QList<LogEntry>& entries) = 0;
    virtual void            appendLog(const LogEntry& entry)    = 0;
};
