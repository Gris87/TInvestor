#pragma once



#include "src/db/logs/ilogsdatabase.h"

#include <gmock/gmock.h>



class LogsDatabaseMock : public ILogsDatabase
{
public:
    explicit LogsDatabaseMock() :
        ILogsDatabase()
    {
    }
    ~LogsDatabaseMock() override = default;

    LogsDatabaseMock(const LogsDatabaseMock& another)            = delete;
    LogsDatabaseMock& operator=(const LogsDatabaseMock& another) = delete;

    MOCK_METHOD(void, setAccount, (const QString& account), (override));
    MOCK_METHOD(QList<LogEntry>, readLogs, (int partId), (override));
    MOCK_METHOD(void, writeLogs, (QList<LogEntry> & entries, int partId), (override));
    MOCK_METHOD(void, appendLog, (const LogEntry& entry, int partId), (override));
    MOCK_METHOD(void, deleteLogs, (int partId), (override));
};
