#include "src/threads/trading/tradingthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/grpc/igrpcretryclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
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
        userStorageMock        = new StrictMock<UserStorageMock>();
        configMock             = new StrictMock<ConfigMock>();
        timeUtilsMock          = new StrictMock<TimeUtilsMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();
        grpcRetryClientMock    = new StrictMock<GrpcRetryClientMock>();
        logsThreadMock         = new StrictMock<LogsThreadMock>();

        EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("aaaaa"), QString("But why")));

        thread = new TradingThread(
            instrumentsStorageMock,
            userStorageMock,
            configMock,
            timeUtilsMock,
            grpcClientMock,
            grpcRetryClientMock,
            logsThreadMock,
            "account-id",
            "aaaaa",
            ASAP_MODE_NONE,
            10.0f,
            15.0f,
            10000.0,
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
        delete grpcClientMock;
        delete grpcRetryClientMock;
        delete logsThreadMock;
    }

    TradingThread*                      thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<UserStorageMock>*        userStorageMock;
    StrictMock<ConfigMock>*             configMock;
    StrictMock<TimeUtilsMock>*          timeUtilsMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
    StrictMock<GrpcRetryClientMock>*    grpcRetryClientMock;
    StrictMock<LogsThreadMock>*         logsThreadMock;
};



TEST_F(Test_TradingThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_TradingThread, Test_run)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker            = "ABBA";
    instrument.name              = "Abstract Basics";
    instrument.lot               = 10;
    instrument.pricePrecision    = 3;
    instrument.minPriceIncrement = Quotation(0, 1000000);

    instruments["aaaaa"] = instrument;

    const std::shared_ptr<tinkoff::PortfolioResponse> portfolioResponse(new tinkoff::PortfolioResponse());
    const std::shared_ptr<tinkoff::PortfolioResponse> portfolioResponse2(new tinkoff::PortfolioResponse());

    tinkoff::PortfolioPosition* position1 = portfolioResponse->add_positions(); // portfolioResponse will take ownership
    tinkoff::PortfolioPosition* position2 = portfolioResponse->add_positions(); // portfolioResponse will take ownership

    tinkoff::Quotation*  tinkoffQuantity1     = new tinkoff::Quotation();  // position1 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo1 = new tinkoff::MoneyValue(); // position1 will take ownership
    tinkoff::Quotation*  tinkoffQuantity2     = new tinkoff::Quotation();  // position2 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo2 = new tinkoff::MoneyValue(); // position2 will take ownership

    tinkoffQuantity1->set_units(1000);
    tinkoffQuantity1->set_nano(0);

    tinkoffAvgPriceFifo1->set_currency("rub");
    tinkoffAvgPriceFifo1->set_units(50);
    tinkoffAvgPriceFifo1->set_nano(500000000);

    tinkoffQuantity2->set_units(1000);
    tinkoffQuantity2->set_nano(0);

    tinkoffAvgPriceFifo2->set_currency("rub");
    tinkoffAvgPriceFifo2->set_units(50);
    tinkoffAvgPriceFifo2->set_nano(500000000);

    position1->set_instrument_uid("bbbbb");
    position1->set_allocated_quantity(tinkoffQuantity1);
    position1->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo1);

    position2->set_instrument_uid("aaaaa");
    position2->set_allocated_quantity(tinkoffQuantity2);
    position2->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo2);

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20)).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(30000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse2));
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20)).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(30000, QThread::currentThread())).WillOnce(Return(true));

    thread->run();

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(nullptr));

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

    Quotation price(10, 500000000);

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse2));
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Trade completed successfully")));

    thread->run();
}

TEST_F(Test_TradingThread, Test_terminateThread)
{
    thread->terminateThread();
}

TEST_F(Test_TradingThread, Test_setAsapMode_and_asapMode)
{
    ASSERT_EQ(thread->asapMode(), ASAP_MODE_NONE);

    thread->setAsapMode(ASAP_MODE_FOLLOW_PRICE);
    ASSERT_EQ(thread->asapMode(), ASAP_MODE_FOLLOW_PRICE);

    thread->setAsapMode(ASAP_MODE_IMMEDIATELY_TRADE);
    ASSERT_EQ(thread->asapMode(), ASAP_MODE_IMMEDIATELY_TRADE);

    thread->setAsapMode(ASAP_MODE_FOLLOW_PRICE);
    ASSERT_EQ(thread->asapMode(), ASAP_MODE_IMMEDIATELY_TRADE);
}

