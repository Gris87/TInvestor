#pragma once



#include <QList>

#include "src/domain/orderbook/orderbookdata.h"



struct OrderBook
{
    OrderBook();
    OrderBook(const OrderBook& another);
    ~OrderBook();

    OrderBook& operator=(const OrderBook& another);

    qint64               timestamp;
    QList<OrderBookData> bids;
    QList<OrderBookData> asks;
};
