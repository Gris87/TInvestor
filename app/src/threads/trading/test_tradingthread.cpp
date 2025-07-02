#include "src/threads/trading/tradingthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/threads/logs/ilogsthread_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_TradingThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();
        logsThreadMock         = new StrictMock<LogsThreadMock>();
        timeUtilsMock          = new StrictMock<TimeUtilsMock>();

        EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("aaaaa"), QString("But why")));

        thread = new TradingThread(
            instrumentsStorageMock, grpcClientMock, logsThreadMock, timeUtilsMock, "account-id", "aaaaa", 10000.0, "But why"
        );
    }

    void TearDown() override
    {
        delete thread;
        delete instrumentsStorageMock;
        delete grpcClientMock;
        delete logsThreadMock;
        delete timeUtilsMock;
    }

    TradingThread*                      thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
    StrictMock<LogsThreadMock>*         logsThreadMock;
    StrictMock<TimeUtilsMock>*          timeUtilsMock;
};



TEST_F(Test_TradingThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_TradingThread, Test_run)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.lot            = 10;
    instrument.pricePrecision = 3;

    instruments["aaaaa"] = instrument;

    const std::shared_ptr<tinkoff::PortfolioResponse> portfolioResponse(new tinkoff::PortfolioResponse());
    const std::shared_ptr<tinkoff::PortfolioResponse> portfolioResponse2(new tinkoff::PortfolioResponse());

    tinkoff::PortfolioPosition* position1 = portfolioResponse->add_positions(); // portfolioResponse will take ownership
    tinkoff::PortfolioPosition* position2 = portfolioResponse->add_positions(); // portfolioResponse will take ownership

    tinkoff::Quotation*  tinkoffQuantity     = new tinkoff::Quotation();  // position2 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo = new tinkoff::MoneyValue(); // position2 will take ownership

    tinkoffQuantity->set_units(1000);
    tinkoffQuantity->set_nano(0);

    tinkoffAvgPriceFifo->set_currency("rub");
    tinkoffAvgPriceFifo->set_units(50);
    tinkoffAvgPriceFifo->set_nano(500000000);

    position1->set_instrument_uid("bbbbb");

    position2->set_instrument_uid("aaaaa");
    position2->set_allocated_quantity(tinkoffQuantity);
    position2->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo);

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1)).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(30000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse2));
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1)).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(30000, QThread::currentThread())).WillOnce(Return(true));

    thread->run();

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id"))).WillOnce(Return(nullptr));

    thread->run();

    const std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBookResponse(new tinkoff::GetOrderBookResponse());

    tinkoff::Order* bid = getOrderBookResponse->add_bids(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* bidPrice = new tinkoff::Quotation(); // bid will take ownership

    bidPrice->set_units(10);
    bidPrice->set_nano(500000000);

    bid->set_quantity(10);
    bid->set_allocated_price(bidPrice);

    const std::shared_ptr<tinkoff::GetMaxLotsResponse> getMaxLotsResponse(new tinkoff::GetMaxLotsResponse());

    tinkoff::GetMaxLotsResponse_BuyLimitsView* buyLimits =
        new tinkoff::GetMaxLotsResponse_BuyLimitsView(); // getMaxLotsResponse will take ownership

    buyLimits->set_buy_max_lots(0);

    getMaxLotsResponse->set_allocated_buy_limits(buyLimits);

    Quotation price;
    price.units = 10;
    price.nano  = 500000000;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse2));
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Trade completed successfully")));

    thread->run();
}

TEST_F(Test_TradingThread, Test_terminateThread)
{
    thread->terminateThread();
}

TEST_F(Test_TradingThread, Test_setExpectedCost_and_expectedCost)
{
    const InSequence seq;

    ASSERT_NEAR(thread->expectedCost(), 10000.0, 0.0001);

    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("aaaaa"), QString("Another but why")));

    thread->setExpectedCost(15000.0, "Another but why");
    ASSERT_NEAR(thread->expectedCost(), 15000.0, 0.0001);
}

