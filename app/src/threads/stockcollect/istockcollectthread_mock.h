#pragma once



#include "src/threads/stockcollect/istockcollectthread.h"

#include <gmock/gmock.h>



class StockCollectThreadMock : public IStockCollectThread
{
    Q_OBJECT

public:
    explicit StockCollectThreadMock(QObject* parent = nullptr) :
        IStockCollectThread(parent)
    {
    }
    ~StockCollectThreadMock() override = default;

    StockCollectThreadMock(const StockCollectThreadMock& another)            = delete;
    StockCollectThreadMock& operator=(const StockCollectThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
