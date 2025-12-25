#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision3config/iselldecision3config_mock.h"
#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/grpc/igrpcretryclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/logs/ilogsthread_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"
#include "src/utils/tradeutils/itradeutils_mock.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";
const char* const CHMK_UID  = "b5e26096-d013-48e4-b2a9-2f38b6090feb";



using ::testing::_;
using ::testing::DoubleEq;
using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_BiDirTradingThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        instrumentsStorageMock  = new StrictMock<InstrumentsStorageMock>();
        userStorageMock         = new StrictMock<UserStorageMock>();
        configMock              = new StrictMock<ConfigMock>();
        timeUtilsMock           = new StrictMock<TimeUtilsMock>();
        tradeUtilsMock          = new StrictMock<TradeUtilsMock>();
        grpcClientMock          = new StrictMock<GrpcClientMock>();
        grpcRetryClientMock     = new StrictMock<GrpcRetryClientMock>();
        logsThreadMock          = new StrictMock<LogsThreadMock>();
        simulatorConfigMock     = new StrictMock<DecisionMakerConfigMock>();
        autoPilotConfigMock     = new StrictMock<DecisionMakerConfigMock>();
        sellDecision3ConfigMock = new StrictMock<SellDecision3ConfigMock>();

        stock = new Stock();

        stock->meta.instrumentId = "aaa-aaa";
        stock->meta.turnover     = 1000000;

        EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("aaa-aaa"), QString("But why")));

        thread = new BiDirTradingThread(
            instrumentsStorageMock,
            userStorageMock,
            configMock,
            timeUtilsMock,
            tradeUtilsMock,
            grpcClientMock,
            grpcRetryClientMock,
            logsThreadMock,
            "account-id",
            stock,
            BIDIR_MODE_HUGE_BID,
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
        delete tradeUtilsMock;
        delete grpcClientMock;
        delete grpcRetryClientMock;
        delete logsThreadMock;
        delete simulatorConfigMock;
        delete autoPilotConfigMock;
        delete sellDecision3ConfigMock;
        delete stock;
    }

    BiDirTradingThread*                  thread;
    StrictMock<InstrumentsStorageMock>*  instrumentsStorageMock;
    StrictMock<UserStorageMock>*         userStorageMock;
    StrictMock<ConfigMock>*              configMock;
    StrictMock<TimeUtilsMock>*           timeUtilsMock;
    StrictMock<TradeUtilsMock>*          tradeUtilsMock;
    StrictMock<GrpcClientMock>*          grpcClientMock;
    StrictMock<GrpcRetryClientMock>*     grpcRetryClientMock;
    StrictMock<LogsThreadMock>*          logsThreadMock;
    StrictMock<DecisionMakerConfigMock>* simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock>* autoPilotConfigMock;
    StrictMock<SellDecision3ConfigMock>* sellDecision3ConfigMock;
    Stock*                               stock;
};



TEST_F(Test_BiDirTradingThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirTradingThread, Test_run)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker            = "ABBA";
    instrument.name              = "Abstract Basics";
    instrument.lot               = 1;
    instrument.pricePrecision    = 2;
    instrument.minPriceIncrement = Quotation(5, 0);

    instruments["aaa-aaa"] = instrument;

    thread->terminateTrading();

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_VERBOSE, QString("aaa-aaa"), QString("Reselling completed successfully")));

    thread->run();
}

TEST_F(Test_BiDirTradingThread, Test_terminateThread)
{
    thread->terminateThread();
}

TEST_F(Test_BiDirTradingThread, Test_setMode_and_bidirMode)
{
    const InSequence seq;

    ASSERT_EQ(thread->bidirMode(), BIDIR_MODE_HUGE_BID);

    thread->setMode(BIDIR_MODE_HUGE_BID, "Huge bid is on");
    ASSERT_EQ(thread->bidirMode(), BIDIR_MODE_HUGE_BID);

    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString("aaa-aaa"), QString("Huge spread is on")));

    thread->setMode(BIDIR_MODE_HUGE_SPREAD, "Huge spread is on");
    ASSERT_EQ(thread->bidirMode(), BIDIR_MODE_HUGE_SPREAD);
}

