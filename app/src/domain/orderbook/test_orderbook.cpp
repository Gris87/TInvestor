#include "src/domain/orderbook/orderbook.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_OrderBook, Test_constructor_and_destructor)
{
    const OrderBook orderbook;

    // clang-format off
    ASSERT_EQ(orderbook.timestamp,   0);
    ASSERT_NEAR(orderbook.price,     0.0f, 0.0001f);
    ASSERT_EQ(orderbook.bids.size(), 0);
    ASSERT_EQ(orderbook.asks.size(), 0);
    // clang-format on
}

TEST(Test_OrderBook, Test_copy_constructor)
{
    OrderBook orderbook;

    OrderBookData orderbookData1;
    OrderBookData orderbookData2;

    orderbookData1.quantity = 3;
    orderbookData1.price    = 4.0f;
    orderbookData2.quantity = 5;
    orderbookData2.price    = 6.0f;

    orderbook.timestamp = 1;
    orderbook.price     = 2.0f;
    orderbook.bids.append(orderbookData1);
    orderbook.asks.append(orderbookData2);

    const OrderBook orderbook2(orderbook);

    // clang-format off
    ASSERT_EQ(orderbook.timestamp,           1);
    ASSERT_NEAR(orderbook.price,             2.0f, 0.0001f);
    ASSERT_EQ(orderbook.bids.size(),         1);
    ASSERT_EQ(orderbook.bids.at(0).quantity, 3);
    ASSERT_NEAR(orderbook.bids.at(0).price,  4.0f, 0.0001f);
    ASSERT_EQ(orderbook.asks.size(),         1);
    ASSERT_EQ(orderbook.asks.at(0).quantity, 5);
    ASSERT_NEAR(orderbook.asks.at(0).price,  6.0f, 0.0001f);
    // clang-format on
}

TEST(Test_OrderBook, Test_assign)
{
    OrderBook orderbook;
    OrderBook orderbook2;

    OrderBookData orderbookData1;
    OrderBookData orderbookData2;

    orderbookData1.quantity = 3;
    orderbookData1.price    = 4.0f;
    orderbookData2.quantity = 5;
    orderbookData2.price    = 6.0f;

    orderbook.timestamp = 1;
    orderbook.price     = 2.0f;
    orderbook.bids.append(orderbookData1);
    orderbook.asks.append(orderbookData2);

    orderbook2 = orderbook;

    // clang-format off
    ASSERT_EQ(orderbook.timestamp,           1);
    ASSERT_NEAR(orderbook.price,             2.0f, 0.0001f);
    ASSERT_EQ(orderbook.bids.size(),         1);
    ASSERT_EQ(orderbook.bids.at(0).quantity, 3);
    ASSERT_NEAR(orderbook.bids.at(0).price,  4.0f, 0.0001f);
    ASSERT_EQ(orderbook.asks.size(),         1);
    ASSERT_EQ(orderbook.asks.at(0).quantity, 5);
    ASSERT_NEAR(orderbook.asks.at(0).price,  6.0f, 0.0001f);
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
