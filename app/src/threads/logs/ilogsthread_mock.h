#pragma once



#include "src/threads/logs/ilogsthread.h"

#include <gmock/gmock.h>



class LogsThreadMock : public ILogsThread
{
    Q_OBJECT

public:
    explicit LogsThreadMock(QObject* parent = nullptr) :
        ILogsThread(parent)
    {
    }
    ~LogsThreadMock() override = default;

    LogsThreadMock(const LogsThreadMock& another)            = delete;
    LogsThreadMock& operator=(const LogsThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, setAccountId, (const QString& account, const QString& accountId), (override));
    MOCK_METHOD(void, addLog, (LogLevel level, const QString& instrumentId, const QString& message), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
