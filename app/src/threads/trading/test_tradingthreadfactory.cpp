#include "src/threads/trading/tradingthreadfactory.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/threads/logs/ilogsthread_mock.h"



using ::testing::StrictMock;



TEST(Test_TradingThreadFactory, Test_constructor_and_destructor)
{
    const TradingThreadFactory factory;
}

TEST(Test_TradingThreadFactory, Test_newInstance)
{
    const TradingThreadFactory factory;

    StrictMock<InstrumentsStorageMock> instrumentsStorageMock;
    StrictMock<GrpcClientMock>         grpcClientMock;
    StrictMock<LogsThreadMock>         logsThreadMock;

    const ITradingThread* thread =
        factory.newInstance(&instrumentsStorageMock, &grpcClientMock, &logsThreadMock, "aaaaa", "bbbbb", 1000.0, nullptr);
    ASSERT_TRUE(thread != nullptr);

    delete thread;
}