TEST_F(Test_BiDirTradingThread, Test_trade)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker            = "ABBA";
    instrument.name              = "Abstract Basics";
    instrument.lot               = 1;
    instrument.pricePrecision    = 2;
    instrument.minPriceIncrement = Quotation(5, 0);

    instruments["aaa-aaa"] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaa-aaa"), 20)).WillOnce(Return(nullptr));

    ASSERT_EQ(thread->trade(), false);

    const std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBookResponse(new tinkoff::GetOrderBookResponse());

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaa-aaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_VERBOSE, QString("aaa-aaa"), QString("Impossible to continue reselling")));

    ASSERT_EQ(thread->trade(), true);

    tinkoff::Order* bid = getOrderBookResponse->add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask = getOrderBookResponse->add_asks(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* bidPrice = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* askPrice = new tinkoff::Quotation(); // ask will take ownership

    bidPrice->set_units(870);
    bidPrice->set_nano(0);
    askPrice->set_units(885);
    askPrice->set_nano(0);

    bid->set_quantity(100);
    bid->set_allocated_price(bidPrice);
    ask->set_quantity(100);
    ask->set_allocated_price(askPrice);

    Quotation priceForBuy(870, 0);
    Quotation priceForSell(885, 0);

    StockOperationalData stockOperationalData;
    stockOperationalData.price = 1000;

    stock->operational.detailedData << stockOperationalData;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaa-aaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(nullptr));

    ASSERT_EQ(thread->trade(), false);

    const std::shared_ptr<tinkoff::PortfolioResponse> portfolioResponse(new tinkoff::PortfolioResponse());

    tinkoff::PortfolioPosition* position1 = portfolioResponse->add_positions(); // portfolioResponse will take ownership
    tinkoff::PortfolioPosition* position2 = portfolioResponse->add_positions(); // portfolioResponse will take ownership

    tinkoff::Quotation*  tinkoffQuantity1     = new tinkoff::Quotation();  // position1 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice1 = new tinkoff::MoneyValue(); // position1 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo1 = new tinkoff::MoneyValue(); // position1 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceWavg1 = new tinkoff::MoneyValue(); // position1 will take ownership
    tinkoff::MoneyValue* tinkoffDailyYield1   = new tinkoff::MoneyValue(); // position1 will take ownership

    tinkoffQuantity1->set_units(100000);
    tinkoffQuantity1->set_nano(0);

    tinkoffCurrentPrice1->set_currency("rub");
    tinkoffCurrentPrice1->set_units(1);
    tinkoffCurrentPrice1->set_nano(0);

    tinkoffAvgPriceFifo1->set_currency("rub");
    tinkoffAvgPriceFifo1->set_units(1);
    tinkoffAvgPriceFifo1->set_nano(0);

    tinkoffAvgPriceWavg1->set_currency("rub");
    tinkoffAvgPriceWavg1->set_units(1);
    tinkoffAvgPriceWavg1->set_nano(0);

    tinkoffDailyYield1->set_currency("rub");
    tinkoffDailyYield1->set_units(0);
    tinkoffDailyYield1->set_nano(0);

    position1->set_instrument_uid(RUBLE_UID);
    position1->set_instrument_type("currency");
    position1->set_allocated_quantity(tinkoffQuantity1);
    position1->set_allocated_current_price(tinkoffCurrentPrice1);
    position1->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo1);
    position1->set_allocated_average_position_price(tinkoffAvgPriceWavg1);
    position1->set_allocated_daily_yield(tinkoffDailyYield1);

    tinkoff::Quotation*  tinkoffQuantity2     = new tinkoff::Quotation();  // position2 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice2 = new tinkoff::MoneyValue(); // position2 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo2 = new tinkoff::MoneyValue(); // position2 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceWavg2 = new tinkoff::MoneyValue(); // position2 will take ownership
    tinkoff::MoneyValue* tinkoffDailyYield2   = new tinkoff::MoneyValue(); // position2 will take ownership

    tinkoffQuantity2->set_units(4);
    tinkoffQuantity2->set_nano(0);

    tinkoffCurrentPrice2->set_currency("rub");
    tinkoffCurrentPrice2->set_units(870);
    tinkoffCurrentPrice2->set_nano(0);

    tinkoffAvgPriceFifo2->set_currency("rub");
    tinkoffAvgPriceFifo2->set_units(900);
    tinkoffAvgPriceFifo2->set_nano(0);

    tinkoffAvgPriceWavg2->set_currency("rub");
    tinkoffAvgPriceWavg2->set_units(900);
    tinkoffAvgPriceWavg2->set_nano(0);

    tinkoffDailyYield2->set_currency("rub");
    tinkoffDailyYield2->set_units(0);
    tinkoffDailyYield2->set_nano(0);

    position2->set_instrument_uid("aaa-aaa");
    position2->set_instrument_type("etf");
    position2->set_allocated_quantity(tinkoffQuantity2);
    position2->set_allocated_current_price(tinkoffCurrentPrice2);
    position2->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo2);
    position2->set_allocated_average_position_price(tinkoffAvgPriceWavg2);
    position2->set_allocated_daily_yield(tinkoffDailyYield2);

    const std::shared_ptr<tinkoff::GetMaxLotsResponse> getMaxLotsResponse(new tinkoff::GetMaxLotsResponse());

    tinkoff::GetMaxLotsResponse_BuyLimitsView* buyLimits =
        new tinkoff::GetMaxLotsResponse_BuyLimitsView(); // getMaxLotsResponse will take ownership

    buyLimits->set_buy_max_lots(5);

    getMaxLotsResponse->set_allocated_buy_limits(buyLimits);

    tinkoff::GetMaxLotsResponse_SellLimitsView* sellLimits =
        new tinkoff::GetMaxLotsResponse_SellLimitsView(); // getMaxLotsResponse will take ownership

    sellLimits->set_sell_max_lots(5);

    getMaxLotsResponse->set_allocated_sell_limits(sellLimits);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse(new tinkoff::PostOrderResponse());

    postOrderResponse->set_order_id("order-id");
    postOrderResponse->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    const std::shared_ptr<tinkoff::OrderState> orderState(new tinkoff::OrderState());

    tinkoff::MoneyValue* orderPrice = new tinkoff::MoneyValue(); // orderState will take ownership

    orderPrice->set_units(9999);
    orderPrice->set_nano(0);

    orderState->set_direction(tinkoff::ORDER_DIRECTION_BUY);
    orderState->set_lots_executed(0);
    orderState->set_lots_requested(5);
    orderState->set_allocated_initial_security_price(orderPrice);
    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    const std::shared_ptr<tinkoff::CancelOrderResponse> cancelOrderResponse(new tinkoff::CancelOrderResponse());

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaa-aaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));
    EXPECT_CALL(*timeUtilsMock, isMorningSession(_)).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*simulatorConfigMock, getSellDecision3Config()).WillOnce(Return(sellDecision3ConfigMock));
    EXPECT_CALL(*sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision3ConfigMock, getLoseYield()).WillOnce(Return(5.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitStockPurchasePart()).WillOnce(Return(2.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitStockPurchasePart()).WillOnce(Return(2.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitByTurnover()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitByTurnoverPercent()).WillOnce(Return(1.0f));
    EXPECT_CALL(
        *tradeUtilsMock,
        calculateAmountOfLotsToBuy(
            true,
            DoubleEq(2.0f),
            true,
            DoubleEq(1.0f),
            DoubleEq(103600.0),
            DoubleEq(103600.0),
            1000000,
            DoubleEq(870),
            DoubleEq(870)
        )
    )
        .WillOnce(Return(20));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), priceForBuy))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_BUY, 5, priceForBuy
        )
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), priceForSell))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(
            QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_SELL, 5, priceForSell
        )
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(30000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaa-aaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(*grpcClientMock, getOrderState(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(orderState));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));
    EXPECT_CALL(*timeUtilsMock, isMorningSession(_)).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*simulatorConfigMock, getSellDecision3Config()).WillOnce(Return(sellDecision3ConfigMock));
    EXPECT_CALL(*sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision3ConfigMock, getLoseYield()).WillOnce(Return(5.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitStockPurchasePart()).WillOnce(Return(2.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitStockPurchasePart()).WillOnce(Return(2.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitByTurnover()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitByTurnoverPercent()).WillOnce(Return(1.0f));
    EXPECT_CALL(
        *tradeUtilsMock,
        calculateAmountOfLotsToBuy(
            true,
            DoubleEq(2.0f),
            true,
            DoubleEq(1.0f),
            DoubleEq(103600.0),
            DoubleEq(103600.0),
            1000000,
            DoubleEq(870),
            DoubleEq(870)
        )
    )
        .WillOnce(Return(20));
    EXPECT_CALL(*grpcClientMock, cancelOrder(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(cancelOrderResponse));
    EXPECT_CALL(*grpcClientMock, cancelOrder(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(cancelOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(3000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(thread->trade(), false);

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getOrderBook(QThread::currentThread(), QString("aaa-aaa"), 20))
        .WillOnce(Return(getOrderBookResponse));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));
    EXPECT_CALL(*timeUtilsMock, isMorningSession(_)).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getAutoPilotConfig()).WillOnce(Return(autoPilotConfigMock));
    EXPECT_CALL(*autoPilotConfigMock, getSellDecision3Config()).WillOnce(Return(sellDecision3ConfigMock));
    EXPECT_CALL(*sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision3ConfigMock, getLoseYield()).WillOnce(Return(5.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitStockPurchasePart()).WillOnce(Return(2.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitStockPurchasePart()).WillOnce(Return(2.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitByTurnover()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitByTurnoverPercent()).WillOnce(Return(1.0f));
    EXPECT_CALL(
        *tradeUtilsMock,
        calculateAmountOfLotsToBuy(
            true,
            DoubleEq(2.0f),
            true,
            DoubleEq(1.0f),
            DoubleEq(103600.0),
            DoubleEq(103600.0),
            1000000,
            DoubleEq(870),
            DoubleEq(870)
        )
    )
        .WillOnce(Return(20));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), priceForBuy))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), priceForSell))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(30000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(thread->trade(), false);
}

TEST_F(Test_BiDirTradingThread, Test_checkIfNeedToCancelAndCreateOrder)
{
    const std::shared_ptr<tinkoff::OrderState> orderState(new tinkoff::OrderState());

    tinkoff::MoneyValue* orderPrice = new tinkoff::MoneyValue(); // orderState will take ownership

    orderPrice->set_units(870);
    orderPrice->set_nano(0);

    orderState->set_direction(tinkoff::ORDER_DIRECTION_BUY);
    orderState->set_lots_executed(0);
    orderState->set_lots_requested(5);
    orderState->set_allocated_initial_security_price(orderPrice);
    orderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    Quotation price(870, 0);

    bool needToCancel = false;
    bool needToOrder  = false;

    thread->checkIfNeedToCancelAndCreateOrder(orderState, 5, price, needToCancel, needToOrder);

    // clang-format off
    ASSERT_EQ(needToCancel, false);
    ASSERT_EQ(needToOrder,  false);
    // clang-format on

    thread->checkIfNeedToCancelAndCreateOrder(orderState, 10, price, needToCancel, needToOrder);

    // clang-format off
    ASSERT_EQ(needToCancel, true);
    ASSERT_EQ(needToOrder,  true);
    // clang-format on

    thread->checkIfNeedToCancelAndCreateOrder(nullptr, 5, price, needToCancel, needToOrder);

    // clang-format off
    ASSERT_EQ(needToCancel, false);
    ASSERT_EQ(needToOrder,  true);
    // clang-format on
}

TEST_F(Test_BiDirTradingThread, Test_buyWithPrice)
{
    const InSequence seq;

    const std::shared_ptr<tinkoff::GetMaxLotsResponse> getMaxLotsResponse(new tinkoff::GetMaxLotsResponse());

    tinkoff::GetMaxLotsResponse_BuyLimitsView* buyLimits =
        new tinkoff::GetMaxLotsResponse_BuyLimitsView(); // getMaxLotsResponse will take ownership

    buyLimits->set_buy_max_lots(0);

    getMaxLotsResponse->set_allocated_buy_limits(buyLimits);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse(new tinkoff::PostOrderResponse());

    postOrderResponse->set_order_id("order-id");
    postOrderResponse->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    const std::shared_ptr<tinkoff::CancelOrderResponse> cancelOrderResponse(new tinkoff::CancelOrderResponse());

    Quotation price(870, 0);

    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(nullptr));

    thread->buyWithPrice(5, price);

    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));

    thread->buyWithPrice(5, price);

    buyLimits->set_buy_max_lots(5);

    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    thread->buyWithPrice(5, price);

    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(postOrderResponse));

    thread->buyWithPrice(5, price);

    postOrderResponse->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_REJECTED);

    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_BUY, 5, price)
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    thread->buyWithPrice(5, price);

    EXPECT_CALL(*grpcClientMock, cancelOrder(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(cancelOrderResponse));
}