TEST_F(Test_TradingThread, Test_setAvgPrice_and_avgPrice)
{
    ASSERT_NEAR(thread->avgPrice(), 10.0f, 0.0001f);

    thread->setAvgPrice(150.0f);
    ASSERT_NEAR(thread->avgPrice(), 150.0f, 0.0001f);
}

TEST_F(Test_TradingThread, Test_setExpectedCost_and_expectedCost)
{
    const InSequence seq;

    ASSERT_NEAR(thread->expectedCost(), 10000.0, 0.0001);

    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("aaaaa"), QString("Another but why")));

    thread->setExpectedCost(15000.0, "Another but why");
    ASSERT_NEAR(thread->expectedCost(), 15000.0, 0.0001);
}

TEST_F(Test_TradingThread, Test_buy)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker            = "ABBA";
    instrument.name              = "Abstract Basics";
    instrument.lot               = 10;
    instrument.pricePrecision    = 3;
    instrument.minPriceIncrement = Quotation(0, 1000000);

    instruments["aaaaa"] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());

    thread->getInstrumentData();

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20)).WillOnce(Return(nullptr));

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    const std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBookResponse(new tinkoff::GetOrderBookResponse());

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    tinkoff::Order* bid = getOrderBookResponse->add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask = getOrderBookResponse->add_asks(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* bidPrice = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* askPrice = new tinkoff::Quotation(); // ask will take ownership

    bidPrice->set_units(10);
    bidPrice->set_nano(30000000);
    askPrice->set_units(10);
    askPrice->set_nano(40000000);

    bid->set_quantity(10);
    bid->set_allocated_price(bidPrice);
    ask->set_quantity(10);
    ask->set_allocated_price(askPrice);

    Quotation priceForBid(10, 30000000);
    Quotation priceForAsk(10, 40000000);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(nullptr));

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    const std::shared_ptr<tinkoff::GetMaxLotsResponse> getMaxLotsResponse(new tinkoff::GetMaxLotsResponse());

    tinkoff::GetMaxLotsResponse_BuyLimitsView* buyLimits =
        new tinkoff::GetMaxLotsResponse_BuyLimitsView(); // getMaxLotsResponse will take ownership

    buyLimits->set_buy_max_lots(0);

    getMaxLotsResponse->set_allocated_buy_limits(buyLimits);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(getMaxLotsResponse));

    ASSERT_EQ(thread->buy(30000, 10000, 40000), true);

    buyLimits->set_buy_max_lots(5);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, priceForBid, false
        )
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_WARNING, QString("aaaaa"), QString("Failed to create order to buy 50 with a price 10.030 \u20BD"))
    );
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, priceForBid, false
        )
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_WARNING, QString("aaaaa"), QString("Failed to create order to buy 50 with a price 10.030 \u20BD"))
    );
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse(new tinkoff::PostOrderResponse());

    postOrderResponse->set_order_id("order-id");
    postOrderResponse->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_REJECTED);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, priceForBid, false
        )
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_DEBUG, QString("aaaaa"), QString("Order to buy 50 rejected with a price 10.030 \u20BD. Let's try again"))
    );
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse2(new tinkoff::PostOrderResponse());

    postOrderResponse2->set_order_id("order-id");
    postOrderResponse2->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, priceForBid, false
        )
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_DEBUG, QString("aaaaa"), QString("Order to buy 50 rejected with a price 10.030 \u20BD. Let's try again"))
    );
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, priceForBid, false
        )
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_VERBOSE,
            QString("aaaaa"),
            QString("Order to buy 50 created with a price 10.030 \u20BD while market price 10.040 \u20BD")
        )
    );

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    const std::shared_ptr<tinkoff::OrderState> orderState(new tinkoff::OrderState());

    tinkoff::MoneyValue* orderPrice = new tinkoff::MoneyValue(); // orderState will take ownership

    orderPrice->set_units(10);
    orderPrice->set_nano(30000000);

    orderState->set_direction(tinkoff::ORDER_DIRECTION_BUY);
    orderState->set_lots_executed(5);
    orderState->set_lots_requested(5);
    orderState->set_allocated_initial_security_price(orderPrice);
    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_FILL);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order completed. 50/50 bought with a price 10.030 \u20BD"))
    );
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(3000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, priceForBid, false
        )
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_VERBOSE,
            QString("aaaaa"),
            QString("Order to buy 50 created with a price 10.030 \u20BD while market price 10.040 \u20BD")
        )
    );

    ASSERT_EQ(thread->buy(30000, 20000, 30000), false);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    const std::shared_ptr<tinkoff::CancelOrderResponse> cancelOrderResponse(new tinkoff::CancelOrderResponse());

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order is still waiting. 50/50 bought with a price 10.030 \u20BD"))
    );
    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order cancelled")));
    EXPECT_CALL(*grpcClientMock, cancelOrder(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(cancelOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(3000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, priceForBid, false
        )
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_VERBOSE,
            QString("aaaaa"),
            QString("Order to buy 50 created with a price 10.030 \u20BD while market price 10.040 \u20BD")
        )
    );

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_FILL);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order completed. 50/50 bought with a price 10.030 \u20BD"))
    );

    ASSERT_EQ(thread->buy(30000, 10000, 40000), true);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_CANCELLED);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order cancelled. 50/50 bought with a price 10.030 \u20BD"))
    );

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, priceForBid, false
        )
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_VERBOSE,
            QString("aaaaa"),
            QString("Order to buy 50 created with a price 10.030 \u20BD while market price 10.040 \u20BD")
        )
    );

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForBid))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaaaa"), tinkoff::ORDER_DIRECTION_BUY, 5, priceForBid, false
        )
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_VERBOSE,
            QString("aaaaa"),
            QString("Order to buy 50 created with a price 10.030 \u20BD while market price 10.040 \u20BD")
        )
    );

    ASSERT_EQ(thread->buy(30000, 10000, 40000), false);

    bidPrice->set_units(20);
    bidPrice->set_nano(500000000);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_DEBUG,
            QString("aaaaa"),
            QString("Trade interrupted because the price reached 20.500 \u20BD with raise +36.67% from the price 15.000 \u20BD")
        )
    );
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(nullptr));

    ASSERT_EQ(thread->buy(30000, 10000, 40000), true);
}

