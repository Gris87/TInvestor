#include "src/threads/bidirtradingcontrol/bidirtradingcontrolthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_BiDirTradingControlThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        userStorageMock   = new StrictMock<UserStorageMock>();
        configMock        = new StrictMock<ConfigMock>();
        timeUtilsMock     = new StrictMock<TimeUtilsMock>();
        grpcClientMock    = new StrictMock<GrpcClientMock>();

        thread = new BiDirTradingControlThread(stocksStorageMock, userStorageMock, configMock, timeUtilsMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
        delete userStorageMock;
        delete configMock;
        delete timeUtilsMock;
        delete grpcClientMock;
    }

    BiDirTradingControlThread*     thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<UserStorageMock>*   userStorageMock;
    StrictMock<ConfigMock>*        configMock;
    StrictMock<TimeUtilsMock>*     timeUtilsMock;
    StrictMock<GrpcClientMock>*    grpcClientMock;
};



TEST_F(Test_BiDirTradingControlThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirTradingControlThread, Test_run)
{
    const InSequence seq;

    EXPECT_CALL(*configMock, isTradeHugeBid()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isTradeHugeSpread()).WillOnce(Return(true));
    EXPECT_CALL(*timeUtilsMock, isWorkingHours(Ge(1704056400000))).WillOnce(Return(false));

    thread->run();
}

TEST_F(Test_BiDirTradingControlThread, Test_detectStocksForBiDirTrading)
{
    const InSequence seq;

    QList<Stock*> stocks;

    Stock stock;
    stock.meta.instrumentId = "aaa-aaa";

    stocks << &stock;

    const std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBookResponse(new tinkoff::GetOrderBookResponse());

    tinkoff::Order* bid = getOrderBookResponse->add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask = getOrderBookResponse->add_asks(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* bidPrice = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* askPrice = new tinkoff::Quotation(); // ask will take ownership

    bidPrice->set_units(10);
    bidPrice->set_nano(400000000);
    askPrice->set_units(20);
    askPrice->set_nano(500000000);

    bid->set_quantity(50);
    bid->set_allocated_price(bidPrice);
    ask->set_quantity(10);
    ask->set_allocated_price(askPrice);

    EXPECT_CALL(*timeUtilsMock, isWorkingHours(Ge(1704092400000))).WillOnce(Return(true));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(true));
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*configMock, getHugeBid()).WillOnce(Return(2.0f));
    EXPECT_CALL(*configMock, getHugeSpread()).WillOnce(Return(0.3f));
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaa-aaa"), 20))
        .WillOnce(Return(getOrderBookResponse));

    thread->detectStocksForBiDirTrading(1704092400000, true, true);
}

TEST_F(Test_BiDirTradingControlThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
