#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/grpc/igrpcretryclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
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
        userStorageMock        = new StrictMock<UserStorageMock>();
        configMock             = new StrictMock<ConfigMock>();
        timeUtilsMock          = new StrictMock<TimeUtilsMock>();
        tradeUtilsMock         = new StrictMock<TradeUtilsMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();
        grpcRetryClientMock    = new StrictMock<GrpcRetryClientMock>();
        logsThreadMock         = new StrictMock<LogsThreadMock>();

        EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("aaa-aaa"), QString("But why")));

        thread = new BiDirTradingThread(
            instrumentsStorageMock,
            userStorageMock,
            configMock,
            timeUtilsMock,
            tradeUtilsMock,
            grpcClientMock,
            grpcRetryClientMock,
            logsThreadMock,
            "account-id",
            "aaa-aaa",
            1000000,
            "But why"
        );
    }

    void TearDown() override
    {
        delete thread;
        delete instrumentsStorageMock;
        delete userStorageMock;
        delete configMock;
        delete timeUtilsMock;
        delete tradeUtilsMock;
        delete grpcClientMock;
        delete grpcRetryClientMock;
        delete logsThreadMock;
    }

    BiDirTradingThread*                 thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<UserStorageMock>*        userStorageMock;
    StrictMock<ConfigMock>*             configMock;
    StrictMock<TimeUtilsMock>*          timeUtilsMock;
    StrictMock<TradeUtilsMock>*         tradeUtilsMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
    StrictMock<GrpcRetryClientMock>*    grpcRetryClientMock;
    StrictMock<LogsThreadMock>*         logsThreadMock;
};



TEST_F(Test_BiDirTradingThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirTradingThread, Test_run)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker                  = "ABBA";
    instrument.name                    = "Abstract Basics";
    instrument.lot                     = 10;
    instrument.pricePrecision          = 3;
    instrument.minPriceIncrement.units = 0;
    instrument.minPriceIncrement.nano  = 1000000;

    instruments["aaa-aaa"] = instrument;

    thread->terminateTrading();

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_VERBOSE, QString("aaa-aaa"), QString("Reselling completed successfully")));

    thread->run();
}

TEST_F(Test_BiDirTradingThread, Test_setTurnover_and_turnover)
{
    ASSERT_EQ(thread->turnover(), 1000000);

    thread->setTurnover(5000);
    ASSERT_EQ(thread->turnover(), 5000);
}

TEST_F(Test_BiDirTradingThread, Test_terminateThread)
{
    thread->terminateThread();
}

TEST_F(Test_BiDirTradingThread, Test_trade)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker                  = "ABBA";
    instrument.name                    = "Abstract Basics";
    instrument.lot                     = 10;
    instrument.pricePrecision          = 3;
    instrument.minPriceIncrement.units = 0;
    instrument.minPriceIncrement.nano  = 1000000;

    instruments["aaa-aaa"] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaa-aaa"), 3)).WillOnce(Return(nullptr));

    ASSERT_EQ(thread->trade(), false);
}
