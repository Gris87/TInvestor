#pragma once



#include <QList>

#include "src/domain/orderbook/orderbookdata.h"



struct OrderBook
{
    OrderBook();
    OrderBook(const OrderBook& another) = default;
    ~OrderBook()                        = default;

    OrderBook& operator=(const OrderBook& another) = default;

    qint64               timestamp;
    float                price;
    QList<OrderBookData> bids;
    QList<OrderBookData> asks;
};
