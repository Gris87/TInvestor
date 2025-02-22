#pragma once



#include "src/threads/pricecollect/ipricecollectthread.h"

#include <gmock/gmock.h>



class PriceCollectThreadMock : public IPriceCollectThread
{
public:
    PriceCollectThreadMock() :
        IPriceCollectThread()
    {
    }
    ~PriceCollectThreadMock() override = default;

    PriceCollectThreadMock(const PriceCollectThreadMock& another)            = delete;
    PriceCollectThreadMock& operator=(const PriceCollectThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
};
