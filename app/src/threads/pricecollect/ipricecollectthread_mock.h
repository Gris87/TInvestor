#pragma once



#include "src/threads/pricecollect/ipricecollectthread.h"

#include <gmock/gmock.h>



class PriceCollectThreadMock : public IPriceCollectThread
{
    Q_OBJECT

public:
    explicit PriceCollectThreadMock(QObject* parent = nullptr) :
        IPriceCollectThread(parent)
    {
    }
    ~PriceCollectThreadMock() override = default;

    PriceCollectThreadMock(const PriceCollectThreadMock& another)            = delete;
    PriceCollectThreadMock& operator=(const PriceCollectThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
};