TEST_F(Test_TradingThread, Test_sell)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker            = "ABBA";
    instrument.name              = "Abstract Basics";
    instrument.lot               = 10;
    instrument.pricePrecision    = 3;
    instrument.minPriceIncrement = Quotation(0, 1000000);

    instruments["aaaaa"] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());

    thread->getInstrumentData();

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20)).WillOnce(Return(nullptr));

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);

    const std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBookResponse(new tinkoff::GetOrderBookResponse());

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);

    tinkoff::Order* bid1 = getOrderBookResponse->add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* bid2 = getOrderBookResponse->add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask1 = getOrderBookResponse->add_asks(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask2 = getOrderBookResponse->add_asks(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* bidPrice1 = new tinkoff::Quotation(); // bid1 will take ownership
    tinkoff::Quotation* bidPrice2 = new tinkoff::Quotation(); // bid2 will take ownership
    tinkoff::Quotation* askPrice1 = new tinkoff::Quotation(); // ask1 will take ownership
    tinkoff::Quotation* askPrice2 = new tinkoff::Quotation(); // ask2 will take ownership

    bidPrice1->set_units(10);
    bidPrice1->set_nano(20000000);
    bidPrice2->set_units(10);
    bidPrice2->set_nano(10000000);
    askPrice1->set_units(10);
    askPrice1->set_nano(30000000);
    askPrice2->set_units(10);
    askPrice2->set_nano(60000000);

    bid1->set_quantity(10);
    bid1->set_allocated_price(bidPrice1);
    bid2->set_quantity(10);
    bid2->set_allocated_price(bidPrice2);
    ask1->set_quantity(10);
    ask1->set_allocated_price(askPrice1);
    ask2->set_quantity(10);
    ask2->set_allocated_price(askPrice2);

    Quotation priceForBid(10, 20000000);
    Quotation priceForAsk(10, 60000000);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForAsk))
        .WillOnce(Return(nullptr));

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);

    const std::shared_ptr<tinkoff::GetMaxLotsResponse> getMaxLotsResponse(new tinkoff::GetMaxLotsResponse());

    tinkoff::GetMaxLotsResponse_SellLimitsView* sellLimits =
        new tinkoff::GetMaxLotsResponse_SellLimitsView(); // getMaxLotsResponse will take ownership

    sellLimits->set_sell_max_lots(0);

    getMaxLotsResponse->set_allocated_sell_limits(sellLimits);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForAsk))
        .WillOnce(Return(getMaxLotsResponse));

    ASSERT_EQ(thread->sell(50000, 10000, 40000), true);

    sellLimits->set_sell_max_lots(5);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForAsk))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(),
            QString("account-id"),
            QString("aaaaa"),
            tinkoff::ORDER_DIRECTION_SELL,
            5,
            priceForAsk,
            false
        )
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_WARNING, QString("aaaaa"), QString("Failed to create order to sell 50 with a price 10.060 \u20BD"))
    );
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForAsk))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(),
            QString("account-id"),
            QString("aaaaa"),
            tinkoff::ORDER_DIRECTION_SELL,
            5,
            priceForAsk,
            false
        )
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_WARNING, QString("aaaaa"), QString("Failed to create order to sell 50 with a price 10.060 \u20BD"))
    );
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse(new tinkoff::PostOrderResponse());

    postOrderResponse->set_order_id("order-id");
    postOrderResponse->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_REJECTED);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForAsk))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(),
            QString("account-id"),
            QString("aaaaa"),
            tinkoff::ORDER_DIRECTION_SELL,
            5,
            priceForAsk,
            false
        )
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_DEBUG, QString("aaaaa"), QString("Order to sell 50 rejected with a price 10.060 \u20BD. Let's try again")
        )
    );
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse2(new tinkoff::PostOrderResponse());

    postOrderResponse2->set_order_id("order-id");
    postOrderResponse2->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForAsk))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(),
            QString("account-id"),
            QString("aaaaa"),
            tinkoff::ORDER_DIRECTION_SELL,
            5,
            priceForAsk,
            false
        )
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_DEBUG, QString("aaaaa"), QString("Order to sell 50 rejected with a price 10.060 \u20BD. Let's try again")
        )
    );
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForAsk))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(),
            QString("account-id"),
            QString("aaaaa"),
            tinkoff::ORDER_DIRECTION_SELL,
            5,
            priceForAsk,
            false
        )
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_VERBOSE,
            QString("aaaaa"),
            QString("Order to sell 50 created with a price 10.060 \u20BD while market price 10.020 \u20BD")
        )
    );

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);

    const std::shared_ptr<tinkoff::OrderState> orderState(new tinkoff::OrderState());

    tinkoff::MoneyValue* orderPrice = new tinkoff::MoneyValue(); // orderState will take ownership

    orderPrice->set_units(10);
    orderPrice->set_nano(60000000);

    orderState->set_direction(tinkoff::ORDER_DIRECTION_SELL);
    orderState->set_lots_executed(5);
    orderState->set_lots_requested(5);
    orderState->set_allocated_initial_security_price(orderPrice);
    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_FILL);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order completed. 50/50 sold with a price 10.060 \u20BD"))
    );
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(3000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForAsk))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(),
            QString("account-id"),
            QString("aaaaa"),
            tinkoff::ORDER_DIRECTION_SELL,
            5,
            priceForAsk,
            false
        )
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_VERBOSE,
            QString("aaaaa"),
            QString("Order to sell 50 created with a price 10.060 \u20BD while market price 10.020 \u20BD")
        )
    );

    ASSERT_EQ(thread->sell(50000, 20000, 30000), false);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_PARTIALLYFILL);

    const std::shared_ptr<tinkoff::CancelOrderResponse> cancelOrderResponse(new tinkoff::CancelOrderResponse());

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order partially completed. 50/50 sold with a price 10.060 \u20BD"))
    );
    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order cancelled")));
    EXPECT_CALL(*grpcClientMock, cancelOrder(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(cancelOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(3000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForAsk))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(),
            QString("account-id"),
            QString("aaaaa"),
            tinkoff::ORDER_DIRECTION_SELL,
            5,
            priceForAsk,
            false
        )
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_VERBOSE,
            QString("aaaaa"),
            QString("Order to sell 50 created with a price 10.060 \u20BD while market price 10.020 \u20BD")
        )
    );

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_FILL);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order completed. 50/50 sold with a price 10.060 \u20BD"))
    );

    ASSERT_EQ(thread->sell(50000, 10000, 40000), true);

    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_REJECTED);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(
        *logsThreadMock,
        addLog(LOG_LEVEL_VERBOSE, QString("aaaaa"), QString("Order rejected. 50/50 sold with a price 10.060 \u20BD"))
    );

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaaaa"), priceForAsk))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(),
            QString("account-id"),
            QString("aaaaa"),
            tinkoff::ORDER_DIRECTION_SELL,
            5,
            priceForAsk,
            false
        )
    )
        .WillOnce(Return(postOrderResponse2));
    EXPECT_CALL(
        *logsThreadMock,
        addLog(
            LOG_LEVEL_VERBOSE,
            QString("aaaaa"),
            QString("Order to sell 50 created with a price 10.060 \u20BD while market price 10.020 \u20BD")
        )
    );

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);

    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaaaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());

    ASSERT_EQ(thread->sell(50000, 10000, 40000), false);
}

