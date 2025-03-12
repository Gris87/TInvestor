#include "src/threads/lastprice/lastpricethread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_LastPriceThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        grpcClientMock    = new StrictMock<GrpcClientMock>();

        thread = new LastPriceThread(stocksStorageMock, grpcClientMock);
    }

    void TearDown()
    {
        delete thread;
        delete stocksStorageMock;
        delete grpcClientMock;
    }

    LastPriceThread*               thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<GrpcClientMock>*    grpcClientMock;
};



TEST_F(Test_LastPriceThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_LastPriceThread, Test_run)
{
    InSequence seq;

    QMutex        mutex;
    QList<Stock*> emptyStocks;
    QList<Stock*> stocks;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());

    Stock stock;
    stock.meta.uid = "aaaa";

    stocks.append(&stock);

    std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse(new tinkoff::MarketDataResponse());

    tinkoff::LastPrice* lastPrice = new tinkoff::LastPrice(); // marketDataResponse will take ownership
    tinkoff::Quotation* price     = new tinkoff::Quotation(); // marketDataResponse will take ownership

    price->set_units(100);
    price->set_nano(500000000);

    ::google::protobuf::Timestamp* time = new ::google::protobuf::Timestamp(); // marketDataResponse will take ownership

    time->set_seconds(1000);
    time->set_nanos(123000000);

    lastPrice->set_allocated_price(price);
    lastPrice->set_allocated_time(time);
    lastPrice->set_instrument_uid("aaaa");

    marketDataResponse->set_allocated_last_price(lastPrice);

    ASSERT_EQ(stock.operational.detailedData.size(), 0);

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(emptyStocks));
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*grpcClientMock, createMarketDataStream()).WillOnce(Return(marketDataStream));
    EXPECT_CALL(*grpcClientMock, subscribeLastPrices(marketDataStream, QStringList() << "aaaa"));
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

TEST_F(Test_LastPriceThread, Test_stocksChanged)
{
    InSequence seq;

    QMutex        mutex;
    QList<Stock*> stocks;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());

    Stock stock;
    stock.meta.uid = "bbbb";

    stocks.append(&stock);

    EXPECT_CALL(*grpcClientMock, createMarketDataStream()).WillOnce(Return(marketDataStream));

    thread->createMarketDataStream();

    EXPECT_CALL(*grpcClientMock, unsubscribeLastPrices(marketDataStream));
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*grpcClientMock, subscribeLastPrices(marketDataStream, QStringList() << "bbbb"));

    thread->stocksChanged();
}

TEST_F(Test_LastPriceThread, Test_terminateThread)
{
    InSequence seq;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());
    EXPECT_CALL(*grpcClientMock, createMarketDataStream()).WillOnce(Return(marketDataStream));

    thread->createMarketDataStream();

    EXPECT_CALL(*grpcClientMock, closeWriteMarketDataStream(marketDataStream));

    thread->terminateThread();
}
