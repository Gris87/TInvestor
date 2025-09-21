#include "src/threads/bidirtrading/bidirtradingthreadfactory.h"

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
using ::testing::StrictMock;



class Test_BiDirTradingThreadFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BiDirTradingThreadFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BiDirTradingThreadFactory* factory;
};



TEST_F(Test_BiDirTradingThreadFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirTradingThreadFactory, Test_newInstance)
{
    const InSequence seq;

    StrictMock<InstrumentsStorageMock> instrumentsStorageMock;
    StrictMock<UserStorageMock>        userStorageMock;
    StrictMock<ConfigMock>             configMock;
    StrictMock<TimeUtilsMock>          timeUtilsMock;
    StrictMock<TradeUtilsMock>         tradeUtilsMock;
    StrictMock<GrpcClientMock>         grpcClientMock;
    StrictMock<GrpcRetryClientMock>    grpcRetryClientMock;
    StrictMock<LogsThreadMock>         logsThreadMock;

    Stock stock;

    stock.meta.instrumentId = "aaa-aaa";
    stock.meta.turnover     = 1000000;

    EXPECT_CALL(logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("aaa-aaa"), QString("But why")));

    const IBiDirTradingThread* thread = factory->newInstance(
        &instrumentsStorageMock,
        &userStorageMock,
        &configMock,
        &timeUtilsMock,
        &tradeUtilsMock,
        &grpcClientMock,
        &grpcRetryClientMock,
        &logsThreadMock,
        "account-id",
        &stock,
        "But why",
        nullptr
    );
    ASSERT_TRUE(thread != nullptr);

    delete thread;
}