TEST_F(Test_TradingThread, Test_sell)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.lot            = 10;
    instrument.pricePrecision = 3;

    instruments["aaaaa"] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());

    thread->getInstrumentData();

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1)).WillOnce(Return(nullptr));

    ASSERT_EQ(thread->sell(10000, 40000), false);

    const std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBookResponse(new tinkoff::GetOrderBookResponse());

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));

    ASSERT_EQ(thread->sell(10000, 40000), false);

    tinkoff::Order* ask = getOrderBookResponse->add_asks(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* askPrice = new tinkoff::Quotation(); // ask will take ownership

    askPrice->set_units(10);
    askPrice->set_nano(500000000);

    ask->set_quantity(10);
    ask->set_allocated_price(askPrice);

    Quotation price;
    price.units = 10;
    price.nano  = 500000000;

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(nullptr));

    ASSERT_EQ(thread->sell(10000, 40000), false);

    const std::shared_ptr<tinkoff::GetMaxLotsResponse> getMaxLotsResponse(new tinkoff::GetMaxLotsResponse());

    tinkoff::GetMaxLotsResponse_SellLimitsView* sellLimits =
        new tinkoff::GetMaxLotsResponse_SellLimitsView(); // getMaxLotsResponse will take ownership

    sellLimits->set_sell_max_lots(0);

    getMaxLotsResponse->set_allocated_sell_limits(sellLimits);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));

    ASSERT_EQ(thread->sell(10000, 40000), true);

    sellLimits->set_sell_max_lots(5);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(nullptr));

    ASSERT_EQ(thread->sell(10000, 40000), false);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse(new tinkoff::PostOrderResponse());

    postOrderResponse->set_order_id("order-id");
    postOrderResponse->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_REJECTED);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(thread->sell(10000, 40000), false);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse2(new tinkoff::PostOrderResponse());

    postOrderResponse2->set_order_id("order-id");
    postOrderResponse2->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order to sell 50 created with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->sell(10000, 40000), false);

    const std::shared_ptr<tinkoff::OrderState> orderState(new tinkoff::OrderState());

    tinkoff::MoneyValue* orderPrice = new tinkoff::MoneyValue(); // orderState will take ownership

    orderPrice->set_units(10);
    orderPrice->set_nano(500000000);

    orderState->set_direction(tinkoff::ORDER_DIRECTION_SELL);
    orderState->set_lots_executed(5);
    orderState->set_allocated_initial_security_price(orderPrice);
    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_FILL);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order completed. 50 sold with a price 10.500 \u20BD"))
    );
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order to sell 50 created with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->sell(20000, 30000), false);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_PARTIALLYFILL);

    const std::shared_ptr<tinkoff::CancelOrderResponse> cancelOrderResponse(new tinkoff::CancelOrderResponse());

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order partially completed. 50 sold with a price 10.500 \u20BD"))
    );
    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order cancelled")));
    EXPECT_CALL(*grpcClientMock, cancelOrder(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(cancelOrderResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order to sell 50 created with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->sell(10000, 40000), false);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_FILL);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order completed. 50 sold with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->sell(10000, 40000), true);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_REJECTED);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order rejected. 50 sold with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->sell(10000, 40000), false);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order to sell 50 created with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->sell(10000, 40000), false);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_REJECTED);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(nullptr));

    ASSERT_EQ(thread->sell(10000, 40000), false);
}

TEST_F(Test_TradingThread, Test_buy)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.lot            = 10;
    instrument.pricePrecision = 3;

    instruments["aaaaa"] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());

    thread->getInstrumentData();

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1)).WillOnce(Return(nullptr));

    ASSERT_EQ(thread->buy(10000, 40000), false);

    const std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBookResponse(new tinkoff::GetOrderBookResponse());

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));

    ASSERT_EQ(thread->buy(10000, 40000), false);

    tinkoff::Order* bid = getOrderBookResponse->add_bids(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* bidPrice = new tinkoff::Quotation(); // bid will take ownership

    bidPrice->set_units(10);
    bidPrice->set_nano(500000000);

    bid->set_quantity(10);
    bid->set_allocated_price(bidPrice);

    Quotation price;
    price.units = 10;
    price.nano  = 500000000;

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(nullptr));

    ASSERT_EQ(thread->buy(10000, 40000), false);

    const std::shared_ptr<tinkoff::GetMaxLotsResponse> getMaxLotsResponse(new tinkoff::GetMaxLotsResponse());

    tinkoff::GetMaxLotsResponse_BuyLimitsView* buyLimits =
        new tinkoff::GetMaxLotsResponse_BuyLimitsView(); // getMaxLotsResponse will take ownership

    buyLimits->set_buy_max_lots(0);

    getMaxLotsResponse->set_allocated_buy_limits(buyLimits);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));

    ASSERT_EQ(thread->buy(10000, 40000), true);

    buyLimits->set_buy_max_lots(5);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(nullptr));

    ASSERT_EQ(thread->buy(10000, 40000), false);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse(new tinkoff::PostOrderResponse());

    postOrderResponse->set_order_id("order-id");
    postOrderResponse->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_REJECTED);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(thread->buy(10000, 40000), false);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse2(new tinkoff::PostOrderResponse());

    postOrderResponse2->set_order_id("order-id");
    postOrderResponse2->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order to buy 50 created with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->buy(10000, 40000), false);

    const std::shared_ptr<tinkoff::OrderState> orderState(new tinkoff::OrderState());

    tinkoff::MoneyValue* orderPrice = new tinkoff::MoneyValue(); // orderState will take ownership

    orderPrice->set_units(10);
    orderPrice->set_nano(500000000);

    orderState->set_direction(tinkoff::ORDER_DIRECTION_BUY);
    orderState->set_lots_executed(5);
    orderState->set_allocated_initial_security_price(orderPrice);
    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_FILL);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order completed. 50 bought with a price 10.500 \u20BD"))
    );
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order to buy 50 created with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->buy(20000, 30000), false);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    const std::shared_ptr<tinkoff::CancelOrderResponse> cancelOrderResponse(new tinkoff::CancelOrderResponse());

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order is still waiting. 50 bought with a price 10.500 \u20BD"))
    );
    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order cancelled")));
    EXPECT_CALL(*grpcClientMock, cancelOrder(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(cancelOrderResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order to buy 50 created with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->buy(10000, 40000), false);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_FILL);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order completed. 50 bought with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->buy(10000, 40000), true);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_CANCELLED);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order cancelled. 50 bought with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->buy(10000, 40000), false);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order to buy 50 created with a price 10.500 \u20BD"))
    );

    ASSERT_EQ(thread->buy(10000, 40000), false);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_REJECTED);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 1))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(nullptr));

    ASSERT_EQ(thread->buy(10000, 40000), false);
}
