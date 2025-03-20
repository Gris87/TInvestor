#pragma once



#include "src/threads/orderbook/iorderbookthread.h"



class OrderBookThread : public IOrderBookThread
{
    Q_OBJECT

public:
    explicit OrderBookThread(QObject* parent = nullptr);
    ~OrderBookThread();

    OrderBookThread(const OrderBookThread& another)            = delete;
    OrderBookThread& operator=(const OrderBookThread& another) = delete;

    void run() override;
};
