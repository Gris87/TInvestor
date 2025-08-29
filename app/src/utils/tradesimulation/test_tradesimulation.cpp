#include "src/utils/tradesimulation/tradesimulation.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Ne;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_TradeSimulation : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_TradeSimulation, Test_createInitOperations)
{
    const InSequence seq;

    StrictMock<InstrumentsStorageMock> instrumentsStorageMock;
    StrictMock<LogosStorageMock>       logosStorageMock;

    Instruments instrumentsMap;
    Instrument  instrument;

    instrument.ticker                  = "RUBLE";
    instrument.name                    = "Ruble";
    instrument.lot                     = 1;
    instrument.pricePrecision          = 2;
    instrument.minPriceIncrement.units = 0;
    instrument.minPriceIncrement.nano  = 10000000;

    instrumentsMap[RUBLE_UID] = instrument;

    Logo logo;

    QList<Operation> operations;

    Operation operation;

    operation.timestamp                       = 500;
    operation.instrumentId                    = RUBLE_UID;
    operation.instrumentLogo                  = &logo;
    operation.instrumentTicker                = "RUBLE";
    operation.instrumentName                  = "Ruble";
    operation.description                     = "Input money";
    operation.price                           = 0.0f;
    operation.avgPriceFifo                    = 0.0f;
    operation.avgPriceWavg                    = 0.0f;
    operation.quantity                        = 0;
    operation.remainedQuantity                = 0;
    operation.payment                         = 1000000;
    operation.avgCostFifo                     = 0.0f;
    operation.costFifo.units                  = 0;
    operation.costFifo.nano                   = 0;
    operation.costWavg.units                  = 0;
    operation.costWavg.nano                   = 0;
    operation.commission                      = 0.0f;
    operation.yield                           = 0.0f;
    operation.yieldWithCommission             = 0.0f;
    operation.yieldWithCommissionPercent      = 0.0f;
    operation.inputMoney.units                = 1000000;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = 1000000;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission.units  = 0;
    operation.totalYieldWithCommission.nano   = 0;
    operation.totalYieldWithCommissionPercent = 0.0f;
    operation.remainedMoney.units             = 1000000;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = 1000000;
    operation.totalMoney.nano                 = 0;
    operation.pricePrecision                  = 2;
    operation.paymentPrecision                = 2;
    operation.commissionPrecision             = 2;

    operations << operation;

    QList<Operation> operations2;

    operation.timestamp                       = 5500;
    operation.instrumentId                    = RUBLE_UID;
    operation.instrumentLogo                  = &logo;
    operation.instrumentTicker                = "RUBLE";
    operation.instrumentName                  = "Ruble";
    operation.description                     = "Input money";
    operation.price                           = 0.0f;
    operation.avgPriceFifo                    = 0.0f;
    operation.avgPriceWavg                    = 0.0f;
    operation.quantity                        = 0;
    operation.remainedQuantity                = 0;
    operation.payment                         = 12345;
    operation.avgCostFifo                     = 0.0f;
    operation.costFifo.units                  = 0;
    operation.costFifo.nano                   = 0;
    operation.costWavg.units                  = 0;
    operation.costWavg.nano                   = 0;
    operation.commission                      = 0.0f;
    operation.yield                           = 0.0f;
    operation.yieldWithCommission             = 0.0f;
    operation.yieldWithCommissionPercent      = 0.0f;
    operation.inputMoney.units                = 12345;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = 12345;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission.units  = 0;
    operation.totalYieldWithCommission.nano   = 0;
    operation.totalYieldWithCommissionPercent = 0.0f;
    operation.remainedMoney.units             = 12345;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = 12345;
    operation.totalMoney.nano                 = 0;
    operation.pricePrecision                  = 2;
    operation.paymentPrecision                = 2;
    operation.commissionPrecision             = 2;

    operations2 << operation;

    EXPECT_CALL(instrumentsStorageMock, readLock());
    EXPECT_CALL(instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instrumentsMap));
    EXPECT_CALL(instrumentsStorageMock, readUnlock());
    EXPECT_CALL(logosStorageMock, readLock());
    EXPECT_CALL(logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(logosStorageMock, readUnlock());

    ASSERT_EQ(createInitOperations(&instrumentsStorageMock, &logosStorageMock, 500, 1000000), operations);

    EXPECT_CALL(instrumentsStorageMock, readLock());
    EXPECT_CALL(instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instrumentsMap));
    EXPECT_CALL(instrumentsStorageMock, readUnlock());
    EXPECT_CALL(logosStorageMock, readLock());
    EXPECT_CALL(logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(logosStorageMock, readUnlock());

    ASSERT_EQ(createInitOperations(&instrumentsStorageMock, &logosStorageMock, 5500, 12345), operations2);
}

TEST_F(Test_TradeSimulation, Test_createInitEntries)
{
    ASSERT_EQ(createInitEntries(), QList<LogEntry>());
}

