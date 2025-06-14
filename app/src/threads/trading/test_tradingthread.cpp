#include "src/threads/trading/tradingthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/threads/logs/ilogsthread_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::InSequence;
using ::testing::StrictMock;



class Test_TradingThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();
        logsThreadMock         = new StrictMock<LogsThreadMock>();
        timeUtilsMock          = new StrictMock<TimeUtilsMock>();

        EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("bbbbb"), QString("But why")));

        thread = new TradingThread(
            instrumentsStorageMock, grpcClientMock, logsThreadMock, timeUtilsMock, "aaaaa", "bbbbb", 1000.0, "But why"
        );
    }

    void TearDown() override
    {
        delete thread;
        delete instrumentsStorageMock;
        delete grpcClientMock;
        delete logsThreadMock;
        delete timeUtilsMock;
    }

    TradingThread*                      thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
    StrictMock<LogsThreadMock>*         logsThreadMock;
    StrictMock<TimeUtilsMock>*          timeUtilsMock;
};



TEST_F(Test_TradingThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_TradingThread, Test_run)
{
    // thread->run();
}
