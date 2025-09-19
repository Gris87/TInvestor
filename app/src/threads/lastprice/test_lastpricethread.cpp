#include "src/threads/lastprice/lastpricethread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_LastPriceThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        timeUtilsMock     = new StrictMock<TimeUtilsMock>();
        grpcClientMock    = new StrictMock<GrpcClientMock>();

        thread = new LastPriceThread(stocksStorageMock, timeUtilsMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
        delete timeUtilsMock;
        delete grpcClientMock;
    }

    LastPriceThread*               thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<TimeUtilsMock>*     timeUtilsMock;
    StrictMock<GrpcClientMock>*    grpcClientMock;
};



TEST_F(Test_LastPriceThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_LastPriceThread, Test_run)
{
    const InSequence seq;

    QList<Stock*> stocks;
    QList<Stock*> emptyStocks;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());

    Stock stock;
    stock.meta.instrumentId = "aaaa";

    stocks << &stock;

    const std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse1(new tinkoff::MarketDataResponse());

    tinkoff::LastPrice* lastPrice1 = new tinkoff::LastPrice(); // marketDataResponse1 will take ownership
    tinkoff::Quotation* price1     = new tinkoff::Quotation(); // marketDataResponse1 will take ownership

    price1->set_units(100);
    price1->set_nano(500000000);

    google::protobuf::Timestamp* time1 = new google::protobuf::Timestamp(); // marketDataResponse1 will take ownership

    time1->set_seconds(2000);
    time1->set_nanos(123000000);

    lastPrice1->set_allocated_price(price1);
    lastPrice1->set_allocated_time(time1);
    lastPrice1->set_instrument_uid("aaaa");

    marketDataResponse1->set_allocated_last_price(lastPrice1);

    const std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse2(new tinkoff::MarketDataResponse());

    tinkoff::LastPrice* lastPrice2 = new tinkoff::LastPrice(); // marketDataResponse2 will take ownership
    tinkoff::Quotation* price2     = new tinkoff::Quotation(); // marketDataResponse2 will take ownership

    price2->set_units(100);
    price2->set_nano(400000000);

    google::protobuf::Timestamp* time2 = new google::protobuf::Timestamp(); // marketDataResponse2 will take ownership

    time2->set_seconds(1000);
    time2->set_nanos(123000000);

    lastPrice2->set_allocated_price(price2);
    lastPrice2->set_allocated_time(time2);
    lastPrice2->set_instrument_uid("aaaa");

    marketDataResponse2->set_allocated_last_price(lastPrice2);

    ASSERT_EQ(stock.operational.detailedData.size(), 0);

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForLastPrice(QStringList() << "aaaa")).WillOnce(Return(marketDataStream));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(marketDataResponse1));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(marketDataResponse2));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, finishMarketDataStream(marketDataStream));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(emptyStocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(emptyStocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    thread->run();

    // clang-format off
    ASSERT_EQ(stock.operational.detailedData.size(),          2);
    ASSERT_EQ(stock.operational.detailedData.at(0).timestamp, 1000123);
    ASSERT_NEAR(stock.operational.detailedData.at(0).price,   100.4f, 0.0001f);
    ASSERT_EQ(stock.operational.detailedData.at(1).timestamp, 2000123);
    ASSERT_NEAR(stock.operational.detailedData.at(1).price,   100.5f, 0.0001f);
    // clang-format on
}

TEST_F(Test_LastPriceThread, Test_stocksChanged)
{
    const InSequence seq;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());
    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForLastPrice(QStringList() << "aaaa")).WillOnce(Return(marketDataStream));

    ASSERT_EQ(thread->createMarketDataStream(QStringList() << "aaaa"), true);

    EXPECT_CALL(*grpcClientMock, cancelMarketDataStream(marketDataStream));

    thread->stocksChanged();
}

TEST_F(Test_LastPriceThread, Test_terminateThread)
{
    const InSequence seq;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());
    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForLastPrice(QStringList() << "aaaa")).WillOnce(Return(marketDataStream));

    ASSERT_EQ(thread->createMarketDataStream(QStringList() << "aaaa"), true);

    EXPECT_CALL(*grpcClientMock, cancelMarketDataStream(marketDataStream));

    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
