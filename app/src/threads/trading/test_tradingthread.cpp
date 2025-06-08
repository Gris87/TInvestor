#include "src/threads/trading/tradingthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"



using ::testing::StrictMock;



class Test_TradingThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        grpcClientMock = new StrictMock<GrpcClientMock>();

        thread = new TradingThread(grpcClientMock, "aaaaa", "bbbbb", 1000.0);
    }

    void TearDown() override
    {
        delete thread;
        delete grpcClientMock;
    }

    TradingThread*              thread;
    StrictMock<GrpcClientMock>* grpcClientMock;
};



TEST_F(Test_TradingThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_TradingThread, Test_run)
{
    // thread->run();
}
