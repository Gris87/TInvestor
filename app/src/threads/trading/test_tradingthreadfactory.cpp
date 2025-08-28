#include "src/threads/trading/tradingthreadfactory.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/threads/logs/ilogsthread_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::InSequence;
using ::testing::StrictMock;



class Test_TradingThreadFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new TradingThreadFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    TradingThreadFactory* factory;
};



TEST_F(Test_TradingThreadFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_TradingThreadFactory, Test_newInstance)
{
    const InSequence seq;

    StrictMock<InstrumentsStorageMock> instrumentsStorageMock;
    StrictMock<TimeUtilsMock>          timeUtilsMock;
    StrictMock<GrpcClientMock>         grpcClientMock;
    StrictMock<LogsThreadMock>         logsThreadMock;

    EXPECT_CALL(logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("bbbbb"), QString("But why")));

    const ITradingThread* thread = factory->newInstance(
        &instrumentsStorageMock, &timeUtilsMock, &grpcClientMock, &logsThreadMock, "aaaaa", "bbbbb", 1000.0, "But why", nullptr
    );
    ASSERT_TRUE(thread != nullptr);

    delete thread;
}
