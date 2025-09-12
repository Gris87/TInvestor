#include "src/threads/autopilotdecisionmaker/autopilotdecisionmakerthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/decisions/idecisionmaker_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/grpc/igrpcretryclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



MATCHER_P(IsInstrumentSellsEqWithoutTimestamp, another, "")
{
    return arg.keys() == another.keys();
}



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_AutoPilotDecisionMakerThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock   = new StrictMock<StocksStorageMock>();
        configMock          = new StrictMock<ConfigMock>();
        decisionMakerMock   = new StrictMock<DecisionMakerMock>();
        timeUtilsMock       = new StrictMock<TimeUtilsMock>();
        grpcClientMock      = new StrictMock<GrpcClientMock>();
        grpcRetryClientMock = new StrictMock<GrpcRetryClientMock>();

        thread = new AutoPilotDecisionMakerThread(
            stocksStorageMock, configMock, decisionMakerMock, timeUtilsMock, grpcClientMock, grpcRetryClientMock
        );
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
        delete configMock;
        delete decisionMakerMock;
        delete timeUtilsMock;
        delete grpcClientMock;
        delete grpcRetryClientMock;
    }

    AutoPilotDecisionMakerThread*    thread;
    StrictMock<StocksStorageMock>*   stocksStorageMock;
    StrictMock<ConfigMock>*          configMock;
    StrictMock<DecisionMakerMock>*   decisionMakerMock;
    StrictMock<TimeUtilsMock>*       timeUtilsMock;
    StrictMock<GrpcClientMock>*      grpcClientMock;
    StrictMock<GrpcRetryClientMock>* grpcRetryClientMock;
};