TEST_F(Test_BiDirTradingThread, Test_sellWithPrice)
{
    const InSequence seq;

    const std::shared_ptr<tinkoff::GetMaxLotsResponse> getMaxLotsResponse(new tinkoff::GetMaxLotsResponse());

    tinkoff::GetMaxLotsResponse_SellLimitsView* sellLimits =
        new tinkoff::GetMaxLotsResponse_SellLimitsView(); // getMaxLotsResponse will take ownership

    sellLimits->set_sell_max_lots(0);

    getMaxLotsResponse->set_allocated_sell_limits(sellLimits);

    const std::shared_ptr<tinkoff::PostOrderResponse> postOrderResponse(new tinkoff::PostOrderResponse());

    postOrderResponse->set_order_id("order-id");
    postOrderResponse->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    const std::shared_ptr<tinkoff::CancelOrderResponse> cancelOrderResponse(new tinkoff::CancelOrderResponse());

    Quotation price(870, 0);

    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(nullptr));

    thread->sellWithPrice(price);

    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));

    thread->sellWithPrice(price);

    sellLimits->set_sell_max_lots(5);

    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    thread->sellWithPrice(price);

    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(postOrderResponse));

    thread->sellWithPrice(price);

    postOrderResponse->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_REJECTED);

    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getMaxLots(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), price))
        .WillOnce(Return(getMaxLotsResponse));
    EXPECT_CALL(
        *grpcClientMock,
        postOrder(QThread::currentThread(), QString("account-id"), QString("aaa-aaa"), tinkoff::ORDER_DIRECTION_SELL, 5, price)
    )
        .WillOnce(Return(postOrderResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    thread->sellWithPrice(price);

    EXPECT_CALL(*grpcClientMock, cancelOrder(QThread::currentThread(), QString("account-id"), QString("order-id")))
        .WillOnce(Return(cancelOrderResponse));
}

TEST_F(Test_BiDirTradingThread, Test_isNeedToSellAsap)
{
    const InSequence seq;

    EXPECT_CALL(*timeUtilsMock, isMorningSession(1704056400000)).WillOnce(Return(true));

    ASSERT_EQ(thread->isNeedToSellAsap(1704056400000, BIDIR_MODE_HUGE_BID, 2.0f, 0.0f, 0.04f), false);

    EXPECT_CALL(*timeUtilsMock, isMorningSession(1704056400000)).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*simulatorConfigMock, getSellDecision3Config()).WillOnce(Return(sellDecision3ConfigMock));
    EXPECT_CALL(*sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision3ConfigMock, getLoseYield()).WillOnce(Return(5.0f));

    ASSERT_EQ(thread->isNeedToSellAsap(1704056400000, BIDIR_MODE_HUGE_BID, 90.0f, -4.93f, 0.04f), true);

    EXPECT_CALL(*timeUtilsMock, isMorningSession(1704056400000)).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*simulatorConfigMock, getSellDecision3Config()).WillOnce(Return(sellDecision3ConfigMock));
    EXPECT_CALL(*sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision3ConfigMock, getLoseYield()).WillOnce(Return(5.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitStockPurchasePart()).WillOnce(Return(2.0f));

    ASSERT_EQ(thread->isNeedToSellAsap(1704056400000, BIDIR_MODE_HUGE_BID, 90.0f, -4.92f, 0.04f), false);

    EXPECT_CALL(*timeUtilsMock, isMorningSession(1704056400000)).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*simulatorConfigMock, getSellDecision3Config()).WillOnce(Return(sellDecision3ConfigMock));
    EXPECT_CALL(*sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision3ConfigMock, getLoseYield()).WillOnce(Return(5.0f));
    EXPECT_CALL(*configMock, isHugeBidLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeBidLimitStockPurchasePart()).WillOnce(Return(2.0f));

    ASSERT_EQ(thread->isNeedToSellAsap(1704056400000, BIDIR_MODE_HUGE_BID, 1.0f, -4.92f, 0.04f), true);

    EXPECT_CALL(*timeUtilsMock, isMorningSession(1704056400000)).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*simulatorConfigMock, getSellDecision3Config()).WillOnce(Return(sellDecision3ConfigMock));
    EXPECT_CALL(*sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision3ConfigMock, getLoseYield()).WillOnce(Return(5.0f));
    EXPECT_CALL(*configMock, isHugeSpreadLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeSpreadLimitStockPurchasePart()).WillOnce(Return(2.0f));

    ASSERT_EQ(thread->isNeedToSellAsap(1704056400000, BIDIR_MODE_HUGE_SPREAD, 90.0f, -4.92f, 0.04f), false);

    EXPECT_CALL(*timeUtilsMock, isMorningSession(1704056400000)).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*simulatorConfigMock, getSellDecision3Config()).WillOnce(Return(sellDecision3ConfigMock));
    EXPECT_CALL(*sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision3ConfigMock, getLoseYield()).WillOnce(Return(5.0f));
    EXPECT_CALL(*configMock, isHugeSpreadLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeSpreadLimitStockPurchasePart()).WillOnce(Return(2.0f));

    ASSERT_EQ(thread->isNeedToSellAsap(1704056400000, BIDIR_MODE_HUGE_SPREAD, 1.0f, -4.92f, 0.04f), true);
}

