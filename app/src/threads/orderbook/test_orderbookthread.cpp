#include "src/threads/orderbook/orderbookthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_OrderBookThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        timeUtilsMock  = new StrictMock<TimeUtilsMock>();
        grpcClientMock = new StrictMock<GrpcClientMock>();

        thread = new OrderBookThread(timeUtilsMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete timeUtilsMock;
        delete grpcClientMock;
    }

    OrderBookThread*            thread;
    StrictMock<TimeUtilsMock>*  timeUtilsMock;
    StrictMock<GrpcClientMock>* grpcClientMock;
};



TEST_F(Test_OrderBookThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_OrderBookThread, Test_run)
{
    const InSequence seq;

    StockOperationalData stockData;

    stockData.timestamp = 100;
    stockData.price     = 5000.75f;

    Stock stock;
    stock.meta.instrumentId = "aaaaa";
    stock.operational.detailedData << stockData;

    thread->setStock(&stock);

    const std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBookResponse(new tinkoff::GetOrderBookResponse());

    tinkoff::Order* bid = getOrderBookResponse->add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask = getOrderBookResponse->add_asks(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* bidPrice = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* askPrice = new tinkoff::Quotation(); // ask will take ownership

    bidPrice->set_units(100);
    bidPrice->set_nano(500000000);
    askPrice->set_units(200);
    askPrice->set_nano(500000000);

    bid->set_quantity(10);
    bid->set_allocated_price(bidPrice);
    ask->set_quantity(10);
    ask->set_allocated_price(askPrice);

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());

    const std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse(new tinkoff::MarketDataResponse());

    tinkoff::OrderBook* orderBook = new tinkoff::OrderBook(); // marketDataResponse will take ownership

    google::protobuf::Timestamp* time = new google::protobuf::Timestamp(); // marketDataResponse will take ownership

    time->set_seconds(1000);
    time->set_nanos(123000000);

    orderBook->set_allocated_time(time);

    tinkoff::Order* lastBid = orderBook->add_bids(); // orderBook will take ownership
    tinkoff::Order* lastAsk = orderBook->add_asks(); // orderBook will take ownership

    tinkoff::Quotation* lastBidPrice = new tinkoff::Quotation(); // lastBid will take ownership
    tinkoff::Quotation* lastAskPrice = new tinkoff::Quotation(); // lastAsk will take ownership

    lastBidPrice->set_units(100);
    lastBidPrice->set_nano(500000000);
    lastAskPrice->set_units(200);
    lastAskPrice->set_nano(500000000);

    lastBid->set_quantity(10);
    lastBid->set_allocated_price(lastBidPrice);
    lastAsk->set_quantity(10);
    lastAsk->set_allocated_price(lastAskPrice);

    marketDataResponse->set_allocated_orderbook(orderBook);

    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForOrderBook(QString("aaaaa"), 50)).WillOnce(Return(marketDataStream));
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 50))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(marketDataResponse));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, cancelMarketDataStream(marketDataStream));
    EXPECT_CALL(*grpcClientMock, finishMarketDataStream(marketDataStream));
    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForOrderBook(QString("aaaaa"), 50)).WillOnce(Return(marketDataStream));
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 50)).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, cancelMarketDataStream(marketDataStream));
    EXPECT_CALL(*grpcClientMock, finishMarketDataStream(marketDataStream));
    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForOrderBook(QString("aaaaa"), 50)).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    thread->run();
}

TEST_F(Test_OrderBookThread, Test_terminateThread)
{
    const InSequence seq;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());
    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForOrderBook(QString("aaaaa"), 50)).WillOnce(Return(marketDataStream));

    ASSERT_EQ(thread->createMarketDataStream("aaaaa"), true);

    EXPECT_CALL(*grpcClientMock, cancelMarketDataStream(marketDataStream));

    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
