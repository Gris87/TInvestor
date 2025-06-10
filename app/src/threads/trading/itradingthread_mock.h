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
    MOCK_METHOD(void, setExpectedCost, (double expectedCost, const QString& cause), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
