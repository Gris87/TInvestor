#pragma once



#include "src/threads/marketwaves/imarketwavesthread.h"

#include <gmock/gmock.h>



class MarketWavesThreadMock : public IMarketWavesThread
{
public:
    MarketWavesThreadMock() :
        IMarketWavesThread()
    {
    }
    ~MarketWavesThreadMock() override = default;

    MarketWavesThreadMock(const MarketWavesThreadMock& another)            = delete;
    MarketWavesThreadMock& operator=(const MarketWavesThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
};
