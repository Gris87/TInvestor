#include "src/threads/orderbook/orderbookthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_OrderBookThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        grpcClientMock = new StrictMock<GrpcClientMock>();

        thread = new OrderBookThread(grpcClientMock);
    }

    void TearDown()
    {
        delete thread;
        delete grpcClientMock;
    }

    OrderBookThread*            thread;
    StrictMock<GrpcClientMock>* grpcClientMock;
};



TEST_F(Test_OrderBookThread, Test_constructor_and_destructor)
{
}
