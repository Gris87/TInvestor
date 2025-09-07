#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/threads/logs/ilogsthread_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"
#include "src/utils/tradeutils/itradeutils_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_BiDirTradingThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        configMock             = new StrictMock<ConfigMock>();
        timeUtilsMock          = new StrictMock<TimeUtilsMock>();
        tradeUtilsMock         = new StrictMock<TradeUtilsMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();
        logsThreadMock         = new StrictMock<LogsThreadMock>();

        EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("aaaaa"), QString("But why")));

        thread = new BiDirTradingThread(
            instrumentsStorageMock,
            configMock,
            timeUtilsMock,
            tradeUtilsMock,
            grpcClientMock,
            logsThreadMock,
            "account-id",
            "aaaaa",
            1000000,
            "But why"
        );
    }

    void TearDown() override
    {
        delete thread;
        delete instrumentsStorageMock;
        delete configMock;
        delete timeUtilsMock;
        delete tradeUtilsMock;
        delete grpcClientMock;
        delete logsThreadMock;
    }

    BiDirTradingThread*                 thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<ConfigMock>*             configMock;
    StrictMock<TimeUtilsMock>*          timeUtilsMock;
    StrictMock<TradeUtilsMock>*         tradeUtilsMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
    StrictMock<LogsThreadMock>*         logsThreadMock;
};



TEST_F(Test_BiDirTradingThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirTradingThread, Test_run)
{
}

TEST_F(Test_BiDirTradingThread, Test_terminateThread)
{
    thread->terminateThread();
}
