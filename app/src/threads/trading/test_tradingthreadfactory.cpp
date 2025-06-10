#include "src/threads/trading/tradingthreadfactory.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/threads/logs/ilogsthread_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::InSequence;
using ::testing::StrictMock;



TEST(Test_TradingThreadFactory, Test_constructor_and_destructor)
{
    const TradingThreadFactory factory;
}

TEST(Test_TradingThreadFactory, Test_newInstance)
{
    const InSequence seq;

    const TradingThreadFactory factory;

    StrictMock<InstrumentsStorageMock> instrumentsStorageMock;
    StrictMock<GrpcClientMock>         grpcClientMock;
    StrictMock<LogsThreadMock>         logsThreadMock;
    StrictMock<TimeUtilsMock>          timeUtilsMock;

    EXPECT_CALL(logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("bbbbb"), QString("But why")));

    const ITradingThread* thread = factory.newInstance(
        &instrumentsStorageMock, &grpcClientMock, &logsThreadMock, &timeUtilsMock, "aaaaa", "bbbbb", 1000.0, "But why", nullptr
    );
    ASSERT_TRUE(thread != nullptr);

    delete thread;
}
