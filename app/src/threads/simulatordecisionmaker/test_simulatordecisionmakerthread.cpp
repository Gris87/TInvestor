#include "src/threads/simulatordecisionmaker/simulatordecisionmakerthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/db/logs/ilogsdatabase_mock.h"
#include "src/db/operations/ioperationsdatabase_mock.h"
#include "src/db/portfolio/iportfoliodatabase_mock.h"
#include "src/decisions/idecisionmaker_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/optimizer/ioptimizer_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Ne;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
MATCHER_P(IsOperationsEqWithoutTimestamp, another, "")
{
    if (arg.size() != another.size())
    {
        return false;
    }

    for (int i = 0; i < arg.size(); ++i)
    {
        Operation        operation        = arg.at(i);
        const Operation& anotherOperation = another.at(i);

        operation.timestamp         = anotherOperation.timestamp;
        operation.originalTimestamp = anotherOperation.originalTimestamp;

        if (operation != anotherOperation)
        {
            return false;
        }
    }

    return true;
}

MATCHER_P(IsLogEntryEqWithoutTimestamp, another, "")
{
    LogEntry entry = arg;

    entry.timestamp = another.timestamp;

    return entry == another;
}



class Test_SimulatorDecisionMakerThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        settingsEditorMock     = new StrictMock<SettingsEditorMock>();
        operationsDatabaseMock = new StrictMock<OperationsDatabaseMock>();
        logsDatabaseMock       = new StrictMock<LogsDatabaseMock>();
        portfolioDatabaseMock  = new StrictMock<PortfolioDatabaseMock>();
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        logosStorageMock       = new StrictMock<LogosStorageMock>();
        userStorageMock        = new StrictMock<UserStorageMock>();
        stocksStorageMock      = new StrictMock<StocksStorageMock>();
        configMock             = new StrictMock<ConfigMock>();
        decisionMakerMock      = new StrictMock<DecisionMakerMock>();
        optimizerMock          = new StrictMock<OptimizerMock>();

        thread = new SimulatorDecisionMakerThread(
            settingsEditorMock,
            operationsDatabaseMock,
            logsDatabaseMock,
            portfolioDatabaseMock,
            instrumentsStorageMock,
            logosStorageMock,
            userStorageMock,
            stocksStorageMock,
            configMock,
            decisionMakerMock,
            optimizerMock
        );
    }

    void TearDown() override
    {
        delete thread;
        delete settingsEditorMock;
        delete operationsDatabaseMock;
        delete logsDatabaseMock;
        delete portfolioDatabaseMock;
        delete instrumentsStorageMock;
        delete logosStorageMock;
        delete userStorageMock;
        delete stocksStorageMock;
        delete configMock;
        delete decisionMakerMock;
        delete optimizerMock;
    }

    SimulatorDecisionMakerThread*       thread;
    StrictMock<SettingsEditorMock>*     settingsEditorMock;
    StrictMock<OperationsDatabaseMock>* operationsDatabaseMock;
    StrictMock<LogsDatabaseMock>*       logsDatabaseMock;
    StrictMock<PortfolioDatabaseMock>*  portfolioDatabaseMock;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<LogosStorageMock>*       logosStorageMock;
    StrictMock<UserStorageMock>*        userStorageMock;
    StrictMock<StocksStorageMock>*      stocksStorageMock;
    StrictMock<ConfigMock>*             configMock;
    StrictMock<DecisionMakerMock>*      decisionMakerMock;
    StrictMock<OptimizerMock>*          optimizerMock;
};