TEST_F(Test_TradingThread, Test_calculateBuyPrice)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker            = "ABBA";
    instrument.name              = "Abstract Basics";
    instrument.lot               = 10;
    instrument.pricePrecision    = 3;
    instrument.minPriceIncrement = Quotation(0, 1000000);

    instruments["aaaaa"] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());

    thread->getInstrumentData();

    tinkoff::GetOrderBookResponse tinkoffOrderBook1;

    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));

    // clang-format off
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook1, ASAP_MODE_NONE, false),              Quotation(0, 0));
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook1, ASAP_MODE_FOLLOW_PRICE, false),      Quotation(0, 0));
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook1, ASAP_MODE_IMMEDIATELY_TRADE, false), Quotation(0, 0));
    // clang-format on

    tinkoff::GetOrderBookResponse tinkoffOrderBook2;

    tinkoff::Order* bid2 = tinkoffOrderBook2.add_bids(); // tinkoffOrderBook2 will take ownership

    tinkoff::Quotation* bidPrice2 = new tinkoff::Quotation(); // bid will take ownership

    bidPrice2->set_units(10);
    bidPrice2->set_nano(10000000);

    bid2->set_quantity(10);
    bid2->set_allocated_price(bidPrice2);

    EXPECT_CALL(*configMock, isAdditionalGap()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAdditionalGapPercent()).WillOnce(Return(0.0f));

    // clang-format off
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook2, ASAP_MODE_NONE, false),              Quotation(10, 10000000));
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook2, ASAP_MODE_FOLLOW_PRICE, false),      Quotation(10, 10000000));
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook2, ASAP_MODE_IMMEDIATELY_TRADE, false), Quotation(10, 10000000));
    // clang-format on

    tinkoff::GetOrderBookResponse tinkoffOrderBook3;

    tinkoff::Order* ask3 = tinkoffOrderBook3.add_asks(); // tinkoffOrderBook3 will take ownership

    tinkoff::Quotation* askPrice3 = new tinkoff::Quotation(); // ask will take ownership

    askPrice3->set_units(10);
    askPrice3->set_nano(20000000);

    ask3->set_quantity(10);
    ask3->set_allocated_price(askPrice3);

    // clang-format off
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook3, ASAP_MODE_NONE, false),              Quotation(0, 0));
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook3, ASAP_MODE_FOLLOW_PRICE, false),      Quotation(0, 0));
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook3, ASAP_MODE_IMMEDIATELY_TRADE, false), Quotation(10, 20000000));
    // clang-format on

    tinkoff::GetOrderBookResponse tinkoffOrderBook4;

    tinkoff::Order* bid41 = tinkoffOrderBook4.add_bids(); // tinkoffOrderBook4 will take ownership
    tinkoff::Order* bid42 = tinkoffOrderBook4.add_bids(); // tinkoffOrderBook4 will take ownership
    tinkoff::Order* ask41 = tinkoffOrderBook4.add_asks(); // tinkoffOrderBook4 will take ownership
    tinkoff::Order* ask42 = tinkoffOrderBook4.add_asks(); // tinkoffOrderBook4 will take ownership

    tinkoff::Quotation* bidPrice41 = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* bidPrice42 = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* askPrice41 = new tinkoff::Quotation(); // ask will take ownership
    tinkoff::Quotation* askPrice42 = new tinkoff::Quotation(); // ask will take ownership

    bidPrice41->set_units(10);
    bidPrice41->set_nano(20000000);
    bidPrice42->set_units(10);
    bidPrice42->set_nano(10000000);
    askPrice41->set_units(10);
    askPrice41->set_nano(30000000);
    askPrice42->set_units(10);
    askPrice42->set_nano(60000000);

    bid41->set_quantity(10);
    bid41->set_allocated_price(bidPrice41);
    bid42->set_quantity(10);
    bid42->set_allocated_price(bidPrice42);
    ask41->set_quantity(10);
    ask41->set_allocated_price(askPrice41);
    ask42->set_quantity(10);
    ask42->set_allocated_price(askPrice42);

    // clang-format off
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook4, ASAP_MODE_NONE, false),              Quotation(10, 20000000));
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook4, ASAP_MODE_FOLLOW_PRICE, false),      Quotation(10, 20000000));
    ASSERT_EQ(thread->calculateBuyPrice(tinkoffOrderBook4, ASAP_MODE_IMMEDIATELY_TRADE, false), Quotation(10, 30000000));
    // clang-format on
}

