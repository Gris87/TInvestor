#include "src/threads/orderbook/orderbookthread.h"

#include <gtest/gtest.h>



class Test_OrderBookThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        thread = new OrderBookThread();
    }

    void TearDown()
    {
        delete thread;
    }

    OrderBookThread* thread;
};



TEST_F(Test_OrderBookThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_OrderBookThread, Test_run)
{
    thread->run();
}
