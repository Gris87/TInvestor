#include "src/threads/orderbook/orderbookthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_OrderBookThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        grpcClientMock = new StrictMock<GrpcClientMock>();

        thread = new OrderBookThread(grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete grpcClientMock;
    }

    OrderBookThread*            thread;
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
    stock.meta.uid = "aaaaa";
    stock.operational.detailedData.append(stockData);

    thread->setStock(&stock);

    std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBookResponse(new tinkoff::GetOrderBookResponse());

    tinkoff::Order* ask = getOrderBookResponse->add_asks(); // getOrderBookResponse will take ownership
    tinkoff::Order* bid = getOrderBookResponse->add_bids(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* askPrice = new tinkoff::Quotation(); // getOrderBookResponse will take ownership
    tinkoff::Quotation* bidPrice = new tinkoff::Quotation(); // getOrderBookResponse will take ownership

    askPrice->set_units(100);
    askPrice->set_nano(500000000);
    bidPrice->set_units(200);
    bidPrice->set_nano(500000000);

    ask->set_quantity(10);
    ask->set_allocated_price(askPrice);

    bid->set_quantity(10);
    bid->set_allocated_price(bidPrice);

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());

    std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse(new tinkoff::MarketDataResponse());

    tinkoff::OrderBook* orderBook = new tinkoff::OrderBook(); // marketDataResponse will take ownership

    ::google::protobuf::Timestamp* time = new ::google::protobuf::Timestamp(); // marketDataResponse will take ownership

    time->set_seconds(1000);
    time->set_nanos(123000000);

    orderBook->set_allocated_time(time);

    tinkoff::Order* lastAsk = orderBook->add_asks(); // orderBook will take ownership
    tinkoff::Order* lastBid = orderBook->add_bids(); // orderBook will take ownership

    tinkoff::Quotation* lastAskPrice = new tinkoff::Quotation(); // orderBook will take ownership
    tinkoff::Quotation* lastBidPrice = new tinkoff::Quotation(); // orderBook will take ownership

    lastAskPrice->set_units(100);
    lastAskPrice->set_nano(500000000);
    lastBidPrice->set_units(200);
    lastBidPrice->set_nano(500000000);

    lastAsk->set_quantity(10);
    lastAsk->set_allocated_price(lastAskPrice);

    lastBid->set_quantity(10);
    lastBid->set_allocated_price(lastBidPrice);

    marketDataResponse->set_allocated_orderbook(orderBook);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"))).WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, createMarketDataStream()).WillOnce(Return(marketDataStream));
    EXPECT_CALL(*grpcClientMock, subscribeOrderBook(marketDataStream, QString("aaaaa"))).WillOnce(Return(true));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(marketDataResponse));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, finishMarketDataStream(marketDataStream));

    thread->run();
}

TEST_F(Test_OrderBookThread, Test_terminateThread)
{
    const InSequence seq;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());
    EXPECT_CALL(*grpcClientMock, createMarketDataStream()).WillOnce(Return(marketDataStream));

    thread->createMarketDataStream();

    EXPECT_CALL(*grpcClientMock, unsubscribeOrderBook(marketDataStream)).WillOnce(Return(true));
    EXPECT_CALL(*grpcClientMock, closeWriteMarketDataStream(marketDataStream)).WillOnce(Return(true));

    thread->terminateThread();
}