TEST_F(Test_TradeSimulation, Test_createInitPortfolio)
{
    const InSequence seq;

    StrictMock<InstrumentsStorageMock> instrumentsStorageMock;
    StrictMock<LogosStorageMock>       logosStorageMock;

    Instruments instrumentsMap;
    Instrument  instrument;

    instrument.ticker                  = "RUBLE";
    instrument.name                    = "Ruble";
    instrument.lot                     = 1;
    instrument.pricePrecision          = 2;
    instrument.minPriceIncrement.units = 0;
    instrument.minPriceIncrement.nano  = 10000000;

    instrumentsMap[RUBLE_UID] = instrument;

    Logo logo;

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item;

    item.instrumentId       = RUBLE_UID;
    item.instrumentLogo     = &logo;
    item.instrumentTicker   = "RUBLE";
    item.instrumentName     = "Ruble";
    item.showPrices         = false;
    item.available          = 1000000;
    item.price              = 1.0f;
    item.avgPriceFifo       = 1.0f;
    item.avgPriceWavg       = 1.0f;
    item.cost               = 1000000;
    item.part               = 100.0;
    item.yield              = 0.0f;
    item.yieldPercent       = 0.0f;
    item.dailyYield         = 0.0f;
    item.priceForDailyYield = 0.0f;
    item.costForDailyYield  = 0.0;
    item.dailyYieldPercent  = 0.0f;
    item.pricePrecision     = 2;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 1000000;
    category1.part = 100.0;
    category1.items << item;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 0.0;
    category2.part = 0.0;

    portfolio.positions << category1 << category2;

    Portfolio             portfolio2;
    PortfolioCategoryItem category3;
    PortfolioCategoryItem category4;
    PortfolioItem         item2;

    item2.instrumentId       = RUBLE_UID;
    item2.instrumentLogo     = &logo;
    item2.instrumentTicker   = "RUBLE";
    item2.instrumentName     = "Ruble";
    item2.showPrices         = false;
    item2.available          = 12345;
    item2.price              = 1.0f;
    item2.avgPriceFifo       = 1.0f;
    item2.avgPriceWavg       = 1.0f;
    item2.cost               = 12345;
    item2.part               = 100.0;
    item2.yield              = 0.0f;
    item2.yieldPercent       = 0.0f;
    item2.dailyYield         = 0.0f;
    item2.priceForDailyYield = 0.0f;
    item2.costForDailyYield  = 0.0;
    item2.dailyYieldPercent  = 0.0f;
    item2.pricePrecision     = 2;

    category3.id   = 0;
    category3.name = "Currency and metals";
    category3.cost = 12345;
    category3.part = 100.0;
    category3.items << item2;

    category4.id   = 1;
    category4.name = "Share";
    category4.cost = 0.0;
    category4.part = 0.0;

    portfolio2.positions << category3 << category4;

    EXPECT_CALL(instrumentsStorageMock, readLock());
    EXPECT_CALL(instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instrumentsMap));
    EXPECT_CALL(instrumentsStorageMock, readUnlock());
    EXPECT_CALL(logosStorageMock, readLock());
    EXPECT_CALL(logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(logosStorageMock, readUnlock());

    ASSERT_EQ(createInitPortfolio(&instrumentsStorageMock, &logosStorageMock, 1000000), portfolio);

    EXPECT_CALL(instrumentsStorageMock, readLock());
    EXPECT_CALL(instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instrumentsMap));
    EXPECT_CALL(instrumentsStorageMock, readUnlock());
    EXPECT_CALL(logosStorageMock, readLock());
    EXPECT_CALL(logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(logosStorageMock, readUnlock());

    ASSERT_EQ(createInitPortfolio(&instrumentsStorageMock, &logosStorageMock, 12345), portfolio2);
}