TEST_F(Test_TradingThread, Test_calculateSellPrice)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker            = "ABBA";
    instrument.name              = "Abstract Basics";
    instrument.lot               = 10;
    instrument.pricePrecision    = 3;
    instrument.minPriceIncrement = Quotation(0, 1000000);

    instruments["aaaaa"] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());

    thread->getInstrumentData();

    tinkoff::GetOrderBookResponse tinkoffOrderBook1;

    // clang-format off
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook1, ASAP_MODE_NONE),              Quotation(0, 0));
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook1, ASAP_MODE_FOLLOW_PRICE),      Quotation(0, 0));
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook1, ASAP_MODE_IMMEDIATELY_TRADE), Quotation(0, 0));
    // clang-format on

    tinkoff::GetOrderBookResponse tinkoffOrderBook2;

    tinkoff::Order* bid2 = tinkoffOrderBook2.add_bids(); // tinkoffOrderBook2 will take ownership

    tinkoff::Quotation* bidPrice2 = new tinkoff::Quotation(); // bid will take ownership

    bidPrice2->set_units(10);
    bidPrice2->set_nano(40000000);

    bid2->set_quantity(10);
    bid2->set_allocated_price(bidPrice2);

    // clang-format off
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook2, ASAP_MODE_NONE),              Quotation(0, 0));
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook2, ASAP_MODE_FOLLOW_PRICE),      Quotation(0, 0));
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook2, ASAP_MODE_IMMEDIATELY_TRADE), Quotation(10, 40000000));
    // clang-format on

    tinkoff::GetOrderBookResponse tinkoffOrderBook3;

    tinkoff::Order* ask3 = tinkoffOrderBook3.add_asks(); // tinkoffOrderBook3 will take ownership

    tinkoff::Quotation* askPrice3 = new tinkoff::Quotation(); // ask will take ownership

    askPrice3->set_units(10);
    askPrice3->set_nano(50000000);

    ask3->set_quantity(10);
    ask3->set_allocated_price(askPrice3);

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());

    // clang-format off
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook3, ASAP_MODE_NONE),              Quotation(10, 50000000));
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook3, ASAP_MODE_FOLLOW_PRICE),      Quotation(10, 50000000));
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook3, ASAP_MODE_IMMEDIATELY_TRADE), Quotation(10, 50000000));
    // clang-format on

    tinkoff::GetOrderBookResponse tinkoffOrderBook4;

    tinkoff::Order* bid41 = tinkoffOrderBook4.add_bids(); // tinkoffOrderBook4 will take ownership
    tinkoff::Order* bid42 = tinkoffOrderBook4.add_bids(); // tinkoffOrderBook4 will take ownership
    tinkoff::Order* ask41 = tinkoffOrderBook4.add_asks(); // tinkoffOrderBook4 will take ownership
    tinkoff::Order* ask42 = tinkoffOrderBook4.add_asks(); // tinkoffOrderBook4 will take ownership

    tinkoff::Quotation* bidPrice41 = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* bidPrice42 = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* askPrice41 = new tinkoff::Quotation(); // ask will take ownership
    tinkoff::Quotation* askPrice42 = new tinkoff::Quotation(); // ask will take ownership

    bidPrice41->set_units(10);
    bidPrice41->set_nano(20000000);
    bidPrice42->set_units(10);
    bidPrice42->set_nano(10000000);
    askPrice41->set_units(10);
    askPrice41->set_nano(30000000);
    askPrice42->set_units(10);
    askPrice42->set_nano(60000000);

    bid41->set_quantity(10);
    bid41->set_allocated_price(bidPrice41);
    bid42->set_quantity(10);
    bid42->set_allocated_price(bidPrice42);
    ask41->set_quantity(10);
    ask41->set_allocated_price(askPrice41);
    ask42->set_quantity(10);
    ask42->set_allocated_price(askPrice42);

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());

    // clang-format off
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook4, ASAP_MODE_NONE),              Quotation(10, 60000000));
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook4, ASAP_MODE_FOLLOW_PRICE),      Quotation(10, 30000000));
    ASSERT_EQ(thread->calculateSellPrice(tinkoffOrderBook4, ASAP_MODE_IMMEDIATELY_TRADE), Quotation(10, 20000000));
    // clang-format on
}
