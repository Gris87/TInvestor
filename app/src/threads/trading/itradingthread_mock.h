#pragma once



#include "src/threads/trading/itradingthread.h"

#include <gmock/gmock.h>



class TradingThreadMock : public ITradingThread
{
    Q_OBJECT

public:
    explicit TradingThreadMock(QObject* parent = nullptr) :
        ITradingThread(parent)
    {
    }
    ~TradingThreadMock() override = default;

    TradingThreadMock(const TradingThreadMock& another)            = delete;
    TradingThreadMock& operator=(const TradingThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, setAccountId, (const QString& accountId), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
