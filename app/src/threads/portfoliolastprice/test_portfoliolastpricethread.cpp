#include "src/threads/portfoliolastprice/portfoliolastpricethread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_PortfolioLastPriceThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        timeUtilsMock     = new StrictMock<TimeUtilsMock>();
        grpcClientMock    = new StrictMock<GrpcClientMock>();

        thread = new PortfolioLastPriceThread(stocksStorageMock, timeUtilsMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
        delete timeUtilsMock;
        delete grpcClientMock;
    }

    PortfolioLastPriceThread*      thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<TimeUtilsMock>*     timeUtilsMock;
    StrictMock<GrpcClientMock>*    grpcClientMock;
};



TEST_F(Test_PortfolioLastPriceThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioLastPriceThread, Test_run)
{
    const InSequence seq;

    QMutex        mutex;
    QList<Stock*> stocks;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());

    Stock stock;
    stock.meta.uid = "aaaa";

    stocks.append(&stock);

    const std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse(new tinkoff::MarketDataResponse());

    tinkoff::LastPrice* lastPrice = new tinkoff::LastPrice(); // marketDataResponse will take ownership
    tinkoff::Quotation* price     = new tinkoff::Quotation(); // marketDataResponse will take ownership

    price->set_units(100);
    price->set_nano(500000000);

    google::protobuf::Timestamp* time = new google::protobuf::Timestamp(); // marketDataResponse will take ownership

    time->set_seconds(1000);
    time->set_nanos(123000000);

    lastPrice->set_allocated_price(price);
    lastPrice->set_allocated_time(time);
    lastPrice->set_instrument_uid("aaaa");

    marketDataResponse->set_allocated_last_price(lastPrice);

    ASSERT_EQ(stock.operational.detailedData.size(), 0);

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*grpcClientMock, createMarketDataStream()).WillOnce(Return(marketDataStream));
    EXPECT_CALL(*grpcClientMock, subscribeLastPrices(marketDataStream, QStringList() << "aaaa")).WillOnce(Return(true));
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(marketDataResponse));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, finishMarketDataStream(marketDataStream));

    thread->run();

    // clang-format off
    ASSERT_EQ(stock.operational.detailedData.size(),          1);
    ASSERT_EQ(stock.operational.detailedData.at(0).timestamp, 1000123);
    ASSERT_NEAR(stock.operational.detailedData.at(0).price,   100.5f, 0.0001f);
    // clang-format on
}

TEST_F(Test_PortfolioLastPriceThread, Test_run_interrupted_without_stocks)
{
    const InSequence seq;

    QMutex        mutex;
    QList<Stock*> stocks;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    thread->run();
}

TEST_F(Test_PortfolioLastPriceThread, Test_stocksChanged)
{
    const InSequence seq;

    QMutex        mutex;
    QList<Stock*> stocks;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());

    Stock stock;
    stock.meta.uid = "bbbb";

    stocks.append(&stock);

    EXPECT_CALL(*grpcClientMock, createMarketDataStream()).WillOnce(Return(marketDataStream));

    thread->createMarketDataStream();

    EXPECT_CALL(*grpcClientMock, unsubscribeLastPrices(marketDataStream)).WillOnce(Return(true));
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*grpcClientMock, subscribeLastPrices(marketDataStream, QStringList() << "bbbb")).WillOnce(Return(true));

    thread->stocksChanged();
}

TEST_F(Test_PortfolioLastPriceThread, Test_terminateThread)
{
    const InSequence seq;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());
    EXPECT_CALL(*grpcClientMock, createMarketDataStream()).WillOnce(Return(marketDataStream));

    thread->createMarketDataStream();

    EXPECT_CALL(*grpcClientMock, closeWriteMarketDataStream(marketDataStream)).WillOnce(Return(true));
    EXPECT_CALL(*grpcClientMock, cancelMarketDataStream(marketDataStream));

    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
