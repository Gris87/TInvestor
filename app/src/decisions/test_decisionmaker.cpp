#include "src/decisions/decisionmaker.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/iconfig_mock.h"
#include "src/decisions/iactiondecision_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
class Test_DecisionMaker : public ::testing::Test
{
protected:
    void SetUp() override
    {
        configMock             = new StrictMock<ConfigMock>();
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        userStorageMock        = new StrictMock<UserStorageMock>();
        buyDecisionMock        = new StrictMock<ActionDecisionMock>();
        sellDecisionMock       = new StrictMock<ActionDecisionMock>();
        simulatorConfigMock    = new StrictMock<DecisionMakerConfigMock>();
        autoPilotConfigMock    = new StrictMock<DecisionMakerConfigMock>();

        decisionMaker = new DecisionMaker(
            instrumentsStorageMock,
            userStorageMock,
            QList<IActionDecision*>() << buyDecisionMock,
            QList<IActionDecision*>() << sellDecisionMock
        );
    }

    void TearDown() override
    {
        delete decisionMaker;
        delete configMock;
        delete instrumentsStorageMock;
        delete userStorageMock;
        delete buyDecisionMock;
        delete sellDecisionMock;
        delete simulatorConfigMock;
        delete autoPilotConfigMock;
    }

    DecisionMaker*                       decisionMaker;
    StrictMock<ConfigMock>*              configMock;
    StrictMock<InstrumentsStorageMock>*  instrumentsStorageMock;
    StrictMock<UserStorageMock>*         userStorageMock;
    StrictMock<ActionDecisionMock>*      buyDecisionMock;
    StrictMock<ActionDecisionMock>*      sellDecisionMock;
    StrictMock<DecisionMakerConfigMock>* simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock>* autoPilotConfigMock;
};



TEST_F(Test_DecisionMaker, Test_constructor_and_destructor)
{
}

