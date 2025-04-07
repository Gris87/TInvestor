#pragma once



#include "src/threads/orderbook/iorderbookthread.h"

#include <gmock/gmock.h>



class OrderBookThreadMock : public IOrderBookThread
{
    Q_OBJECT

public:
    OrderBookThreadMock(QObject* parent = nullptr) :
        IOrderBookThread(parent)
    {
    }
    ~OrderBookThreadMock() override = default;

    OrderBookThreadMock(const OrderBookThreadMock& another)            = delete;
    OrderBookThreadMock& operator=(const OrderBookThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, setStock, (Stock * stock), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
