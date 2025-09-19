#include "src/threads/highliquidity/highliquiditythread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/grpc/igrpcretryclient_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";
const char* const TMON_UID  = "498ec3ff-ef27-4729-9703-a5aac48d5789";



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_HighLiquidityThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        configMock          = new StrictMock<ConfigMock>();
        timeUtilsMock       = new StrictMock<TimeUtilsMock>();
        grpcClientMock      = new StrictMock<GrpcClientMock>();
        grpcRetryClientMock = new StrictMock<GrpcRetryClientMock>();

        thread = new HighLiquidityThread(configMock, timeUtilsMock, grpcClientMock, grpcRetryClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete configMock;
        delete timeUtilsMock;
        delete grpcClientMock;
        delete grpcRetryClientMock;
    }

    HighLiquidityThread*             thread;
    StrictMock<ConfigMock>*          configMock;
    StrictMock<TimeUtilsMock>*       timeUtilsMock;
    StrictMock<GrpcClientMock>*      grpcClientMock;
    StrictMock<GrpcRetryClientMock>* grpcRetryClientMock;
};



TEST_F(Test_HighLiquidityThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_HighLiquidityThread, Test_run)
{
    const InSequence seq;

    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(false));
    EXPECT_CALL(*timeUtilsMock, isWorkingHours(Ge(1704056400000))).WillOnce(Return(false));

    thread->run();
}

TEST_F(Test_HighLiquidityThread, Test_makeDecisionBaseOnTimestamp)
{
    const InSequence seq;

    thread->setAccountId("account-id");

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

    position2->set_instrument_uid(TMON_UID);
    position2->set_instrument_type("etf");
    position2->set_allocated_quantity(tinkoffQuantity2);
    position2->set_allocated_current_price(tinkoffCurrentPrice2);
    position2->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo2);
    position2->set_allocated_average_position_price(tinkoffAvgPriceWavg2);
    position2->set_allocated_daily_yield(tinkoffDailyYield2);

    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));

    thread->makeDecisionBaseOnTimestamp(1704092340000); // 09:59 MSK

    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));

    thread->makeDecisionBaseOnTimestamp(1704542400000); // 15:00 MSK Sat

    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));

    thread->makeDecisionBaseOnTimestamp(1704628800000); // 15:00 MSK Sun

    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));

    thread->makeDecisionBaseOnTimestamp(1704092400000); // 10:00 MSK

    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));

    thread->makeDecisionBaseOnTimestamp(1704141540000); // 23:39 MSK

    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));
    EXPECT_CALL(*configMock, getLiquidityEtfRemainedPartNightly()).WillOnce(Return(1.0f));

    thread->makeDecisionBaseOnTimestamp(1704141600000); // 23:40 MSK

    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(false));
    EXPECT_CALL(*timeUtilsMock, isWorkingHours(1704122940000)).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));

    thread->makeDecisionBaseOnTimestamp(1704122940000); // 18:29 MSK

    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(false));
    EXPECT_CALL(*timeUtilsMock, isWorkingHours(1704123000000)).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(false));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));
    EXPECT_CALL(*configMock, getLiquidityEtfRemainedPartNightly()).WillOnce(Return(1.0f));

    thread->makeDecisionBaseOnTimestamp(1704123000000); // 18:30 MSK
}

TEST_F(Test_HighLiquidityThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
