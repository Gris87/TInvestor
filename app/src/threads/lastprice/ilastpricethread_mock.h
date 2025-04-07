#pragma once



#include "src/threads/lastprice/ilastpricethread.h"

#include <gmock/gmock.h>



class LastPriceThreadMock : public ILastPriceThread
{
    Q_OBJECT

public:
    LastPriceThreadMock(QObject* parent = nullptr) :
        ILastPriceThread(parent)
    {
    }
    ~LastPriceThreadMock() override = default;

    LastPriceThreadMock(const LastPriceThreadMock& another)            = delete;
    LastPriceThreadMock& operator=(const LastPriceThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, stocksChanged, (), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
