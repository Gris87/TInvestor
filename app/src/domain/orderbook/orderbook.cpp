#include "src/domain/orderbook/orderbook.h"



OrderBook::OrderBook() :
    timestamp(),
    bids(),
    asks()
{
}

OrderBook::OrderBook(const OrderBook& another) :
    timestamp(another.timestamp),
    bids(another.bids),
    asks(another.asks)
{
}

OrderBook::~OrderBook()
{
}

OrderBook& OrderBook::operator=(const OrderBook& another)
{
    timestamp = another.timestamp;
    bids      = another.bids;
    asks      = another.asks;

    return *this;
}