TEST_F(Test_SimulatorDecisionMakerThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_SimulatorDecisionMakerThread, Test_run)
{
    const InSequence seq;

    ASSERT_EQ(thread->isLoaded(), false);
    thread->reset();

    Instruments instruments;
    Instrument  instrument1;
    Instrument  instrument2;
    Instrument  instrument3;

    instrument1.ticker            = "RUBLE";
    instrument1.name              = "Ruble";
    instrument1.lot               = 1;
    instrument1.pricePrecision    = 2;
    instrument1.minPriceIncrement = Quotation(0, 10000000);

    instrument2.ticker            = "ABBA";
    instrument2.name              = "Abstract Basics";
    instrument2.lot               = 5;
    instrument2.pricePrecision    = 1;
    instrument2.minPriceIncrement = Quotation(0, 100000000);

    instrument3.ticker            = "BASE";
    instrument3.name              = "Basketball enhancement";
    instrument3.lot               = 10;
    instrument3.pricePrecision    = 2;
    instrument3.minPriceIncrement = Quotation(0, 10000000);

    instruments[RUBLE_UID] = instrument1;
    instruments["aaaaa"]   = instrument2;
    instruments["bbbbb"]   = instrument3;

    Logo logo;

    QList<Operation> operations;

    Operation operation;

    operation.timestamp                       = QDateTime::currentMSecsSinceEpoch();
    operation.originalTimestamp               = QDateTime::currentMSecsSinceEpoch();
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
    operation.costFifo                        = Quotation(0, 0);
    operation.costWavg                        = Quotation(0, 0);
    operation.commission                      = 0.0f;
    operation.yield                           = 0.0f;
    operation.yieldWithCommission             = 0.0f;
    operation.yieldWithCommissionPercent      = 0.0f;
    operation.inputMoney                      = Quotation(1000000, 0);
    operation.maxInputMoney                   = Quotation(1000000, 0);
    operation.totalYieldWithCommission        = Quotation(0, 0);
    operation.totalYieldWithCommissionPercent = 0.0f;
    operation.remainedMoney                   = Quotation(1000000, 0);
    operation.totalMoney                      = Quotation(1000000, 0);
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

    InstrumentSells instrumentSells;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    StockData stockData;

    stockData.timestamp = 1704110400000;
    stockData.quantity  = 1;
    stockData.price     = 250.0f;

    stock1.meta.instrumentId        = "N/A";
    stock1.meta.instrumentTicker    = "N/A";
    stock1.meta.instrumentName      = "N/A";
    stock1.meta.forQualInvestorFlag = false;
    stock1.meta.minPriceIncrement   = 0.01f;
    stock1.meta.pricePrecision      = 2;
    stock1.meta.lastTradeTime       = QTime(18, 39);
    stock1.meta.turnover            = 5000000;

    stock2.meta.instrumentId        = "aaaaa";
    stock2.meta.instrumentTicker    = "ABBA";
    stock2.meta.instrumentName      = "Abstract Basics";
    stock2.meta.forQualInvestorFlag = false;
    stock2.meta.minPriceIncrement   = 0.1f;
    stock2.meta.pricePrecision      = 1;
    stock2.meta.lastTradeTime       = QTime(23, 49);
    stock2.meta.turnover            = 2000000;
    stock2.data << stockData;

    stock3.meta.instrumentId        = "bbbbb";
    stock3.meta.instrumentTicker    = "BASE";
    stock3.meta.instrumentName      = "Basketball enhancement";
    stock3.meta.forQualInvestorFlag = false;
    stock3.meta.minPriceIncrement   = 0.1f;
    stock3.meta.pricePrecision      = 1;
    stock3.meta.lastTradeTime       = QTime(9, 59);
    stock3.meta.turnover            = 2000000;
    stock3.data << stockData;

    stocks << &stock1 << &stock2 << &stock3;

    InstrumentsForTrading instrumentsForTrading;
    TradingInfo           tradingInfo;

    tradingInfo.asapMode     = ASAP_MODE_NONE;
    tradingInfo.avgPrice     = -1.0f;
    tradingInfo.price        = 200.0f;
    tradingInfo.expectedCost = 100000.0;
    tradingInfo.cause        = "I want to buy";

    instrumentsForTrading["aaaaa"] = tradingInfo;

    QList<Operation> buyOperations;

    Operation buyOperation;

    buyOperation.timestamp                       = QDateTime::currentMSecsSinceEpoch();
    buyOperation.originalTimestamp               = QDateTime::currentMSecsSinceEpoch();
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
    buyOperation.costFifo                        = Quotation(100000, 0);
    buyOperation.costWavg                        = Quotation(100000, 0);
    buyOperation.commission                      = -40.0f;
    buyOperation.yield                           = 0.0f;
    buyOperation.yieldWithCommission             = -40.0f;
    buyOperation.yieldWithCommissionPercent      = -0.04f;
    buyOperation.inputMoney                      = Quotation(1000000, 0);
    buyOperation.maxInputMoney                   = Quotation(1000000, 0);
    buyOperation.totalYieldWithCommission        = Quotation(-39, -999998989);
    buyOperation.totalYieldWithCommissionPercent = -0.004f;
    buyOperation.remainedMoney                   = Quotation(899960, 1010);
    buyOperation.totalMoney                      = Quotation(999960, 1010);
    buyOperation.pricePrecision                  = 1;
    buyOperation.paymentPrecision                = 1;
    buyOperation.commissionPrecision             = 1;

    buyOperations << buyOperation;

    LogEntry buyEntry1;
    LogEntry buyEntry2;
    LogEntry buyEntry3;
    LogEntry buyEntry4;

    buyEntry1.timestamp        = QDateTime::currentMSecsSinceEpoch();
    buyEntry1.level            = LOG_LEVEL_DEBUG;
    buyEntry1.instrumentId     = "aaaaa";
    buyEntry1.instrumentLogo   = &logo;
    buyEntry1.instrumentTicker = "ABBA";
    buyEntry1.instrumentName   = "Abstract Basics";
    buyEntry1.message          = "I want to buy";

    buyEntry2.timestamp        = QDateTime::currentMSecsSinceEpoch();
    buyEntry2.level            = LOG_LEVEL_VERBOSE;
    buyEntry2.instrumentId     = "aaaaa";
    buyEntry2.instrumentLogo   = &logo;
    buyEntry2.instrumentTicker = "ABBA";
    buyEntry2.instrumentName   = "Abstract Basics";
    buyEntry2.message          = "Order to buy 500 created with a price 200.0 \u20BD";

    buyEntry3.timestamp        = QDateTime::currentMSecsSinceEpoch();
    buyEntry3.level            = LOG_LEVEL_VERBOSE;
    buyEntry3.instrumentId     = "aaaaa";
    buyEntry3.instrumentLogo   = &logo;
    buyEntry3.instrumentTicker = "ABBA";
    buyEntry3.instrumentName   = "Abstract Basics";
    buyEntry3.message          = "Order completed. 500/500 bought with a price 200.0 \u20BD";

    buyEntry4.timestamp        = QDateTime::currentMSecsSinceEpoch();
    buyEntry4.level            = LOG_LEVEL_VERBOSE;
    buyEntry4.instrumentId     = "aaaaa";
    buyEntry4.instrumentLogo   = &logo;
    buyEntry4.instrumentTicker = "ABBA";
    buyEntry4.instrumentName   = "Abstract Basics";
    buyEntry4.message          = "Trade completed successfully";

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
    buyItem1.part               = 89.9996f;
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
    buyItem2.price              = 250.0f;
    buyItem2.avgPriceFifo       = 200.0f;
    buyItem2.avgPriceWavg       = 200.0f;
    buyItem2.cost               = 100000.0;
    buyItem2.part               = 10.0004f;
    buyItem2.yield              = 25000.0f;
    buyItem2.yieldPercent       = 25.0f;
    buyItem2.dailyYield         = 25000.0f;
    buyItem2.priceForDailyYield = 200.0f;
    buyItem2.costForDailyYield  = 100000.0;
    buyItem2.dailyYieldPercent  = 25.0f;
    buyItem2.pricePrecision     = 1;

    buyCategory1.id   = 0;
    buyCategory1.name = "Currency and metals";
    buyCategory1.cost = 899960.0;
    buyCategory1.part = 89.9996f;
    buyCategory1.items << buyItem1;

    buyCategory2.id   = 1;
    buyCategory2.name = "Share";
    buyCategory2.cost = 100000.0;
    buyCategory2.part = 10.0004f;
    buyCategory2.items << buyItem2;

    buyPortfolio.positions << buyCategory1 << buyCategory2;

    InstrumentSells buyInstrumentSells;

    EXPECT_CALL(*settingsEditorMock, value(QString("Options/StartMoney"), QVariant(0))).WillOnce(Return(QVariant(1000000)));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*operationsDatabaseMock, writeOperations(IsOperationsEqWithoutTimestamp(operations), -1));
    EXPECT_CALL(*logsDatabaseMock, writeLogs(entries, -1));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*portfolioDatabaseMock, writePortfolio(portfolio, -1));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(), Ge(1704056400000), configMock, instrumentSells, portfolio, stocks, false, false, true
        )
    )
        .WillOnce(Return(instrumentsForTrading));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*operationsDatabaseMock, appendOperations(IsOperationsEqWithoutTimestamp(buyOperations), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(buyEntry1), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(buyEntry2), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(buyEntry3), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(buyEntry4), -1));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*portfolioDatabaseMock, writePortfolio(buyPortfolio, -1));

    thread->run();

    tradingInfo.asapMode     = ASAP_MODE_NONE;
    tradingInfo.avgPrice     = -1.0f;
    tradingInfo.price        = 200.0f;
    tradingInfo.expectedCost = 100000.0;
    tradingInfo.cause        = "I want to buy";

    instrumentsForTrading["aaaaa"] = tradingInfo;

    tradingInfo.asapMode     = ASAP_MODE_NONE;
    tradingInfo.avgPrice     = -1.0f;
    tradingInfo.price        = 200.0f;
    tradingInfo.expectedCost = 150000.0;
    tradingInfo.cause        = "I want to buy";

    instrumentsForTrading["bbbbb"] = tradingInfo;

    QList<Operation> buyOperations2;

    Operation buyOperation2;

    buyOperation2.timestamp                       = QDateTime::currentMSecsSinceEpoch();
    buyOperation2.originalTimestamp               = QDateTime::currentMSecsSinceEpoch();
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
    buyOperation2.costFifo                        = Quotation(150000, 0);
    buyOperation2.costWavg                        = Quotation(150000, 0);
    buyOperation2.commission                      = -60.0f;
    buyOperation2.yield                           = 0.0f;
    buyOperation2.yieldWithCommission             = -60.0f;
    buyOperation2.yieldWithCommissionPercent      = -0.04f;
    buyOperation2.inputMoney                      = Quotation(1000000, 0);
    buyOperation2.maxInputMoney                   = Quotation(1000000, 0);
    buyOperation2.totalYieldWithCommission        = Quotation(-99, -999997473);
    buyOperation2.totalYieldWithCommissionPercent = -0.01f;
    buyOperation2.remainedMoney                   = Quotation(749900, 2526);
    buyOperation2.totalMoney                      = Quotation(999900, 2526);
    buyOperation2.pricePrecision                  = 2;
    buyOperation2.paymentPrecision                = 2;
    buyOperation2.commissionPrecision             = 2;

    buyOperations2 << buyOperation2;

    buyEntry1.timestamp        = QDateTime::currentMSecsSinceEpoch();
    buyEntry1.level            = LOG_LEVEL_DEBUG;
    buyEntry1.instrumentId     = "bbbbb";
    buyEntry1.instrumentLogo   = &logo;
    buyEntry1.instrumentTicker = "BASE";
    buyEntry1.instrumentName   = "Basketball enhancement";
    buyEntry1.message          = "I want to buy";

    buyEntry2.timestamp        = QDateTime::currentMSecsSinceEpoch();
    buyEntry2.level            = LOG_LEVEL_VERBOSE;
    buyEntry2.instrumentId     = "bbbbb";
    buyEntry2.instrumentLogo   = &logo;
    buyEntry2.instrumentTicker = "BASE";
    buyEntry2.instrumentName   = "Basketball enhancement";
    buyEntry2.message          = "Order to buy 750 created with a price 200.00 \u20BD";

    buyEntry3.timestamp        = QDateTime::currentMSecsSinceEpoch();
    buyEntry3.level            = LOG_LEVEL_VERBOSE;
    buyEntry3.instrumentId     = "bbbbb";
    buyEntry3.instrumentLogo   = &logo;
    buyEntry3.instrumentTicker = "BASE";
    buyEntry3.instrumentName   = "Basketball enhancement";
    buyEntry3.message          = "Order completed. 750/750 bought with a price 200.00 \u20BD";

    buyEntry4.timestamp        = QDateTime::currentMSecsSinceEpoch();
    buyEntry4.level            = LOG_LEVEL_VERBOSE;
    buyEntry4.instrumentId     = "bbbbb";
    buyEntry4.instrumentLogo   = &logo;
    buyEntry4.instrumentTicker = "BASE";
    buyEntry4.instrumentName   = "Basketball enhancement";
    buyEntry4.message          = "Trade completed successfully";

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
    buyItem3.part               = 74.9975f;
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
    buyItem4.price              = 250.0f;
    buyItem4.avgPriceFifo       = 200.0f;
    buyItem4.avgPriceWavg       = 200.0f;
    buyItem4.cost               = 100000.0;
    buyItem4.part               = 10.001f;
    buyItem4.yield              = 25000.0f;
    buyItem4.yieldPercent       = 25.0f;
    buyItem4.dailyYield         = 25000.0f;
    buyItem4.priceForDailyYield = 200.0f;
    buyItem4.costForDailyYield  = 100000.0;
    buyItem4.dailyYieldPercent  = 25.0f;
    buyItem4.pricePrecision     = 1;

    buyItem5.instrumentId       = "bbbbb";
    buyItem5.instrumentLogo     = &logo;
    buyItem5.instrumentTicker   = "BASE";
    buyItem5.instrumentName     = "Basketball enhancement";
    buyItem5.showPrices         = true;
    buyItem5.available          = 750;
    buyItem5.price              = 250.0f;
    buyItem5.avgPriceFifo       = 200.0f;
    buyItem5.avgPriceWavg       = 200.0f;
    buyItem5.cost               = 150000.0;
    buyItem5.part               = 15.0015f;
    buyItem5.yield              = 37500.0f;
    buyItem5.yieldPercent       = 25.0f;
    buyItem5.dailyYield         = 37500.0f;
    buyItem5.priceForDailyYield = 200.0f;
    buyItem5.costForDailyYield  = 150000.0;
    buyItem5.dailyYieldPercent  = 25.0f;
    buyItem5.pricePrecision     = 2;

    buyCategory3.id   = 0;
    buyCategory3.name = "Currency and metals";
    buyCategory3.cost = 749900.0;
    buyCategory3.part = 74.9975f;
    buyCategory3.items << buyItem3;

    buyCategory4.id   = 1;
    buyCategory4.name = "Share";
    buyCategory4.cost = 250000.0;
    buyCategory4.part = 25.0025f;
    buyCategory4.items << buyItem4 << buyItem5;

    buyPortfolio2.positions << buyCategory3 << buyCategory4;

    InstrumentSells buyInstrumentSells2;

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(), Ge(1704056400000), configMock, buyInstrumentSells, buyPortfolio, stocks, false, false, true
        )
    )
        .WillOnce(Return(instrumentsForTrading));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("bbbbb"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*operationsDatabaseMock, appendOperations(IsOperationsEqWithoutTimestamp(buyOperations2), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(buyEntry1), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(buyEntry2), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(buyEntry3), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(buyEntry4), -1));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*portfolioDatabaseMock, writePortfolio(buyPortfolio2, -1));

    thread->run();

    tradingInfo.asapMode     = ASAP_MODE_NONE;
    tradingInfo.avgPrice     = 250.0f;
    tradingInfo.price        = 300.0f;
    tradingInfo.expectedCost = 0.0;
    tradingInfo.cause        = "I want to sell";

    instrumentsForTrading["bbbbb"] = tradingInfo;

    QList<Operation> sellOperations;

    Operation sellOperation;

    sellOperation.timestamp                       = QDateTime::currentMSecsSinceEpoch();
    sellOperation.originalTimestamp               = QDateTime::currentMSecsSinceEpoch();
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
    sellOperation.costFifo                        = Quotation(0, 0);
    sellOperation.costWavg                        = Quotation(0, 0);
    sellOperation.commission                      = -90.0f;
    sellOperation.yield                           = 75000.0f;
    sellOperation.yieldWithCommission             = 74910.0f;
    sellOperation.yieldWithCommissionPercent      = 49.94f;
    sellOperation.inputMoney                      = Quotation(1000000, 0);
    sellOperation.maxInputMoney                   = Quotation(1000000, 0);
    sellOperation.totalYieldWithCommission        = Quotation(74810, 4799);
    sellOperation.totalYieldWithCommissionPercent = 7.481f;
    sellOperation.remainedMoney                   = Quotation(974810, 4799);
    sellOperation.totalMoney                      = Quotation(1074810, 4799);
    sellOperation.pricePrecision                  = 2;
    sellOperation.paymentPrecision                = 2;
    sellOperation.commissionPrecision             = 2;

    sellOperations << sellOperation;

    LogEntry sellEntry1;
    LogEntry sellEntry2;
    LogEntry sellEntry3;
    LogEntry sellEntry4;

    sellEntry1.timestamp        = QDateTime::currentMSecsSinceEpoch();
    sellEntry1.level            = LOG_LEVEL_DEBUG;
    sellEntry1.instrumentId     = "bbbbb";
    sellEntry1.instrumentLogo   = &logo;
    sellEntry1.instrumentTicker = "BASE";
    sellEntry1.instrumentName   = "Basketball enhancement";
    sellEntry1.message          = "I want to sell";

    sellEntry2.timestamp        = QDateTime::currentMSecsSinceEpoch();
    sellEntry2.level            = LOG_LEVEL_VERBOSE;
    sellEntry2.instrumentId     = "bbbbb";
    sellEntry2.instrumentLogo   = &logo;
    sellEntry2.instrumentTicker = "BASE";
    sellEntry2.instrumentName   = "Basketball enhancement";
    sellEntry2.message          = "Order to sell 750 created with a price 300.00 \u20BD";

    sellEntry3.timestamp        = QDateTime::currentMSecsSinceEpoch();
    sellEntry3.level            = LOG_LEVEL_VERBOSE;
    sellEntry3.instrumentId     = "bbbbb";
    sellEntry3.instrumentLogo   = &logo;
    sellEntry3.instrumentTicker = "BASE";
    sellEntry3.instrumentName   = "Basketball enhancement";
    sellEntry3.message          = "Order completed. 750/750 sold with a price 300.00 \u20BD";

    sellEntry4.timestamp        = QDateTime::currentMSecsSinceEpoch();
    sellEntry4.level            = LOG_LEVEL_VERBOSE;
    sellEntry4.instrumentId     = "bbbbb";
    sellEntry4.instrumentLogo   = &logo;
    sellEntry4.instrumentTicker = "BASE";
    sellEntry4.instrumentName   = "Basketball enhancement";
    sellEntry4.message          = "Trade completed successfully";

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
    sellItem1.part               = 90.696f;
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
    sellItem2.price              = 250.0f;
    sellItem2.avgPriceFifo       = 200.0f;
    sellItem2.avgPriceWavg       = 200.0f;
    sellItem2.cost               = 100000.0;
    sellItem2.part               = 9.304f;
    sellItem2.yield              = 25000.0f;
    sellItem2.yieldPercent       = 25.0f;
    sellItem2.dailyYield         = 25000.0f;
    sellItem2.priceForDailyYield = 200.0f;
    sellItem2.costForDailyYield  = 100000.0;
    sellItem2.dailyYieldPercent  = 25.0f;
    sellItem2.pricePrecision     = 1;

    sellCategory1.id   = 0;
    sellCategory1.name = "Currency and metals";
    sellCategory1.cost = 974810.0;
    sellCategory1.part = 90.696f;
    sellCategory1.items << sellItem1;

    sellCategory2.id   = 1;
    sellCategory2.name = "Share";
    sellCategory2.cost = 100000.0;
    sellCategory2.part = 9.30397f;
    sellCategory2.items << sellItem2;

    sellPortfolio.positions << sellCategory1 << sellCategory2;

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(),
            Ge(1704056400000),
            configMock,
            buyInstrumentSells2,
            buyPortfolio2,
            stocks,
            false,
            false,
            true
        )
    )
        .WillOnce(Return(instrumentsForTrading));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("bbbbb"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*operationsDatabaseMock, appendOperations(IsOperationsEqWithoutTimestamp(sellOperations), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(sellEntry1), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(sellEntry2), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(sellEntry3), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(sellEntry4), -1));
    EXPECT_CALL(*portfolioDatabaseMock, writePortfolio(sellPortfolio, -1));

    thread->run();

    tradingInfo.asapMode     = ASAP_MODE_NONE;
    tradingInfo.avgPrice     = 1990.0f;
    tradingInfo.price        = 2000.0f;
    tradingInfo.expectedCost = 0.0;
    tradingInfo.cause        = "I want to sell";

    instrumentsForTrading["aaaaa"] = tradingInfo;

    QList<Operation> sellOperations2;

    Operation sellOperation2;

    sellOperation2.timestamp                       = QDateTime::currentMSecsSinceEpoch();
    sellOperation2.originalTimestamp               = QDateTime::currentMSecsSinceEpoch();
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
    sellOperation2.costFifo                        = Quotation(0, 0);
    sellOperation2.costWavg                        = Quotation(0, 0);
    sellOperation2.commission                      = 0.0f;
    sellOperation2.yield                           = 0.0f;
    sellOperation2.yieldWithCommission             = 0.0f;
    sellOperation2.yieldWithCommissionPercent      = 0.0f;
    sellOperation2.inputMoney                      = Quotation(1000000, 0);
    sellOperation2.maxInputMoney                   = Quotation(1000000, 0);
    sellOperation2.totalYieldWithCommission        = Quotation(74810, 4799);
    sellOperation2.totalYieldWithCommissionPercent = 7.481f;
    sellOperation2.remainedMoney                   = Quotation(1074810, 4799);
    sellOperation2.totalMoney                      = Quotation(1074810, 4799);
    sellOperation2.pricePrecision                  = 1;
    sellOperation2.paymentPrecision                = 1;
    sellOperation2.commissionPrecision             = 1;

    sellOperations2 << sellOperation2;

    sellEntry1.timestamp        = QDateTime::currentMSecsSinceEpoch();
    sellEntry1.level            = LOG_LEVEL_DEBUG;
    sellEntry1.instrumentId     = "aaaaa";
    sellEntry1.instrumentLogo   = &logo;
    sellEntry1.instrumentTicker = "ABBA";
    sellEntry1.instrumentName   = "Abstract Basics";
    sellEntry1.message          = "I want to sell";

    sellEntry2.timestamp        = QDateTime::currentMSecsSinceEpoch();
    sellEntry2.level            = LOG_LEVEL_VERBOSE;
    sellEntry2.instrumentId     = "aaaaa";
    sellEntry2.instrumentLogo   = &logo;
    sellEntry2.instrumentTicker = "ABBA";
    sellEntry2.instrumentName   = "Abstract Basics";
    sellEntry2.message          = "Order to sell 500 created with a price 2000.0 \u20BD";

    sellEntry3.timestamp        = QDateTime::currentMSecsSinceEpoch();
    sellEntry3.level            = LOG_LEVEL_VERBOSE;
    sellEntry3.instrumentId     = "aaaaa";
    sellEntry3.instrumentLogo   = &logo;
    sellEntry3.instrumentTicker = "ABBA";
    sellEntry3.instrumentName   = "Abstract Basics";
    sellEntry3.message          = "Order completed. 500/500 sold with a price 2000.0 \u20BD";

    sellEntry4.timestamp        = QDateTime::currentMSecsSinceEpoch();
    sellEntry4.level            = LOG_LEVEL_VERBOSE;
    sellEntry4.instrumentId     = "aaaaa";
    sellEntry4.instrumentLogo   = &logo;
    sellEntry4.instrumentTicker = "ABBA";
    sellEntry4.instrumentName   = "Abstract Basics";
    sellEntry4.message          = "Trade completed successfully";

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
    sellCategory3.cost = 1074810.0;
    sellCategory3.part = 100.0f;
    sellCategory3.items << sellItem3;

    sellCategory4.id   = 1;
    sellCategory4.name = "Share";
    sellCategory4.cost = 0.0;
    sellCategory4.part = 0.0f;

    sellPortfolio2.positions << sellCategory3 << sellCategory4;

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(),
            Ge(1704056400000),
            configMock,
            Ne(buyInstrumentSells2),
            sellPortfolio,
            stocks,
            false,
            false,
            true
        )
    )
        .WillOnce(Return(instrumentsForTrading));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*operationsDatabaseMock, appendOperations(IsOperationsEqWithoutTimestamp(sellOperations2), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(sellEntry1), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(sellEntry2), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(sellEntry3), -1));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimestamp(sellEntry4), -1));
    EXPECT_CALL(*portfolioDatabaseMock, writePortfolio(sellPortfolio2, -1));

    thread->run();
}