TEST_F(Test_AutoPilotDecisionMakerThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_AutoPilotDecisionMakerThread, Test_run)
{
    const InSequence seq;

    thread->setAccountId("account-id");
    thread->notifyAboutSell("bbb-bbb");

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

    tinkoffQuantity2->set_units(400);
    tinkoffQuantity2->set_nano(0);

    tinkoffCurrentPrice2->set_currency("rub");
    tinkoffCurrentPrice2->set_units(1000);
    tinkoffCurrentPrice2->set_nano(0);

    tinkoffAvgPriceFifo2->set_currency("rub");
    tinkoffAvgPriceFifo2->set_units(1000);
    tinkoffAvgPriceFifo2->set_nano(0);

    tinkoffAvgPriceWavg2->set_currency("rub");
    tinkoffAvgPriceWavg2->set_units(1000);
    tinkoffAvgPriceWavg2->set_nano(0);

    tinkoffDailyYield2->set_currency("rub");
    tinkoffDailyYield2->set_units(0);
    tinkoffDailyYield2->set_nano(0);

    position2->set_instrument_uid("aaa-aaa");
    position2->set_instrument_type("share");
    position2->set_allocated_quantity(tinkoffQuantity2);
    position2->set_allocated_current_price(tinkoffCurrentPrice2);
    position2->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo2);
    position2->set_allocated_average_position_price(tinkoffAvgPriceWavg2);
    position2->set_allocated_daily_yield(tinkoffDailyYield2);

    const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> getOperationsByCursorResponse(
        new tinkoff::GetOperationsByCursorResponse()
    );
    tinkoff::OperationItem* operationItem1 =
        getOperationsByCursorResponse->add_items(); // getOperationsByCursorResponse will take ownership
    tinkoff::OperationItem* operationItem2 =
        getOperationsByCursorResponse->add_items(); // getOperationsByCursorResponse will take ownership

    google::protobuf::Timestamp* dateTimestamp1 = new google::protobuf::Timestamp(); // operationItem1 will take ownership

    dateTimestamp1->set_seconds(1704056460);
    dateTimestamp1->set_nanos(0);

    tinkoff::MoneyValue* price1 = new tinkoff::MoneyValue(); // operationItem1 will take ownership

    price1->set_units(280);
    price1->set_nano(0);

    tinkoff::MoneyValue* payment1 = new tinkoff::MoneyValue(); // operationItem1 will take ownership

    payment1->set_units(2800);
    payment1->set_nano(0);

    tinkoff::MoneyValue* commission1 = new tinkoff::MoneyValue(); // operationItem1 will take ownership

    commission1->set_units(-1);
    commission1->set_nano(-400000000);

    operationItem1->set_allocated_date(dateTimestamp1);
    operationItem1->set_type(tinkoff::OPERATION_TYPE_SELL);
    operationItem1->set_instrument_uid("aaa-aaa");
    operationItem1->set_position_uid("position-id");
    operationItem1->set_description("Sell 10 ivashka durashka shares");
    operationItem1->set_allocated_price(price1);
    operationItem1->set_quantity_done(10);
    operationItem1->set_allocated_payment(payment1);
    operationItem1->set_allocated_commission(commission1);

    google::protobuf::Timestamp* dateTimestamp2 = new google::protobuf::Timestamp(); // operationItem2 will take ownership

    dateTimestamp2->set_seconds(1704056400);
    dateTimestamp2->set_nanos(0);

    tinkoff::MoneyValue* price2 = new tinkoff::MoneyValue(); // operationItem2 will take ownership

    price2->set_units(253);
    price2->set_nano(300000000);

    tinkoff::MoneyValue* payment2 = new tinkoff::MoneyValue(); // operationItem2 will take ownership

    payment2->set_units(-2533);
    payment2->set_nano(0);

    tinkoff::MoneyValue* commission2 = new tinkoff::MoneyValue(); // operationItem2 will take ownership

    commission2->set_units(-1);
    commission2->set_nano(-266500000);

    operationItem2->set_allocated_date(dateTimestamp2);
    operationItem2->set_type(tinkoff::OPERATION_TYPE_BUY);
    operationItem2->set_instrument_uid("aaa-aaa");
    operationItem2->set_position_uid("position-id");
    operationItem2->set_description("Buy 10 ivashka durashka shares");
    operationItem2->set_allocated_price(price2);
    operationItem2->set_quantity_done(10);
    operationItem2->set_allocated_payment(payment2);
    operationItem2->set_allocated_commission(commission2);

    InstrumentSells instrumentSells;
    instrumentSells["aaa-aaa"] = 1704056460000;
    instrumentSells["bbb-bbb"] = 0;

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item1;
    PortfolioItem         item2;

    item1.instrumentId = RUBLE_UID;
    item1.showPrices   = false;
    item1.available    = 100000.0;
    item1.price        = 1.0f;
    item1.avgPriceFifo = 1.0f;
    item1.avgPriceWavg = 1.0f;
    item1.cost         = 100000.0;

    item2.instrumentId = "aaa-aaa";
    item2.showPrices   = true;
    item2.available    = 400.0;
    item2.price        = 1000.0f;
    item2.avgPriceFifo = 1000.0f;
    item2.avgPriceWavg = 1000.0f;
    item2.cost         = 400000.0;

    category1.name = "currency";
    category1.items << item1;

    category2.name = "share";
    category2.items << item2;

    portfolio.positions << category1 << category2;

    QList<Stock*> stocks;

    InstrumentsForTrading instrumentsForTrading;
    TradingInfo           tradingInfo;

    tradingInfo.asapMode     = ASAP_MODE_NONE;
    tradingInfo.avgPrice     = 1990.0f;
    tradingInfo.price        = 2000.0f;
    tradingInfo.expectedCost = 0.0;
    tradingInfo.cause        = "I want to sell";

    instrumentsForTrading["aaa-aaa"] = tradingInfo;

    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));
    EXPECT_CALL(
        *grpcClientMock,
        getOperations(QThread::currentThread(), QString("account-id"), Ge(1704056400000), Ge(1704056400000), QString(""))
    )
        .WillOnce(Return(getOperationsByCursorResponse));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(),
            Ge(1704056400000),
            configMock,
            IsInstrumentSellsEqWithoutTimestamp(instrumentSells),
            portfolio,
            stocks,
            true,
            false,
            true
        )
    )
        .WillOnce(Return(instrumentsForTrading));
    EXPECT_CALL(*stocksStorageMock, readUnlock());

    thread->run();
}

TEST_F(Test_AutoPilotDecisionMakerThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
