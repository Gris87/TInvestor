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
    MOCK_METHOD(QList<LogEntry>, readLogs, (), (override));
    MOCK_METHOD(void, writeLogs, (QList<LogEntry> & entries), (override));
    MOCK_METHOD(void, appendLog, (const LogEntry& entry), (override));
};