TEST_F(Test_TradeSimulation, Test_simulateTrading)
{
    const InSequence seq;

    StrictMock<InstrumentsStorageMock> instrumentsStorageMock;
    StrictMock<LogosStorageMock>       logosStorageMock;
    StrictMock<UserStorageMock>        userStorageMock;

    Instruments instrumentsMap;
    Instrument  instrument1;
    Instrument  instrument2;
    Instrument  instrument3;

    instrument1.ticker                  = "RUBLE";
    instrument1.name                    = "Ruble";
    instrument1.lot                     = 1;
    instrument1.pricePrecision          = 2;
    instrument1.minPriceIncrement.units = 0;
    instrument1.minPriceIncrement.nano  = 10000000;

    instrument2.ticker                  = "ABBA";
    instrument2.name                    = "Abstract Basics";
    instrument2.lot                     = 5;
    instrument2.pricePrecision          = 1;
    instrument2.minPriceIncrement.units = 0;
    instrument2.minPriceIncrement.nano  = 100000000;

    instrument3.ticker                  = "BASE";
    instrument3.name                    = "Basketball enhancement";
    instrument3.lot                     = 10;
    instrument3.pricePrecision          = 2;
    instrument3.minPriceIncrement.units = 0;
    instrument3.minPriceIncrement.nano  = 10000000;

    instrumentsMap[RUBLE_UID] = instrument1;
    instrumentsMap["aaaaa"]   = instrument2;
    instrumentsMap["bbbbb"]   = instrument3;

    Logo logo;

    QList<Operation> operations;

    Operation operation;

    operation.timestamp                       = 500;
    operation.instrumentId                    = RUBLE_UID;
    operation.instrumentLogo                  = &logo;
    operation.instrumentTicker                = "RUBLE";
    operation.instrumentName                  = "Ruble";
    operation.description                     = "Input money";
    operation.price                           = 0.0f;
    operation.avgPriceFifo                    = 0.0f;
    operation.avgPriceWavg                    = 0.0f;
    operation.quantity                        = 0;
    operation.remainedQuantity                = 0;
    operation.payment                         = 1000000;
    operation.avgCostFifo                     = 0.0f;
    operation.costFifo.units                  = 0;
    operation.costFifo.nano                   = 0;
    operation.costWavg.units                  = 0;
    operation.costWavg.nano                   = 0;
    operation.commission                      = 0.0f;
    operation.yield                           = 0.0f;
    operation.yieldWithCommission             = 0.0f;
    operation.yieldWithCommissionPercent      = 0.0f;
    operation.inputMoney.units                = 1000000;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = 1000000;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission.units  = 0;
    operation.totalYieldWithCommission.nano   = 0;
    operation.totalYieldWithCommissionPercent = 0.0f;
    operation.remainedMoney.units             = 1000000;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = 1000000;
    operation.totalMoney.nano                 = 0;
    operation.pricePrecision                  = 2;
    operation.paymentPrecision                = 2;
    operation.commissionPrecision             = 2;

    operations << operation;

    QList<LogEntry> entries;

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item;

    item.instrumentId       = RUBLE_UID;
    item.instrumentLogo     = &logo;
    item.instrumentTicker   = "RUBLE";
    item.instrumentName     = "Ruble";
    item.showPrices         = false;
    item.available          = 1000000;
    item.price              = 1.0f;
    item.avgPriceFifo       = 1.0f;
    item.avgPriceWavg       = 1.0f;
    item.cost               = 1000000;
    item.part               = 100.0;
    item.yield              = 0.0f;
    item.yieldPercent       = 0.0f;
    item.dailyYield         = 0.0f;
    item.priceForDailyYield = 0.0f;
    item.costForDailyYield  = 0.0;
    item.dailyYieldPercent  = 0.0f;
    item.pricePrecision     = 2;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 1000000;
    category1.part = 100.0;
    category1.items << item;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 0.0;
    category2.part = 0.0;

    portfolio.positions << category1 << category2;

    QuantityAndCostDoubleInstruments instruments;
    InstrumentSells                  instrumentSells;

    double totalMoney = 1000000.0;

    InstrumentsForTrading instrumentsForTrading;
    TradingInfo           tradingInfo;

    tradingInfo.price        = 200.0f;
    tradingInfo.expectedCost = 100000.0;
    tradingInfo.cause        = "I want to buy";

    instrumentsForTrading["aaaaa"] = tradingInfo;

    QList<Operation> buyOperations = operations;

    Operation buyOperation;

    buyOperation.timestamp                       = 1000;
    buyOperation.instrumentId                    = "aaaaa";
    buyOperation.instrumentLogo                  = &logo;
    buyOperation.instrumentTicker                = "ABBA";
    buyOperation.instrumentName                  = "Abstract Basics";
    buyOperation.description                     = "Purchase of shares";
    buyOperation.price                           = 200.0f;
    buyOperation.avgPriceFifo                    = 200.0f;
    buyOperation.avgPriceWavg                    = 200.0f;
    buyOperation.quantity                        = 500;
    buyOperation.remainedQuantity                = 500;
    buyOperation.payment                         = -100000.0f;
    buyOperation.avgCostFifo                     = 100000.0f;
    buyOperation.costFifo.units                  = 100000;
    buyOperation.costFifo.nano                   = 0;
    buyOperation.costWavg.units                  = 100000;
    buyOperation.costWavg.nano                   = 0;
    buyOperation.commission                      = -40.0f;
    buyOperation.yield                           = 0.0f;
    buyOperation.yieldWithCommission             = -40.0f;
    buyOperation.yieldWithCommissionPercent      = -0.04f;
    buyOperation.inputMoney.units                = 1000000;
    buyOperation.inputMoney.nano                 = 0;
    buyOperation.maxInputMoney.units             = 1000000;
    buyOperation.maxInputMoney.nano              = 0;
    buyOperation.totalYieldWithCommission.units  = -39;
    buyOperation.totalYieldWithCommission.nano   = -999998989;
    buyOperation.totalYieldWithCommissionPercent = -0.004f;
    buyOperation.remainedMoney.units             = 899960;
    buyOperation.remainedMoney.nano              = 1010;
    buyOperation.totalMoney.units                = 999960;
    buyOperation.totalMoney.nano                 = 1010;
    buyOperation.pricePrecision                  = 1;
    buyOperation.paymentPrecision                = 1;
    buyOperation.commissionPrecision             = 1;

    buyOperations << buyOperation;

    QList<LogEntry> buyEntries = entries;

    LogEntry buyEntry1;
    LogEntry buyEntry2;
    LogEntry buyEntry3;
    LogEntry buyEntry4;

    buyEntry1.timestamp        = 1001;
    buyEntry1.level            = LOG_LEVEL_DEBUG;
    buyEntry1.instrumentId     = "aaaaa";
    buyEntry1.instrumentLogo   = &logo;
    buyEntry1.instrumentTicker = "ABBA";
    buyEntry1.instrumentName   = "Abstract Basics";
    buyEntry1.message          = "I want to buy";

    buyEntry2.timestamp        = 1002;
    buyEntry2.level            = LOG_LEVEL_VERBOSE;
    buyEntry2.instrumentId     = "aaaaa";
    buyEntry2.instrumentLogo   = &logo;
    buyEntry2.instrumentTicker = "ABBA";
    buyEntry2.instrumentName   = "Abstract Basics";
    buyEntry2.message          = "Order to buy 500 created with a price 200.0 \u20BD";

    buyEntry3.timestamp        = 1003;
    buyEntry3.level            = LOG_LEVEL_VERBOSE;
    buyEntry3.instrumentId     = "aaaaa";
    buyEntry3.instrumentLogo   = &logo;
    buyEntry3.instrumentTicker = "ABBA";
    buyEntry3.instrumentName   = "Abstract Basics";
    buyEntry3.message          = "Order completed. 500 bought with a price 200.0 \u20BD";

    buyEntry4.timestamp        = 1004;
    buyEntry4.level            = LOG_LEVEL_VERBOSE;
    buyEntry4.instrumentId     = "aaaaa";
    buyEntry4.instrumentLogo   = &logo;
    buyEntry4.instrumentTicker = "ABBA";
    buyEntry4.instrumentName   = "Abstract Basics";
    buyEntry4.message          = "Trade completed successfully";

    buyEntries << buyEntry1 << buyEntry2 << buyEntry3 << buyEntry4;

    Portfolio             buyPortfolio;
    PortfolioCategoryItem buyCategory1;
    PortfolioCategoryItem buyCategory2;
    PortfolioItem         buyItem1;
    PortfolioItem         buyItem2;

    buyItem1.instrumentId       = RUBLE_UID;
    buyItem1.instrumentLogo     = &logo;
    buyItem1.instrumentTicker   = "RUBLE";
    buyItem1.instrumentName     = "Ruble";
    buyItem1.showPrices         = false;
    buyItem1.available          = 899960.0;
    buyItem1.price              = 1.0f;
    buyItem1.avgPriceFifo       = 1.0f;
    buyItem1.avgPriceWavg       = 1.0f;
    buyItem1.cost               = 899960.0;
    buyItem1.part               = 100.0f;
    buyItem1.yield              = 0.0f;
    buyItem1.yieldPercent       = 0.0f;
    buyItem1.dailyYield         = 0.0f;
    buyItem1.priceForDailyYield = 0.0f;
    buyItem1.costForDailyYield  = 0.0;
    buyItem1.dailyYieldPercent  = 0.0f;
    buyItem1.pricePrecision     = 2;

    buyItem2.instrumentId       = "aaaaa";
    buyItem2.instrumentLogo     = &logo;
    buyItem2.instrumentTicker   = "ABBA";
    buyItem2.instrumentName     = "Abstract Basics";
    buyItem2.showPrices         = true;
    buyItem2.available          = 500;
    buyItem2.price              = 200.0f;
    buyItem2.avgPriceFifo       = 200.0f;
    buyItem2.avgPriceWavg       = 200.0f;
    buyItem2.cost               = 100000.0;
    buyItem2.part               = 0.0f;
    buyItem2.yield              = 0.0f;
    buyItem2.yieldPercent       = 0.0f;
    buyItem2.dailyYield         = 0.0f;
    buyItem2.priceForDailyYield = 200.0f;
    buyItem2.costForDailyYield  = 100000.0;
    buyItem2.dailyYieldPercent  = 0.0f;
    buyItem2.pricePrecision     = 1;

    buyCategory1.id   = 0;
    buyCategory1.name = "Currency and metals";
    buyCategory1.cost = 1000000.0;
    buyCategory1.part = 100.0f;
    buyCategory1.items << buyItem1;

    buyCategory2.id   = 1;
    buyCategory2.name = "Share";
    buyCategory2.cost = 0.0;
    buyCategory2.part = 0.0f;
    buyCategory2.items << buyItem2;

    buyPortfolio.positions << buyCategory1 << buyCategory2;

    EXPECT_CALL(instrumentsStorageMock, readLock());
    EXPECT_CALL(instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instrumentsMap));
    EXPECT_CALL(instrumentsStorageMock, readUnlock());
    EXPECT_CALL(userStorageMock, readLock());
    EXPECT_CALL(userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(userStorageMock, readUnlock());
    EXPECT_CALL(logosStorageMock, readLock());
    EXPECT_CALL(logosStorageMock, getLogo(QString("aaaaa"))).WillOnce(Return(&logo));
    EXPECT_CALL(logosStorageMock, readUnlock());

    simulateTrading(
        1000,
        &instrumentsStorageMock,
        &logosStorageMock,
        &userStorageMock,
        instrumentsForTrading,
        1000000,
        totalMoney,
        operations,
        entries,
        portfolio,
        instruments,
        instrumentSells
    );

    // clang-format off
    ASSERT_NEAR(totalMoney,                  999960.0, 0.0001);
    ASSERT_EQ(operations,                    buyOperations);
    ASSERT_EQ(entries,                       buyEntries);
    ASSERT_EQ(portfolio,                     buyPortfolio);
    ASSERT_EQ(instruments.size(),            1);
    ASSERT_EQ(instruments["aaaaa"].quantity, 500);
    ASSERT_NEAR(instruments["aaaaa"].cost,   100000.0, 0.0001);
    ASSERT_EQ(instrumentSells.size(),        0);
    // clang-format on

    tradingInfo.price        = 200.0f;
    tradingInfo.expectedCost = 100000.0;
    tradingInfo.cause        = "I want to buy";

    instrumentsForTrading["aaaaa"] = tradingInfo;

    tradingInfo.price        = 200.0f;
    tradingInfo.expectedCost = 150000.0;
    tradingInfo.cause        = "I want to buy";

    instrumentsForTrading["bbbbb"] = tradingInfo;

    QList<Operation> buyOperations2 = operations;

    Operation buyOperation2;

    buyOperation2.timestamp                       = 2000;
    buyOperation2.instrumentId                    = "bbbbb";
    buyOperation2.instrumentLogo                  = &logo;
    buyOperation2.instrumentTicker                = "BASE";
    buyOperation2.instrumentName                  = "Basketball enhancement";
    buyOperation2.description                     = "Purchase of shares";
    buyOperation2.price                           = 200.0f;
    buyOperation2.avgPriceFifo                    = 200.0f;
    buyOperation2.avgPriceWavg                    = 200.0f;
    buyOperation2.quantity                        = 750;
    buyOperation2.remainedQuantity                = 750;
    buyOperation2.payment                         = -150000.0f;
    buyOperation2.avgCostFifo                     = 150000.0f;
    buyOperation2.costFifo.units                  = 150000;
    buyOperation2.costFifo.nano                   = 0;
    buyOperation2.costWavg.units                  = 150000;
    buyOperation2.costWavg.nano                   = 0;
    buyOperation2.commission                      = -60.0f;
    buyOperation2.yield                           = 0.0f;
    buyOperation2.yieldWithCommission             = -60.0f;
    buyOperation2.yieldWithCommissionPercent      = -0.04f;
    buyOperation2.inputMoney.units                = 1000000;
    buyOperation2.inputMoney.nano                 = 0;
    buyOperation2.maxInputMoney.units             = 1000000;
    buyOperation2.maxInputMoney.nano              = 0;
    buyOperation2.totalYieldWithCommission.units  = -99;
    buyOperation2.totalYieldWithCommission.nano   = -999997473;
    buyOperation2.totalYieldWithCommissionPercent = -0.01f;
    buyOperation2.remainedMoney.units             = 749900;
    buyOperation2.remainedMoney.nano              = 2526;
    buyOperation2.totalMoney.units                = 999900;
    buyOperation2.totalMoney.nano                 = 2526;
    buyOperation2.pricePrecision                  = 2;
    buyOperation2.paymentPrecision                = 2;
    buyOperation2.commissionPrecision             = 2;

    buyOperations2 << buyOperation2;

    QList<LogEntry> buyEntries2 = entries;

    buyEntry1.timestamp        = 2001;
    buyEntry1.level            = LOG_LEVEL_DEBUG;
    buyEntry1.instrumentId     = "bbbbb";
    buyEntry1.instrumentLogo   = &logo;
    buyEntry1.instrumentTicker = "BASE";
    buyEntry1.instrumentName   = "Basketball enhancement";
    buyEntry1.message          = "I want to buy";

    buyEntry2.timestamp        = 2002;
    buyEntry2.level            = LOG_LEVEL_VERBOSE;
    buyEntry2.instrumentId     = "bbbbb";
    buyEntry2.instrumentLogo   = &logo;
    buyEntry2.instrumentTicker = "BASE";
    buyEntry2.instrumentName   = "Basketball enhancement";
    buyEntry2.message          = "Order to buy 750 created with a price 200.00 \u20BD";

    buyEntry3.timestamp        = 2003;
    buyEntry3.level            = LOG_LEVEL_VERBOSE;
    buyEntry3.instrumentId     = "bbbbb";
    buyEntry3.instrumentLogo   = &logo;
    buyEntry3.instrumentTicker = "BASE";
    buyEntry3.instrumentName   = "Basketball enhancement";
    buyEntry3.message          = "Order completed. 750 bought with a price 200.00 \u20BD";

    buyEntry4.timestamp        = 2004;
    buyEntry4.level            = LOG_LEVEL_VERBOSE;
    buyEntry4.instrumentId     = "bbbbb";
    buyEntry4.instrumentLogo   = &logo;
    buyEntry4.instrumentTicker = "BASE";
    buyEntry4.instrumentName   = "Basketball enhancement";
    buyEntry4.message          = "Trade completed successfully";

    buyEntries2 << buyEntry1 << buyEntry2 << buyEntry3 << buyEntry4;

    Portfolio             buyPortfolio2;
    PortfolioCategoryItem buyCategory3;
    PortfolioCategoryItem buyCategory4;
    PortfolioItem         buyItem3;
    PortfolioItem         buyItem4;
    PortfolioItem         buyItem5;

    buyItem3.instrumentId       = RUBLE_UID;
    buyItem3.instrumentLogo     = &logo;
    buyItem3.instrumentTicker   = "RUBLE";
    buyItem3.instrumentName     = "Ruble";
    buyItem3.showPrices         = false;
    buyItem3.available          = 749900.0;
    buyItem3.price              = 1.0f;
    buyItem3.avgPriceFifo       = 1.0f;
    buyItem3.avgPriceWavg       = 1.0f;
    buyItem3.cost               = 749900.0;
    buyItem3.part               = 100.0;
    buyItem3.yield              = 0.0f;
    buyItem3.yieldPercent       = 0.0f;
    buyItem3.dailyYield         = 0.0f;
    buyItem3.priceForDailyYield = 0.0f;
    buyItem3.costForDailyYield  = 0.0;
    buyItem3.dailyYieldPercent  = 0.0f;
    buyItem3.pricePrecision     = 2;

    buyItem4.instrumentId       = "aaaaa";
    buyItem4.instrumentLogo     = &logo;
    buyItem4.instrumentTicker   = "ABBA";
    buyItem4.instrumentName     = "Abstract Basics";
    buyItem4.showPrices         = true;
    buyItem4.available          = 500;
    buyItem4.price              = 200.0f;
    buyItem4.avgPriceFifo       = 200.0f;
    buyItem4.avgPriceWavg       = 200.0f;
    buyItem4.cost               = 100000.0;
    buyItem4.part               = 0.0;
    buyItem4.yield              = 0.0f;
    buyItem4.yieldPercent       = 0.0f;
    buyItem4.dailyYield         = 0.0f;
    buyItem4.priceForDailyYield = 200.0f;
    buyItem4.costForDailyYield  = 100000.0;
    buyItem4.dailyYieldPercent  = 0.0f;
    buyItem4.pricePrecision     = 1;

    buyItem5.instrumentId       = "bbbbb";
    buyItem5.instrumentLogo     = &logo;
    buyItem5.instrumentTicker   = "BASE";
    buyItem5.instrumentName     = "Basketball enhancement";
    buyItem5.showPrices         = true;
    buyItem5.available          = 750;
    buyItem5.price              = 200.0f;
    buyItem5.avgPriceFifo       = 200.0f;
    buyItem5.avgPriceWavg       = 200.0f;
    buyItem5.cost               = 150000.0;
    buyItem5.part               = 0.0;
    buyItem5.yield              = 0.0f;
    buyItem5.yieldPercent       = 0.0f;
    buyItem5.dailyYield         = 0.0f;
    buyItem5.priceForDailyYield = 200.0f;
    buyItem5.costForDailyYield  = 150000.0;
    buyItem5.dailyYieldPercent  = 0.0f;
    buyItem5.pricePrecision     = 2;

    buyCategory3.id   = 0;
    buyCategory3.name = "Currency and metals";
    buyCategory3.cost = 1000000.0;
    buyCategory3.part = 100.0;
    buyCategory3.items << buyItem3;

    buyCategory4.id   = 1;
    buyCategory4.name = "Share";
    buyCategory4.cost = 0.0;
    buyCategory4.part = 0.0;
    buyCategory4.items << buyItem4 << buyItem5;

    buyPortfolio2.positions << buyCategory3 << buyCategory4;

    EXPECT_CALL(instrumentsStorageMock, readLock());
    EXPECT_CALL(instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instrumentsMap));
    EXPECT_CALL(instrumentsStorageMock, readUnlock());
    EXPECT_CALL(userStorageMock, readLock());
    EXPECT_CALL(userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(userStorageMock, readUnlock());
    EXPECT_CALL(logosStorageMock, readLock());
    EXPECT_CALL(logosStorageMock, getLogo(QString("bbbbb"))).WillOnce(Return(&logo));
    EXPECT_CALL(logosStorageMock, readUnlock());

    simulateTrading(
        2000,
        &instrumentsStorageMock,
        &logosStorageMock,
        &userStorageMock,
        instrumentsForTrading,
        1000000,
        totalMoney,
        operations,
        entries,
        portfolio,
        instruments,
        instrumentSells
    );

    // clang-format off
    ASSERT_NEAR(totalMoney,                  999900.0, 0.0001);
    ASSERT_EQ(operations,                    buyOperations2);
    ASSERT_EQ(entries,                       buyEntries2);
    ASSERT_EQ(portfolio,                     buyPortfolio2);
    ASSERT_EQ(instruments.size(),            2);
    ASSERT_EQ(instruments["aaaaa"].quantity, 500);
    ASSERT_NEAR(instruments["aaaaa"].cost,   100000.0, 0.0001);
    ASSERT_EQ(instruments["bbbbb"].quantity, 750);
    ASSERT_NEAR(instruments["bbbbb"].cost,   150000.0, 0.0001);
    ASSERT_EQ(instrumentSells.size(),        0);
    // clang-format on

    tradingInfo.price        = 300.0f;
    tradingInfo.expectedCost = 0.0;
    tradingInfo.cause        = "I want to sell";

    instrumentsForTrading["bbbbb"] = tradingInfo;

    QList<Operation> sellOperations = operations;

    Operation sellOperation;

    sellOperation.timestamp                       = 3000;
    sellOperation.instrumentId                    = "bbbbb";
    sellOperation.instrumentLogo                  = &logo;
    sellOperation.instrumentTicker                = "BASE";
    sellOperation.instrumentName                  = "Basketball enhancement";
    sellOperation.description                     = "Sale of shares";
    sellOperation.price                           = 300.0f;
    sellOperation.avgPriceFifo                    = 200.0f;
    sellOperation.avgPriceWavg                    = 200.0f;
    sellOperation.quantity                        = 750;
    sellOperation.remainedQuantity                = 0;
    sellOperation.payment                         = 225000.0f;
    sellOperation.avgCostFifo                     = 150000.0f;
    sellOperation.costFifo.units                  = 0;
    sellOperation.costFifo.nano                   = 0;
    sellOperation.costWavg.units                  = 0;
    sellOperation.costWavg.nano                   = 0;
    sellOperation.commission                      = -90.0f;
    sellOperation.yield                           = 75000.0f;
    sellOperation.yieldWithCommission             = 74910.0f;
    sellOperation.yieldWithCommissionPercent      = 49.94f;
    sellOperation.inputMoney.units                = 1000000;
    sellOperation.inputMoney.nano                 = 0;
    sellOperation.maxInputMoney.units             = 1000000;
    sellOperation.maxInputMoney.nano              = 0;
    sellOperation.totalYieldWithCommission.units  = 74810;
    sellOperation.totalYieldWithCommission.nano   = 4799;
    sellOperation.totalYieldWithCommissionPercent = 7.481f;
    sellOperation.remainedMoney.units             = 974810;
    sellOperation.remainedMoney.nano              = 4799;
    sellOperation.totalMoney.units                = 1074810;
    sellOperation.totalMoney.nano                 = 4799;
    sellOperation.pricePrecision                  = 2;
    sellOperation.paymentPrecision                = 2;
    sellOperation.commissionPrecision             = 2;

    sellOperations << sellOperation;

    QList<LogEntry> sellEntries = entries;

    LogEntry sellEntry1;
    LogEntry sellEntry2;
    LogEntry sellEntry3;
    LogEntry sellEntry4;

    sellEntry1.timestamp        = 3001;
    sellEntry1.level            = LOG_LEVEL_DEBUG;
    sellEntry1.instrumentId     = "bbbbb";
    sellEntry1.instrumentLogo   = &logo;
    sellEntry1.instrumentTicker = "BASE";
    sellEntry1.instrumentName   = "Basketball enhancement";
    sellEntry1.message          = "I want to sell";

    sellEntry2.timestamp        = 3002;
    sellEntry2.level            = LOG_LEVEL_VERBOSE;
    sellEntry2.instrumentId     = "bbbbb";
    sellEntry2.instrumentLogo   = &logo;
    sellEntry2.instrumentTicker = "BASE";
    sellEntry2.instrumentName   = "Basketball enhancement";
    sellEntry2.message          = "Order to sell 750 created with a price 300.00 \u20BD";

    sellEntry3.timestamp        = 3003;
    sellEntry3.level            = LOG_LEVEL_VERBOSE;
    sellEntry3.instrumentId     = "bbbbb";
    sellEntry3.instrumentLogo   = &logo;
    sellEntry3.instrumentTicker = "BASE";
    sellEntry3.instrumentName   = "Basketball enhancement";
    sellEntry3.message          = "Order completed. 750 sold with a price 300.00 \u20BD";

    sellEntry4.timestamp        = 3004;
    sellEntry4.level            = LOG_LEVEL_VERBOSE;
    sellEntry4.instrumentId     = "bbbbb";
    sellEntry4.instrumentLogo   = &logo;
    sellEntry4.instrumentTicker = "BASE";
    sellEntry4.instrumentName   = "Basketball enhancement";
    sellEntry4.message          = "Trade completed successfully";

    sellEntries << sellEntry1 << sellEntry2 << sellEntry3 << sellEntry4;

    Portfolio             sellPortfolio;
    PortfolioCategoryItem sellCategory1;
    PortfolioCategoryItem sellCategory2;
    PortfolioItem         sellItem1;
    PortfolioItem         sellItem2;

    sellItem1.instrumentId       = RUBLE_UID;
    sellItem1.instrumentLogo     = &logo;
    sellItem1.instrumentTicker   = "RUBLE";
    sellItem1.instrumentName     = "Ruble";
    sellItem1.showPrices         = false;
    sellItem1.available          = 974810.0;
    sellItem1.price              = 1.0f;
    sellItem1.avgPriceFifo       = 1.0f;
    sellItem1.avgPriceWavg       = 1.0f;
    sellItem1.cost               = 974810.0;
    sellItem1.part               = 100.0;
    sellItem1.yield              = 0.0f;
    sellItem1.yieldPercent       = 0.0f;
    sellItem1.dailyYield         = 0.0f;
    sellItem1.priceForDailyYield = 0.0f;
    sellItem1.costForDailyYield  = 0.0;
    sellItem1.dailyYieldPercent  = 0.0f;
    sellItem1.pricePrecision     = 2;

    sellItem2.instrumentId       = "aaaaa";
    sellItem2.instrumentLogo     = &logo;
    sellItem2.instrumentTicker   = "ABBA";
    sellItem2.instrumentName     = "Abstract Basics";
    sellItem2.showPrices         = true;
    sellItem2.available          = 500;
    sellItem2.price              = 200.0f;
    sellItem2.avgPriceFifo       = 200.0f;
    sellItem2.avgPriceWavg       = 200.0f;
    sellItem2.cost               = 100000.0;
    sellItem2.part               = 0.0;
    sellItem2.yield              = 0.0f;
    sellItem2.yieldPercent       = 0.0f;
    sellItem2.dailyYield         = 0.0f;
    sellItem2.priceForDailyYield = 200.0f;
    sellItem2.costForDailyYield  = 100000.0;
    sellItem2.dailyYieldPercent  = 0.0;
    sellItem2.pricePrecision     = 1;

    sellCategory1.id   = 0;
    sellCategory1.name = "Currency and metals";
    sellCategory1.cost = 1000000.0;
    sellCategory1.part = 100.0;
    sellCategory1.items << sellItem1;

    sellCategory2.id   = 1;
    sellCategory2.name = "Share";
    sellCategory2.cost = 0.0;
    sellCategory2.part = 0.0;
    sellCategory2.items << sellItem2;

    sellPortfolio.positions << sellCategory1 << sellCategory2;

    EXPECT_CALL(instrumentsStorageMock, readLock());
    EXPECT_CALL(instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instrumentsMap));
    EXPECT_CALL(instrumentsStorageMock, readUnlock());
    EXPECT_CALL(userStorageMock, readLock());
    EXPECT_CALL(userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(userStorageMock, readUnlock());
    EXPECT_CALL(logosStorageMock, readLock());
    EXPECT_CALL(logosStorageMock, getLogo(QString("bbbbb"))).WillOnce(Return(&logo));
    EXPECT_CALL(logosStorageMock, readUnlock());

    simulateTrading(
        3000,
        &instrumentsStorageMock,
        &logosStorageMock,
        &userStorageMock,
        instrumentsForTrading,
        1000000,
        totalMoney,
        operations,
        entries,
        portfolio,
        instruments,
        instrumentSells
    );

    // clang-format off
    ASSERT_NEAR(totalMoney,                  1074810.0, 0.0001);
    ASSERT_EQ(operations,                    sellOperations);
    ASSERT_EQ(entries,                       sellEntries);
    ASSERT_EQ(portfolio,                     sellPortfolio);
    ASSERT_EQ(instruments.size(),            1);
    ASSERT_EQ(instruments["aaaaa"].quantity, 500);
    ASSERT_NEAR(instruments["aaaaa"].cost,   100000.0, 0.0001);
    ASSERT_EQ(instrumentSells.size(),        1);
    ASSERT_EQ(instrumentSells["bbbbb"],      3005);
    // clang-format on

    tradingInfo.price        = 2000.0f;
    tradingInfo.expectedCost = 0.0;
    tradingInfo.cause        = "I want to sell";

    instrumentsForTrading["aaaaa"] = tradingInfo;

    QList<Operation> sellOperations2 = operations;

    Operation sellOperation2;

    sellOperation2.timestamp                       = 4000;
    sellOperation2.instrumentId                    = "aaaaa";
    sellOperation2.instrumentLogo                  = &logo;
    sellOperation2.instrumentTicker                = "ABBA";
    sellOperation2.instrumentName                  = "Abstract Basics";
    sellOperation2.description                     = "Sale of shares";
    sellOperation2.price                           = 2000.0f;
    sellOperation2.avgPriceFifo                    = 200.0f;
    sellOperation2.avgPriceWavg                    = 200.0f;
    sellOperation2.quantity                        = 500;
    sellOperation2.remainedQuantity                = 0;
    sellOperation2.payment                         = 100000.0f;
    sellOperation2.avgCostFifo                     = 100000.0f;
    sellOperation2.costFifo.units                  = 0;
    sellOperation2.costFifo.nano                   = 0;
    sellOperation2.costWavg.units                  = 0;
    sellOperation2.costWavg.nano                   = 0;
    sellOperation2.commission                      = 0.0f;
    sellOperation2.yield                           = 0.0f;
    sellOperation2.yieldWithCommission             = 0.0f;
    sellOperation2.yieldWithCommissionPercent      = 0.0f;
    sellOperation2.inputMoney.units                = 1000000;
    sellOperation2.inputMoney.nano                 = 0;
    sellOperation2.maxInputMoney.units             = 1000000;
    sellOperation2.maxInputMoney.nano              = 0;
    sellOperation2.totalYieldWithCommission.units  = 74810;
    sellOperation2.totalYieldWithCommission.nano   = 4799;
    sellOperation2.totalYieldWithCommissionPercent = 7.481f;
    sellOperation2.remainedMoney.units             = 1074810;
    sellOperation2.remainedMoney.nano              = 4799;
    sellOperation2.totalMoney.units                = 1074810;
    sellOperation2.totalMoney.nano                 = 4799;
    sellOperation2.pricePrecision                  = 1;
    sellOperation2.paymentPrecision                = 1;
    sellOperation2.commissionPrecision             = 1;

    sellOperations2 << sellOperation2;

    QList<LogEntry> sellEntries2 = entries;

    sellEntry1.timestamp        = 4001;
    sellEntry1.level            = LOG_LEVEL_DEBUG;
    sellEntry1.instrumentId     = "aaaaa";
    sellEntry1.instrumentLogo   = &logo;
    sellEntry1.instrumentTicker = "ABBA";
    sellEntry1.instrumentName   = "Abstract Basics";
    sellEntry1.message          = "I want to sell";

    sellEntry2.timestamp        = 4002;
    sellEntry2.level            = LOG_LEVEL_VERBOSE;
    sellEntry2.instrumentId     = "aaaaa";
    sellEntry2.instrumentLogo   = &logo;
    sellEntry2.instrumentTicker = "ABBA";
    sellEntry2.instrumentName   = "Abstract Basics";
    sellEntry2.message          = "Order to sell 500 created with a price 2000.0 \u20BD";

    sellEntry3.timestamp        = 4003;
    sellEntry3.level            = LOG_LEVEL_VERBOSE;
    sellEntry3.instrumentId     = "aaaaa";
    sellEntry3.instrumentLogo   = &logo;
    sellEntry3.instrumentTicker = "ABBA";
    sellEntry3.instrumentName   = "Abstract Basics";
    sellEntry3.message          = "Order completed. 500 sold with a price 2000.0 \u20BD";

    sellEntry4.timestamp        = 4004;
    sellEntry4.level            = LOG_LEVEL_VERBOSE;
    sellEntry4.instrumentId     = "aaaaa";
    sellEntry4.instrumentLogo   = &logo;
    sellEntry4.instrumentTicker = "ABBA";
    sellEntry4.instrumentName   = "Abstract Basics";
    sellEntry4.message          = "Trade completed successfully";

    sellEntries2 << sellEntry1 << sellEntry2 << sellEntry3 << sellEntry4;

    Portfolio             sellPortfolio2;
    PortfolioCategoryItem sellCategory3;
    PortfolioCategoryItem sellCategory4;
    PortfolioItem         sellItem3;

    sellItem3.instrumentId       = RUBLE_UID;
    sellItem3.instrumentLogo     = &logo;
    sellItem3.instrumentTicker   = "RUBLE";
    sellItem3.instrumentName     = "Ruble";
    sellItem3.showPrices         = false;
    sellItem3.available          = 1074810.0;
    sellItem3.price              = 1.0f;
    sellItem3.avgPriceFifo       = 1.0f;
    sellItem3.avgPriceWavg       = 1.0f;
    sellItem3.cost               = 1074810.0;
    sellItem3.part               = 100.0;
    sellItem3.yield              = 0.0f;
    sellItem3.yieldPercent       = 0.0f;
    sellItem3.dailyYield         = 0.0f;
    sellItem3.priceForDailyYield = 0.0f;
    sellItem3.costForDailyYield  = 0.0;
    sellItem3.dailyYieldPercent  = 0.0f;
    sellItem3.pricePrecision     = 2;

    sellCategory3.id   = 0;
    sellCategory3.name = "Currency and metals";
    sellCategory3.cost = 1000000.0;
    sellCategory3.part = 100.0f;
    sellCategory3.items << sellItem3;

    sellCategory4.id   = 1;
    sellCategory4.name = "Share";
    sellCategory4.cost = 0.0;
    sellCategory4.part = 0.0f;

    sellPortfolio2.positions << sellCategory3 << sellCategory4;

    EXPECT_CALL(instrumentsStorageMock, readLock());
    EXPECT_CALL(instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instrumentsMap));
    EXPECT_CALL(instrumentsStorageMock, readUnlock());
    EXPECT_CALL(userStorageMock, readLock());
    EXPECT_CALL(userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(userStorageMock, readUnlock());
    EXPECT_CALL(logosStorageMock, readLock());
    EXPECT_CALL(logosStorageMock, getLogo(QString("aaaaa"))).WillOnce(Return(&logo));
    EXPECT_CALL(logosStorageMock, readUnlock());

    simulateTrading(
        4000,
        &instrumentsStorageMock,
        &logosStorageMock,
        &userStorageMock,
        instrumentsForTrading,
        1000000,
        totalMoney,
        operations,
        entries,
        portfolio,
        instruments,
        instrumentSells
    );

    // clang-format off
    ASSERT_NEAR(totalMoney,             1074810.0, 0.0001);
    ASSERT_EQ(operations,               sellOperations2);
    ASSERT_EQ(entries,                  sellEntries2);
    ASSERT_EQ(portfolio,                sellPortfolio2);
    ASSERT_EQ(instruments.size(),       0);
    ASSERT_EQ(instrumentSells.size(),   2);
    ASSERT_EQ(instrumentSells["aaaaa"], 4005);
    ASSERT_EQ(instrumentSells["bbbbb"], 3005);
    // clang-format on
}
