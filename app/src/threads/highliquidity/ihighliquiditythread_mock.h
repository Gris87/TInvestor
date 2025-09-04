#pragma once



#include "src/threads/highliquidity/ihighliquiditythread.h"

#include <gmock/gmock.h>



class HighLiquidityThreadMock : public IHighLiquidityThread
{
    Q_OBJECT

public:
    explicit HighLiquidityThreadMock(QObject* parent = nullptr) :
        IHighLiquidityThread(parent)
    {
    }
    ~HighLiquidityThreadMock() override = default;

    HighLiquidityThreadMock(const HighLiquidityThreadMock& another)            = delete;
    HighLiquidityThreadMock& operator=(const HighLiquidityThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, setAccountId, (const QString& accountId), (override));
    MOCK_METHOD(void, setKeepMoney, (int value), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
