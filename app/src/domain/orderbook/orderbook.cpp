#include "src/domain/orderbook/orderbook.h"



OrderBook::OrderBook() :
    timestamp(),
    price(),
    bids(),
    asks()
{
}

OrderBook::OrderBook(const OrderBook& another) :
    timestamp(another.timestamp),
    price(another.price),
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
    price     = another.price;
    bids      = another.bids;
    asks      = another.asks;

    return *this;
}
