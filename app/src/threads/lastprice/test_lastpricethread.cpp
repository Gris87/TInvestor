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

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(emptyStocks));
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*grpcClientMock, createMarketDataStream()).WillOnce(Return(marketDataStream));
    EXPECT_CALL(*grpcClientMock, subscribeLastPrices(marketDataStream, QStringList() << "aaaa"));
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, finishMarketDataStream(marketDataStream));

    thread->run();
}