TEST_F(Test_BiDirTradingThread, Test_removeOwnOrdersFromOrderBook)
{
    tinkoff::GetOrderBookResponse getOrderBookResponse;

    tinkoff::Order* bid1 = getOrderBookResponse.add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* bid2 = getOrderBookResponse.add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask1 = getOrderBookResponse.add_asks(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask2 = getOrderBookResponse.add_asks(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* bidPrice1 = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* bidPrice2 = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* askPrice1 = new tinkoff::Quotation(); // ask will take ownership
    tinkoff::Quotation* askPrice2 = new tinkoff::Quotation(); // ask will take ownership

    bidPrice1->set_units(870);
    bidPrice1->set_nano(0);
    bidPrice2->set_units(875);
    bidPrice2->set_nano(0);
    askPrice1->set_units(885);
    askPrice1->set_nano(0);
    askPrice2->set_units(890);
    askPrice2->set_nano(0);

    bid1->set_quantity(100);
    bid1->set_allocated_price(bidPrice1);
    bid2->set_quantity(200);
    bid2->set_allocated_price(bidPrice2);
    ask1->set_quantity(300);
    ask1->set_allocated_price(askPrice1);
    ask2->set_quantity(400);
    ask2->set_allocated_price(askPrice2);

    const std::shared_ptr<tinkoff::OrderState> buyOrderState(new tinkoff::OrderState());
    const std::shared_ptr<tinkoff::OrderState> sellOrderState(new tinkoff::OrderState());

    tinkoff::MoneyValue* buyOrderPrice  = new tinkoff::MoneyValue(); // buyOrderState will take ownership
    tinkoff::MoneyValue* sellOrderPrice = new tinkoff::MoneyValue(); // sellOrderState will take ownership

    buyOrderPrice->set_units(875);
    buyOrderPrice->set_nano(0);
    sellOrderPrice->set_units(890);
    sellOrderPrice->set_nano(0);

    buyOrderState->set_direction(tinkoff::ORDER_DIRECTION_BUY);
    buyOrderState->set_lots_executed(0);
    buyOrderState->set_lots_requested(15);
    buyOrderState->set_allocated_initial_security_price(buyOrderPrice);
    buyOrderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    sellOrderState->set_direction(tinkoff::ORDER_DIRECTION_SELL);
    sellOrderState->set_lots_executed(0);
    sellOrderState->set_lots_requested(5);
    sellOrderState->set_allocated_initial_security_price(sellOrderPrice);
    sellOrderState->set_execution_report_status(tinkoff::EXECUTION_REPORT_STATUS_NEW);

    // clang-format off
    ASSERT_EQ(getOrderBookResponse.bids_size(),             2);
    ASSERT_EQ(getOrderBookResponse.bids(0).quantity(),      100);
    ASSERT_EQ(getOrderBookResponse.bids(0).price().units(), 870);
    ASSERT_EQ(getOrderBookResponse.bids(0).price().nano(),  0);
    ASSERT_EQ(getOrderBookResponse.bids(1).quantity(),      200);
    ASSERT_EQ(getOrderBookResponse.bids(1).price().units(), 875);
    ASSERT_EQ(getOrderBookResponse.bids(1).price().nano(),  0);
    ASSERT_EQ(getOrderBookResponse.asks_size(),             2);
    ASSERT_EQ(getOrderBookResponse.asks(0).quantity(),      300);
    ASSERT_EQ(getOrderBookResponse.asks(0).price().units(), 885);
    ASSERT_EQ(getOrderBookResponse.asks(0).price().nano(),  0);
    ASSERT_EQ(getOrderBookResponse.asks(1).quantity(),      400);
    ASSERT_EQ(getOrderBookResponse.asks(1).price().units(), 890);
    ASSERT_EQ(getOrderBookResponse.asks(1).price().nano(),  0);
    // clang-format on

    thread->removeOwnOrdersFromOrderBook(getOrderBookResponse, buyOrderState, sellOrderState);

    // clang-format off
    ASSERT_EQ(getOrderBookResponse.bids_size(),             2);
    ASSERT_EQ(getOrderBookResponse.bids(0).quantity(),      100);
    ASSERT_EQ(getOrderBookResponse.bids(0).price().units(), 870);
    ASSERT_EQ(getOrderBookResponse.bids(0).price().nano(),  0);
    ASSERT_EQ(getOrderBookResponse.bids(1).quantity(),      185);
    ASSERT_EQ(getOrderBookResponse.bids(1).price().units(), 875);
    ASSERT_EQ(getOrderBookResponse.bids(1).price().nano(),  0);
    ASSERT_EQ(getOrderBookResponse.asks_size(),             2);
    ASSERT_EQ(getOrderBookResponse.asks(0).quantity(),      300);
    ASSERT_EQ(getOrderBookResponse.asks(0).price().units(), 885);
    ASSERT_EQ(getOrderBookResponse.asks(0).price().nano(),  0);
    ASSERT_EQ(getOrderBookResponse.asks(1).quantity(),      395);
    ASSERT_EQ(getOrderBookResponse.asks(1).price().units(), 890);
    ASSERT_EQ(getOrderBookResponse.asks(1).price().nano(),  0);
    // clang-format on
}

TEST_F(Test_BiDirTradingThread, Test_calculateBuyPrice)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker            = "ABBA";
    instrument.name              = "Abstract Basics";
    instrument.lot               = 1;
    instrument.pricePrecision    = 2;
    instrument.minPriceIncrement = Quotation(5, 0);

    instruments["aaa-aaa"] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());

    thread->getInstrumentData();

    tinkoff::GetOrderBookResponse getOrderBookResponse;

    tinkoff::Order* bid1 = getOrderBookResponse.add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* bid2 = getOrderBookResponse.add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask1 = getOrderBookResponse.add_asks(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask2 = getOrderBookResponse.add_asks(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* bidPrice1 = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* bidPrice2 = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* askPrice1 = new tinkoff::Quotation(); // ask will take ownership
    tinkoff::Quotation* askPrice2 = new tinkoff::Quotation(); // ask will take ownership

    bidPrice1->set_units(860);
    bidPrice1->set_nano(0);
    bidPrice2->set_units(855);
    bidPrice2->set_nano(0);
    askPrice1->set_units(900);
    askPrice1->set_nano(0);
    askPrice2->set_units(905);
    askPrice2->set_nano(0);

    bid1->set_quantity(100);
    bid1->set_allocated_price(bidPrice1);
    bid2->set_quantity(200);
    bid2->set_allocated_price(bidPrice2);
    ask1->set_quantity(300);
    ask1->set_allocated_price(askPrice1);
    ask2->set_quantity(400);
    ask2->set_allocated_price(askPrice2);

    ASSERT_EQ(thread->calculateBuyPrice(getOrderBookResponse, BIDIR_MODE_HUGE_BID, 100), Quotation(860, 0));
    ASSERT_EQ(thread->calculateBuyPrice(getOrderBookResponse, BIDIR_MODE_HUGE_BID, 200), Quotation(855, 0));

    EXPECT_CALL(*configMock, getHugeSpread()).WillOnce(Return(0.7f));

    ASSERT_EQ(thread->calculateBuyPrice(getOrderBookResponse, BIDIR_MODE_HUGE_SPREAD, 100), Quotation(860, 0));

    EXPECT_CALL(*configMock, getHugeSpread()).WillOnce(Return(0.7f));

    ASSERT_EQ(thread->calculateBuyPrice(getOrderBookResponse, BIDIR_MODE_HUGE_SPREAD, 500), Quotation(855, 0));

    delete thread;

    stock->meta.instrumentId = CHMK_UID;
    stock->meta.turnover     = 1000000;

    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_DEBUG, QString(CHMK_UID), QString("But why")));

    thread = new BiDirTradingThread(
        instrumentsStorageMock,
        userStorageMock,
        configMock,
        timeUtilsMock,
        tradeUtilsMock,
        grpcClientMock,
        grpcRetryClientMock,
        logsThreadMock,
        "account-id",
        stock,
        BIDIR_MODE_HUGE_BID,
        "But why"
    );

    instruments[CHMK_UID] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());

    thread->getInstrumentData();

    ASSERT_EQ(thread->calculateBuyPrice(getOrderBookResponse, BIDIR_MODE_HUGE_BID, 100), Quotation(860, 0));
    ASSERT_EQ(thread->calculateBuyPrice(getOrderBookResponse, BIDIR_MODE_HUGE_BID, 500), Quotation(855, 0));
}