TEST_F(Test_DecisionMaker, Test_makeDecision)
{
    const InSequence seq;

    InstrumentSells instrumentSells;

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item1;
    PortfolioItem         item2;
    PortfolioItem         item3;

    item1.instrumentId       = RUBLE_UID;
    item1.instrumentLogo     = nullptr;
    item1.instrumentTicker   = "RUBLE";
    item1.instrumentName     = "Ruble";
    item1.showPrices         = false;
    item1.available          = 100000.12;
    item1.price              = 1.0f;
    item1.avgPriceFifo       = 1.0f;
    item1.avgPriceWavg       = 1.0f;
    item1.cost               = 100000.0;
    item1.part               = 10.0;
    item1.yield              = 0.0f;
    item1.yieldPercent       = 0.0f;
    item1.dailyYield         = 0.0f;
    item1.priceForDailyYield = 0.0f;
    item1.costForDailyYield  = 0.0;
    item1.dailyYieldPercent  = 0.0f;
    item1.pricePrecision     = 2;

    item2.instrumentId       = "aaaaa";
    item2.instrumentLogo     = nullptr;
    item2.instrumentTicker   = "ABBA";
    item2.instrumentName     = "Abstract Basics";
    item2.showPrices         = true;
    item2.available          = 100.0;
    item2.price              = 101.0f;
    item2.avgPriceFifo       = 102.0f;
    item2.avgPriceWavg       = 103.0f;
    item2.cost               = 400000.0;
    item2.part               = 40.0;
    item2.yield              = -105.0f;
    item2.yieldPercent       = -106.0f;
    item2.dailyYield         = 107.0f;
    item2.priceForDailyYield = 108.0f;
    item2.costForDailyYield  = 109.0;
    item2.dailyYieldPercent  = -110.0f;
    item2.pricePrecision     = 3;

    item3.instrumentId       = "bbbbb";
    item3.instrumentLogo     = nullptr;
    item3.instrumentTicker   = "BASE";
    item3.instrumentName     = "Basketball enhancement";
    item3.showPrices         = true;
    item3.available          = 200.0;
    item3.price              = 201.0f;
    item3.avgPriceFifo       = 202.0f;
    item3.avgPriceWavg       = 203.0f;
    item3.cost               = 500000.0;
    item3.part               = 50.0;
    item3.yield              = 205.0f;
    item3.yieldPercent       = 206.0f;
    item3.dailyYield         = 207.0f;
    item3.priceForDailyYield = 208.0f;
    item3.costForDailyYield  = 209.0;
    item3.dailyYieldPercent  = 210.0f;
    item3.pricePrecision     = 4;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 100000.0;
    category1.part = 10.0;
    category1.items << item1;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 900000.0;
    category2.part = 90.0;
    category2.items << item2 << item3;

    portfolio.positions << category1 << category2;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    StockData stockData1;
    StockData stockData2;
    StockData stockData3;

    stockData1.timestamp = 1704110400000;
    stockData2.timestamp = 1704110400000;
    stockData3.timestamp = 1704110400000;

    stockData1.quantity = 1;
    stockData2.quantity = 2;
    stockData3.quantity = 3;

    stockData1.price = 0.1f;
    stockData2.price = 0.2f;
    stockData3.price = 0.3f;

    StockOperationalData stockOperationalData1;
    StockOperationalData stockOperationalData2;
    StockOperationalData stockOperationalData3;

    stockOperationalData1.timestamp = 1704110400000;
    stockOperationalData2.timestamp = 1704110400000;
    stockOperationalData3.timestamp = 1704110400000;

    stockOperationalData1.price = 0.1f;
    stockOperationalData2.price = 0.2f;
    stockOperationalData3.price = 0.3f;

    stock1.meta.instrumentId        = "aaaaa";
    stock1.meta.instrumentTicker    = "ABBA";
    stock1.meta.instrumentName      = "Abstract Basics";
    stock1.meta.forQualInvestorFlag = false;
    stock1.meta.minPriceIncrement   = 0.1f;
    stock1.meta.turnover            = 2000000;
    stock1.meta.pricePrecision      = 1;
    stock1.data << stockData1;
    stock1.operational.detailedData << stockOperationalData1;
    stock2.meta.instrumentId        = "bbbbb";
    stock2.meta.instrumentTicker    = "BASE";
    stock2.meta.instrumentName      = "Basketball enhancement";
    stock2.meta.forQualInvestorFlag = false;
    stock2.meta.minPriceIncrement   = 0.01f;
    stock2.meta.turnover            = 5000000;
    stock2.meta.pricePrecision      = 2;
    stock2.data << stockData2;
    stock2.operational.detailedData << stockOperationalData2;
    stock3.meta.instrumentId        = "ccccc";
    stock3.meta.instrumentTicker    = "HNYA";
    stock3.meta.instrumentName      = "The best of the best";
    stock3.meta.forQualInvestorFlag = false;
    stock3.meta.minPriceIncrement   = 0.001f;
    stock3.meta.turnover            = 9000000000;
    stock3.meta.pricePrecision      = 3;
    stock3.data << stockData3;
    stock3.operational.detailedData << stockOperationalData3;

    stocks << &stock1 << &stock2 << &stock3;

    Instruments instruments;
    Instrument  instrument1;
    Instrument  instrument2;
    Instrument  instrument3;

    instrument1.ticker                  = "ABBA";
    instrument1.name                    = "Abstract Basics";
    instrument1.lot                     = 5;
    instrument1.pricePrecision          = 1;
    instrument1.minPriceIncrement.units = 0;
    instrument1.minPriceIncrement.nano  = 100000000;

    instrument2.ticker                  = "BASE";
    instrument2.name                    = "Basketball enhancement";
    instrument2.lot                     = 10;
    instrument2.pricePrecision          = 2;
    instrument2.minPriceIncrement.units = 0;
    instrument2.minPriceIncrement.nano  = 10000000;

    instrument3.ticker                  = "HNYA";
    instrument3.name                    = "The best of the best";
    instrument3.lot                     = 100;
    instrument3.pricePrecision          = 3;
    instrument3.minPriceIncrement.units = 0;
    instrument3.minPriceIncrement.nano  = 5000000;

    instruments["aaaaa"] = instrument1;
    instruments["bbbbb"] = instrument2;
    instruments["ccccc"] = instrument3;

    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getScheduleStartHour()).WillOnce(Return(10));
    EXPECT_CALL(*configMock, getScheduleStartMinute()).WillOnce(Return(0));
    EXPECT_CALL(*configMock, getScheduleEndHour()).WillOnce(Return(18));
    EXPECT_CALL(*configMock, getScheduleEndMinute()).WillOnce(Return(40));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(
        *sellDecisionMock,
        makeDecision(
            QThread::currentThread(), simulatorConfigMock, 0, &stock1, false, -1, FloatEq(0.1f), FloatEq(103.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return(""));
    EXPECT_CALL(
        *sellDecisionMock,
        makeDecision(
            QThread::currentThread(), simulatorConfigMock, 0, &stock2, false, -1, FloatEq(0.2f), FloatEq(203.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return("I want to sell"));
    EXPECT_CALL(
        *buyDecisionMock,
        makeDecision(
            QThread::currentThread(), simulatorConfigMock, 0, &stock3, false, -1, FloatEq(0.3f), FloatEq(-1.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return("I want to buy"));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*configMock, isLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isLimitByTurnover()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLimitStockPurchasePart()).WillOnce(Return(7.0f));
    EXPECT_CALL(*configMock, getLimitByTurnoverPercent()).WillOnce(Return(0.0005f));

    InstrumentsForTrading result = decisionMaker->makeDecision(
        QThread::currentThread(), 1704110400000, configMock, instrumentSells, portfolio, stocks, false, 0, false, false
    );

    // clang-format off
    ASSERT_EQ(result.size(),                  2);
    ASSERT_EQ(result.contains("ccccc"),       true);
    ASSERT_EQ(result["ccccc"].asap,           false);
    ASSERT_NEAR(result["ccccc"].avgPrice,     -1.0f, 0.0001f);
    ASSERT_NEAR(result["ccccc"].price,        0.3f, 0.0001f);
    ASSERT_NEAR(result["ccccc"].expectedCost, 45000.00286, 0.0001);
    ASSERT_EQ(result["ccccc"].cause,          "I want to buy");
    ASSERT_EQ(result.contains("bbbbb"),       true);
    ASSERT_EQ(result["bbbbb"].asap,           false);
    ASSERT_NEAR(result["bbbbb"].avgPrice,     203.0f, 0.0001f);
    ASSERT_NEAR(result["bbbbb"].price,        0.2f, 0.0001f);
    ASSERT_NEAR(result["bbbbb"].expectedCost, 0.0f, 0.0001f);
    ASSERT_EQ(result["bbbbb"].cause,          "I want to sell");
    // clang-format on

    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(true));

    result = decisionMaker->makeDecision(
        QThread::currentThread(), 1704542400000, configMock, instrumentSells, portfolio, stocks, false, 0, false, true
    );

    ASSERT_EQ(result.size(), 0);

    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getScheduleStartHour()).WillOnce(Return(10));
    EXPECT_CALL(*configMock, getScheduleStartMinute()).WillOnce(Return(0));
    EXPECT_CALL(*configMock, getScheduleEndHour()).WillOnce(Return(18));
    EXPECT_CALL(*configMock, getScheduleEndMinute()).WillOnce(Return(40));

    result = decisionMaker->makeDecision(
        QThread::currentThread(), 1704056400000, configMock, instrumentSells, portfolio, stocks, false, 0, false, true
    );

    ASSERT_EQ(result.size(), 0);

    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAutoPilotConfig()).WillOnce(Return(autoPilotConfigMock));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(
        *sellDecisionMock,
        makeDecision(
            QThread::currentThread(), autoPilotConfigMock, 0, &stock1, false, -1, FloatEq(0.1f), FloatEq(103.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return(""));
    EXPECT_CALL(
        *sellDecisionMock,
        makeDecision(
            QThread::currentThread(), autoPilotConfigMock, 0, &stock2, false, -1, FloatEq(0.2f), FloatEq(203.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return("I want to sell"));
    EXPECT_CALL(
        *buyDecisionMock,
        makeDecision(
            QThread::currentThread(), autoPilotConfigMock, 0, &stock3, false, -1, FloatEq(0.3f), FloatEq(-1.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return("I want to buy"));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*configMock, isLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isLimitByTurnover()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getLimitStockPurchasePart()).WillOnce(Return(7.0f));

    result = decisionMaker->makeDecision(
        QThread::currentThread(), 1704110400000, configMock, instrumentSells, portfolio, stocks, false, 0, false, true
    );

    // clang-format off
    ASSERT_EQ(result.size(),                  2);
    ASSERT_EQ(result.contains("ccccc"),       true);
    ASSERT_EQ(result["ccccc"].asap,           false);
    ASSERT_NEAR(result["ccccc"].avgPrice,     -1.0f, 0.0001f);
    ASSERT_NEAR(result["ccccc"].price,        0.3f, 0.0001f);
    ASSERT_NEAR(result["ccccc"].expectedCost, 69990.00445, 0.0001);
    ASSERT_EQ(result["ccccc"].cause,          "I want to buy");
    ASSERT_EQ(result.contains("bbbbb"),       true);
    ASSERT_EQ(result["bbbbb"].asap,           false);
    ASSERT_NEAR(result["bbbbb"].avgPrice,     203.0f, 0.0001f);
    ASSERT_NEAR(result["bbbbb"].price,        0.2f, 0.0001f);
    ASSERT_NEAR(result["bbbbb"].expectedCost, 0.0f, 0.0001f);
    ASSERT_EQ(result["bbbbb"].cause,          "I want to sell");
    // clang-format on

    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(
        *sellDecisionMock,
        makeDecision(
            QThread::currentThread(), simulatorConfigMock, 0, &stock1, false, -1, FloatEq(0.1f), FloatEq(103.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return(""));
    EXPECT_CALL(
        *sellDecisionMock,
        makeDecision(
            QThread::currentThread(), simulatorConfigMock, 0, &stock2, false, -1, FloatEq(0.2f), FloatEq(203.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return("I want to sell"));
    EXPECT_CALL(
        *buyDecisionMock,
        makeDecision(
            QThread::currentThread(), simulatorConfigMock, 0, &stock3, false, -1, FloatEq(0.3f), FloatEq(-1.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return("I want to buy"));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*configMock, isLimitStockPurchase()).WillOnce(Return(false));

    result = decisionMaker->makeDecision(
        QThread::currentThread(), 1704110400000, configMock, instrumentSells, portfolio, stocks, false, 0, false, true
    );

    // clang-format off
    ASSERT_EQ(result.size(),                  2);
    ASSERT_EQ(result.contains("ccccc"),       true);
    ASSERT_EQ(result["ccccc"].asap,           false);
    ASSERT_NEAR(result["ccccc"].avgPrice,     -1.0f, 0.0001f);
    ASSERT_NEAR(result["ccccc"].price,        0.3f, 0.0001f);
    ASSERT_NEAR(result["ccccc"].expectedCost, 99960.00635, 0.0001);
    ASSERT_EQ(result["ccccc"].cause,          "I want to buy");
    ASSERT_EQ(result.contains("bbbbb"),       true);
    ASSERT_EQ(result["bbbbb"].asap,           false);
    ASSERT_NEAR(result["bbbbb"].avgPrice,     203.0f, 0.0001f);
    ASSERT_NEAR(result["bbbbb"].price,        0.2f, 0.0001f);
    ASSERT_NEAR(result["bbbbb"].expectedCost, 0.0f, 0.0001f);
    ASSERT_EQ(result["bbbbb"].cause,          "I want to sell");
    // clang-format on

    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getAutoPilotConfig()).WillOnce(Return(autoPilotConfigMock));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(
        *sellDecisionMock,
        makeDecision(
            QThread::currentThread(), autoPilotConfigMock, 0, &stock1, false, -1, FloatEq(0.1f), FloatEq(103.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return(""));
    EXPECT_CALL(
        *sellDecisionMock,
        makeDecision(
            QThread::currentThread(), autoPilotConfigMock, 0, &stock2, false, -1, FloatEq(0.2f), FloatEq(203.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return("I want to sell"));

    result = decisionMaker->makeDecision(
        QThread::currentThread(), 1704110400000, configMock, instrumentSells, portfolio, stocks, true, 200000, false, true
    );

    // clang-format off
    ASSERT_EQ(result.size(),                  1);
    ASSERT_EQ(result.contains("bbbbb"),       true);
    ASSERT_EQ(result["bbbbb"].asap,           false);
    ASSERT_NEAR(result["bbbbb"].avgPrice,     203.0f, 0.0001f);
    ASSERT_NEAR(result["bbbbb"].price,        0.2f, 0.0001f);
    ASSERT_NEAR(result["bbbbb"].expectedCost, 0.0f, 0.0001f);
    ASSERT_EQ(result["bbbbb"].cause,          "I want to sell");
    // clang-format on

    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getAutoPilotConfig()).WillOnce(Return(autoPilotConfigMock));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(
        *sellDecisionMock,
        makeDecision(
            QThread::currentThread(), autoPilotConfigMock, 0, &stock1, true, 0, FloatEq(0.1f), FloatEq(103.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return(""));
    EXPECT_CALL(
        *sellDecisionMock,
        makeDecision(
            QThread::currentThread(), autoPilotConfigMock, 0, &stock2, true, 0, FloatEq(0.2f), FloatEq(203.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return("I want to sell"));
    EXPECT_CALL(
        *buyDecisionMock,
        makeDecision(
            QThread::currentThread(), autoPilotConfigMock, 0, &stock3, true, 0, FloatEq(0.3f), FloatEq(-1.0f), FloatEq(0.04f)
        )
    )
        .WillOnce(Return("I want to buy"));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*configMock, isLimitStockPurchase()).WillOnce(Return(false));

    result = decisionMaker->makeDecision(
        QThread::currentThread(), 1704110400000, configMock, instrumentSells, portfolio, stocks, true, 0, true, true
    );

    // clang-format off
    ASSERT_EQ(result.size(),                  2);
    ASSERT_EQ(result.contains("ccccc"),       true);
    ASSERT_EQ(result["ccccc"].asap,           false);
    ASSERT_NEAR(result["ccccc"].avgPrice,     -1.0f, 0.0001f);
    ASSERT_NEAR(result["ccccc"].price,        0.3f, 0.0001f);
    ASSERT_NEAR(result["ccccc"].expectedCost, 99960.00635, 0.0001);
    ASSERT_EQ(result["ccccc"].cause,          "I want to buy");
    ASSERT_EQ(result.contains("bbbbb"),       true);
    ASSERT_EQ(result["bbbbb"].asap,           false);
    ASSERT_NEAR(result["bbbbb"].avgPrice,     203.0f, 0.0001f);
    ASSERT_NEAR(result["bbbbb"].price,        0.2f, 0.0001f);
    ASSERT_NEAR(result["bbbbb"].expectedCost, 0.0f, 0.0001f);
    ASSERT_EQ(result["bbbbb"].cause,          "I want to sell");
    // clang-format on

    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getAutoPilotConfig()).WillOnce(Return(autoPilotConfigMock));
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04f));
    EXPECT_CALL(*userStorageMock, readUnlock());

    result = decisionMaker->makeDecision(
        QThread::currentThread(), 1704110460000, configMock, instrumentSells, portfolio, stocks, true, 0, true, true
    );

    ASSERT_EQ(result.size(), 0);
}
// NOLINTEND(readability-magic-numbers)
