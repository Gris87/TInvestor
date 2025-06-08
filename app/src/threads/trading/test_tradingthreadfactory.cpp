#include "src/threads/trading/tradingthreadfactory.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"



using ::testing::StrictMock;



TEST(Test_TradingThreadFactory, Test_constructor_and_destructor)
{
    const TradingThreadFactory factory;
}

TEST(Test_TradingThreadFactory, Test_newInstance)
{
    const TradingThreadFactory factory;

    StrictMock<GrpcClientMock> grpcClientMock;

    const ITradingThread* thread = factory.newInstance(&grpcClientMock, "aaaaa", "bbbbb", 1000.0, nullptr);
    ASSERT_TRUE(thread != nullptr);

    delete thread;
}