TEST_F(Test_BiDirTradingThread, Test_calculateSellPrice)
{
    const InSequence seq;

    Instruments instruments;
    Instrument  instrument;

    instrument.ticker            = "ABBA";
    instrument.name              = "Abstract Basics";
    instrument.lot               = 1;
    instrument.pricePrecision    = 2;
    instrument.minPriceIncrement = Quotation(5, 0);

    instruments["aaa-aaa"] = instrument;

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());

    thread->getInstrumentData();

    tinkoff::GetOrderBookResponse getOrderBookResponse;

    tinkoff::Order* bid1 = getOrderBookResponse.add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* bid2 = getOrderBookResponse.add_bids(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask1 = getOrderBookResponse.add_asks(); // getOrderBookResponse will take ownership
    tinkoff::Order* ask2 = getOrderBookResponse.add_asks(); // getOrderBookResponse will take ownership

    tinkoff::Quotation* bidPrice1 = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* bidPrice2 = new tinkoff::Quotation(); // bid will take ownership
    tinkoff::Quotation* askPrice1 = new tinkoff::Quotation(); // ask will take ownership
    tinkoff::Quotation* askPrice2 = new tinkoff::Quotation(); // ask will take ownership

    bidPrice1->set_units(860);
    bidPrice1->set_nano(0);
    bidPrice2->set_units(855);
    bidPrice2->set_nano(0);
    askPrice1->set_units(900);
    askPrice1->set_nano(0);
    askPrice2->set_units(905);
    askPrice2->set_nano(0);

    bid1->set_quantity(100);
    bid1->set_allocated_price(bidPrice1);
    bid2->set_quantity(200);
    bid2->set_allocated_price(bidPrice2);
    ask1->set_quantity(300);
    ask1->set_allocated_price(askPrice1);
    ask2->set_quantity(400);
    ask2->set_allocated_price(askPrice2);

    EXPECT_CALL(*timeUtilsMock, isMorningSession(_)).WillOnce(Return(true));

    ASSERT_EQ(
        thread->calculateSellPrice(getOrderBookResponse, BIDIR_MODE_HUGE_BID, 100000.0, 10000.0, 870.0, 0.04f), Quotation(900, 0)
    );

    EXPECT_CALL(*timeUtilsMock, isMorningSession(_)).WillOnce(Return(true));

    ASSERT_EQ(
        thread->calculateSellPrice(getOrderBookResponse, BIDIR_MODE_HUGE_BID, 100000.0, 10000.0, 899.0, 0.04f), Quotation(905, 0)
    );
}