TEST_F(Test_SimulatorDecisionMakerThread, Test_terminateThread)
{
    thread->terminateThread();
}

TEST_F(Test_SimulatorDecisionMakerThread, Test_optimizeOperations_and_optimizeLogs)
{
    const InSequence seq;

    Logo logo;

    QList<Operation> operations;
    QList<Operation> optimizedOperations;

    operations.resizeForOverwrite(11);
    optimizedOperations.resizeForOverwrite(5);

    thread->testSetLimitOperations(operations.size() - 1);
    thread->testSetOptimizeOperationsSize(optimizedOperations.size());

    for (int i = 0; i < operations.size() - 1; i += 2)
    {
        Operation& operation1 = operations[i];
        Operation& operation2 = operations[i + 1];

        operation1.timestamp                       = operations.size() - i;
        operation1.originalTimestamp               = operations.size() - i;
        operation1.instrumentId                    = "aaaaa";
        operation1.instrumentTicker                = "aaaaa";
        operation1.instrumentName                  = "?????";
        operation1.description                     = "Sell 10 ivashka durashka shares";
        operation1.price                           = 280.0f;
        operation1.avgPriceFifo                    = 253.3f;
        operation1.avgPriceWavg                    = 253.3f;
        operation1.quantity                        = 10;
        operation1.remainedQuantity                = 0;
        operation1.payment                         = 2800.0f;
        operation1.avgCostFifo                     = 2533.0f;
        operation1.costFifo                        = Quotation(0, 0);
        operation1.costWavg                        = Quotation(0, 0);
        operation1.commission                      = -1.4f;
        operation1.yield                           = 267.0f;
        operation1.yieldWithCommission             = 265.6f;
        operation1.yieldWithCommissionPercent      = 10.4856f;
        operation1.inputMoney                      = Quotation(200000, 0);
        operation1.maxInputMoney                   = Quotation(200000, 0);
        operation1.totalYieldWithCommission        = Quotation(265, -666500000);
        operation1.totalYieldWithCommissionPercent = 0.1321667f;
        operation1.remainedMoney                   = Quotation(200300, 0);
        operation1.totalMoney                      = Quotation(200300, 0);
        operation1.pricePrecision                  = 2;
        operation1.paymentPrecision                = 2;
        operation1.commissionPrecision             = 2;

        operation2.timestamp                       = operations.size() - i - 1;
        operation2.originalTimestamp               = operations.size() - i - 1;
        operation2.instrumentId                    = "aaaaa";
        operation2.instrumentTicker                = "aaaaa";
        operation2.instrumentName                  = "?????";
        operation2.description                     = "Buy 10 ivashka durashka shares";
        operation2.price                           = 253.3f;
        operation2.avgPriceFifo                    = 253.3f;
        operation2.avgPriceWavg                    = 253.3f;
        operation2.quantity                        = 10;
        operation2.remainedQuantity                = 10;
        operation2.payment                         = -2533.0f;
        operation2.avgCostFifo                     = 2533.0f;
        operation2.costFifo                        = Quotation(2533, 0);
        operation2.costWavg                        = Quotation(2533, 0);
        operation2.commission                      = -1.2665f;
        operation2.yield                           = 0.0f;
        operation2.yieldWithCommission             = -1.2665f;
        operation2.yieldWithCommissionPercent      = -0.05f;
        operation2.inputMoney                      = Quotation(200000, 0);
        operation2.maxInputMoney                   = Quotation(200000, 0);
        operation2.totalYieldWithCommission        = Quotation(-1, -266500000);
        operation2.totalYieldWithCommissionPercent = -0.0006332f;
        operation2.remainedMoney                   = Quotation(197466, -266500000);
        operation2.totalMoney                      = Quotation(199999, -266500000);
        operation2.pricePrecision                  = 2;
        operation2.paymentPrecision                = 2;
        operation2.commissionPrecision             = 4;
    }

    for (int i = 0; i < optimizedOperations.size() - 1; i += 2)
    {
        Operation& operation1 = optimizedOperations[i];
        Operation& operation2 = optimizedOperations[i + 1];

        operation1.timestamp                       = operations.size() - i;
        operation1.originalTimestamp               = operations.size() - i;
        operation1.instrumentId                    = "aaaaa";
        operation1.instrumentTicker                = "aaaaa";
        operation1.instrumentName                  = "?????";
        operation1.description                     = "Sell 10 ivashka durashka shares";
        operation1.price                           = 280.0f;
        operation1.avgPriceFifo                    = 253.3f;
        operation1.avgPriceWavg                    = 253.3f;
        operation1.quantity                        = 10;
        operation1.remainedQuantity                = 0;
        operation1.payment                         = 2800.0f;
        operation1.avgCostFifo                     = 2533.0f;
        operation1.costFifo                        = Quotation(0, 0);
        operation1.costWavg                        = Quotation(0, 0);
        operation1.commission                      = -1.4f;
        operation1.yield                           = 267.0f;
        operation1.yieldWithCommission             = 265.6f;
        operation1.yieldWithCommissionPercent      = 10.4856f;
        operation1.inputMoney                      = Quotation(200000, 0);
        operation1.maxInputMoney                   = Quotation(200000, 0);
        operation1.totalYieldWithCommission        = Quotation(265, -666500000);
        operation1.totalYieldWithCommissionPercent = 0.1321667f;
        operation1.remainedMoney                   = Quotation(200300, 0);
        operation1.totalMoney                      = Quotation(200300, 0);
        operation1.pricePrecision                  = 2;
        operation1.paymentPrecision                = 2;
        operation1.commissionPrecision             = 2;

        operation2.timestamp                       = operations.size() - i - 1;
        operation2.originalTimestamp               = operations.size() - i - 1;
        operation2.instrumentId                    = "aaaaa";
        operation2.instrumentTicker                = "aaaaa";
        operation2.instrumentName                  = "?????";
        operation2.description                     = "Buy 10 ivashka durashka shares";
        operation2.price                           = 253.3f;
        operation2.avgPriceFifo                    = 253.3f;
        operation2.avgPriceWavg                    = 253.3f;
        operation2.quantity                        = 10;
        operation2.remainedQuantity                = 10;
        operation2.payment                         = -2533.0f;
        operation2.avgCostFifo                     = 2533.0f;
        operation2.costFifo                        = Quotation(2533, 0);
        operation2.costWavg                        = Quotation(2533, 0);
        operation2.commission                      = -1.2665f;
        operation2.yield                           = 0.0f;
        operation2.yieldWithCommission             = -1.2665f;
        operation2.yieldWithCommissionPercent      = -0.05f;
        operation2.inputMoney                      = Quotation(200000, 0);
        operation2.maxInputMoney                   = Quotation(200000, 0);
        operation2.totalYieldWithCommission        = Quotation(-1, -266500000);
        operation2.totalYieldWithCommissionPercent = -0.0006332f;
        operation2.remainedMoney                   = Quotation(197466, -266500000);
        operation2.totalMoney                      = Quotation(199999, -266500000);
        operation2.pricePrecision                  = 2;
        operation2.paymentPrecision                = 2;
        operation2.commissionPrecision             = 4;
    }

    QList<LogEntry> entries;
    QList<LogEntry> optimizedEntries;

    entries.resizeForOverwrite(11);
    optimizedEntries.resizeForOverwrite(5);

    thread->testSetLimitLogs(entries.size() - 1);
    thread->testSetOptimizeLogsSize(optimizedEntries.size());

    for (int i = 0; i < entries.size(); ++i)
    {
        LogEntry& entry = entries[i];

        entry.timestamp        = entries.size() - i;
        entry.level            = LOG_LEVEL_DEBUG;
        entry.instrumentId     = "aaaaa";
        entry.instrumentLogo   = nullptr;
        entry.instrumentTicker = "MAFA";
        entry.instrumentName   = "Mathafaka";
        entry.message          = "Buy without reason";
    }

    for (int i = 0; i < optimizedEntries.size(); ++i)
    {
        LogEntry& entry = optimizedEntries[i];

        entry.timestamp        = entries.size() - i;
        entry.level            = LOG_LEVEL_DEBUG;
        entry.instrumentId     = "aaaaa";
        entry.instrumentLogo   = nullptr;
        entry.instrumentTicker = "MAFA";
        entry.instrumentName   = "Mathafaka";
        entry.message          = "Buy without reason";
    }

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item1;
    PortfolioItem         item2;

    item1.instrumentId       = RUBLE_UID;
    item1.instrumentLogo     = &logo;
    item1.instrumentTicker   = "RUBLE";
    item1.instrumentName     = "Ruble";
    item1.showPrices         = false;
    item1.available          = 899960.0;
    item1.price              = 1.0f;
    item1.avgPriceFifo       = 1.0f;
    item1.avgPriceWavg       = 1.0f;
    item1.cost               = 899960.0;
    item1.part               = 89.9996f;
    item1.yield              = 0.0f;
    item1.yieldPercent       = 0.0f;
    item1.dailyYield         = 0.0f;
    item1.priceForDailyYield = 0.0f;
    item1.costForDailyYield  = 0.0;
    item1.dailyYieldPercent  = 0.0f;
    item1.pricePrecision     = 2;

    item2.instrumentId       = "aaaaa";
    item2.instrumentLogo     = &logo;
    item2.instrumentTicker   = "ABBA";
    item2.instrumentName     = "Abstract Basics";
    item2.showPrices         = true;
    item2.available          = 500;
    item2.price              = 250.0f;
    item2.avgPriceFifo       = 200.0f;
    item2.avgPriceWavg       = 200.0f;
    item2.cost               = 100000.0;
    item2.part               = 10.0004f;
    item2.yield              = 25000.0f;
    item2.yieldPercent       = 25.0f;
    item2.dailyYield         = 25000.0f;
    item2.priceForDailyYield = 200.0f;
    item2.costForDailyYield  = 100000.0;
    item2.dailyYieldPercent  = 25.0f;
    item2.pricePrecision     = 1;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 899960.0;
    category1.part = 89.9996f;
    category1.items << item1;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 100000.0;
    category2.part = 10.0004f;
    category2.items << item2;

    portfolio.positions << category1 << category2;

    QList<Stock*> stocks;

    InstrumentsForTrading instrumentsForTrading;

    EXPECT_CALL(*settingsEditorMock, value(QString("Options/StartMoney"), QVariant(0))).WillOnce(Return(QVariant(1000000)));
    EXPECT_CALL(*operationsDatabaseMock, readOperations(-1)).WillOnce(Return(operations));
    EXPECT_CALL(*logsDatabaseMock, readLogs(-1)).WillOnce(Return(entries));
    EXPECT_CALL(*portfolioDatabaseMock, readPortfolio(-1)).WillOnce(Return(portfolio));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(), Ge(1704056400000), configMock, Ne(InstrumentSells()), portfolio, stocks, false, false, true
        )
    )
        .WillOnce(Return(instrumentsForTrading));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*operationsDatabaseMock, readOperations(-1)).WillOnce(Return(operations));
    EXPECT_CALL(*optimizerMock, optimizeOperations(operations, 5, QStringList() << "aaaaa"))
        .WillOnce(Return(optimizedOperations));
    EXPECT_CALL(*operationsDatabaseMock, writeOperations(optimizedOperations, -1));
    EXPECT_CALL(*logsDatabaseMock, readLogs(-1)).WillOnce(Return(entries));
    EXPECT_CALL(*optimizerMock, optimizeLogs(entries, 5)).WillOnce(Return(optimizedEntries));
    EXPECT_CALL(*logsDatabaseMock, writeLogs(optimizedEntries, -1));

    thread->run();
}
// NOLINTEND(readability-magic-numbers)
