#include "src/threads/simulatordecisionmaker/simulatordecisionmakerthread.h"

#include <gtest/gtest.h>

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
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
MATCHER_P(IsOperationsEqWithoutTimeout, another, "")
{
    if (arg.size() != another.size())
    {
        return false;
    }

    for (int i = 0; i < arg.size(); ++i)
    {
        Operation        operation        = arg.at(i);
        const Operation& anotherOperation = arg.at(i);

        operation.timestamp = anotherOperation.timestamp;

        if (operation != anotherOperation)
        {
            return false;
        }
    }

    return true;
}

MATCHER_P(IsLogEntryEqWithoutTimeout, another, "")
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
    StrictMock<DecisionMakerMock>*      decisionMakerMock;
    StrictMock<OptimizerMock>*          optimizerMock;
};



TEST_F(Test_SimulatorDecisionMakerThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_SimulatorDecisionMakerThread, Test_run)
{
    const InSequence seq;

    thread->reset();

    Instruments instruments;
    Instrument  instrument1;
    Instrument  instrument2;

    instrument1.ticker         = "RUBLE";
    instrument1.name           = "Ruble";
    instrument1.lot            = 1;
    instrument1.pricePrecision = 2;

    instrument2.ticker         = "ABBA";
    instrument2.name           = "Abstract Basics";
    instrument2.lot            = 5;
    instrument2.pricePrecision = 1;

    instruments[RUBLE_UID] = instrument1;
    instruments["aaaaa"]   = instrument2;

    Logo logo;

    QList<Operation> operations;

    Operation operation;

    operation.timestamp                       = QDateTime::currentMSecsSinceEpoch();
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

    operations.append(operation);

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
    category1.items.append(item);

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 0.0;
    category2.part = 0.0;

    portfolio.positions << category1 << category2;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    StockData stockData;

    stockData.timestamp = 1704110400000;
    stockData.quantity  = 1;
    stockData.price     = 250.0f;

    stock1.meta.instrumentId        = "N/A";
    stock1.meta.instrumentTicker    = "N/A";
    stock1.meta.instrumentName      = "N/A";
    stock1.meta.forQualInvestorFlag = false;
    stock1.meta.minPriceIncrement   = 0.01f;
    stock1.meta.turnover            = 5000000;
    stock1.meta.pricePrecision      = 2;
    stock2.meta.instrumentId        = "aaaaa";
    stock2.meta.instrumentTicker    = "ABBA";
    stock2.meta.instrumentName      = "Abstract Basics";
    stock2.meta.forQualInvestorFlag = false;
    stock2.meta.minPriceIncrement   = 0.1f;
    stock2.meta.turnover            = 2000000;
    stock2.meta.pricePrecision      = 1;
    stock2.data << stockData;

    stocks << &stock1 << &stock2;

    InstrumentsForTrading instrumentsForTrading;
    TradingInfo           tradingInfo;

    tradingInfo.price        = 200.0f;
    tradingInfo.expectedCost = 100000.0;
    tradingInfo.cause        = "I want to buy";

    instrumentsForTrading["aaaaa"] = tradingInfo;

    QList<Operation> buyOperations;

    Operation buyOperation;

    buyOperation.timestamp                       = QDateTime::currentMSecsSinceEpoch();
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
    buyOperation.costFifo.units                  = 0;
    buyOperation.costFifo.nano                   = 100000;
    buyOperation.costWavg.units                  = 0;
    buyOperation.costWavg.nano                   = 100000;
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
    buyOperation.totalYieldWithCommissionPercent = -0.04f;
    buyOperation.remainedMoney.units             = 899960;
    buyOperation.remainedMoney.nano              = 1010;
    buyOperation.totalMoney.units                = 999960;
    buyOperation.totalMoney.nano                 = 1010;
    buyOperation.pricePrecision                  = 1;
    buyOperation.paymentPrecision                = 1;
    buyOperation.commissionPrecision             = 1;

    buyOperations.append(buyOperation);

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
    buyEntry3.message          = "Order completed. 500 bought with a price 200.0 \u20BD";

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
    buyCategory1.items.append(buyItem1);

    buyCategory2.id   = 1;
    buyCategory2.name = "Share";
    buyCategory2.cost = 100000.0;
    buyCategory2.part = 10.0004f;
    buyCategory2.items.append(buyItem2);

    buyPortfolio.positions << buyCategory1 << buyCategory2;

    EXPECT_CALL(*settingsEditorMock, value(QString("Options/StartMoney"), QVariant(0))).WillOnce(Return(QVariant(1000000)));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*operationsDatabaseMock, writeOperations(IsOperationsEqWithoutTimeout(operations)));
    EXPECT_CALL(*logsDatabaseMock, writeLogs(entries));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*portfolioDatabaseMock, writePortfolio(portfolio));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*decisionMakerMock, makeDecision(Ge(1704056400000), portfolio, stocks, 0, false))
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
    EXPECT_CALL(*operationsDatabaseMock, appendOperations(IsOperationsEqWithoutTimeout(buyOperations)));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimeout(buyEntry1)));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimeout(buyEntry2)));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimeout(buyEntry3)));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimeout(buyEntry4)));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*portfolioDatabaseMock, writePortfolio(buyPortfolio));

    thread->run();

    tradingInfo.price        = 220.0f;
    tradingInfo.expectedCost = 0.0;
    tradingInfo.cause        = "I want to sell";

    instrumentsForTrading["aaaaa"] = tradingInfo;

    QList<Operation> sellOperations;

    Operation sellOperation;

    sellOperation.timestamp                       = QDateTime::currentMSecsSinceEpoch();
    sellOperation.instrumentId                    = "aaaaa";
    sellOperation.instrumentLogo                  = &logo;
    sellOperation.instrumentTicker                = "ABBA";
    sellOperation.instrumentName                  = "Abstract Basics";
    sellOperation.description                     = "Sale of shares";
    sellOperation.price                           = 220.0f;
    sellOperation.avgPriceFifo                    = 200.0f;
    sellOperation.avgPriceWavg                    = 200.0f;
    sellOperation.quantity                        = 500;
    sellOperation.remainedQuantity                = 0;
    sellOperation.payment                         = 110000.0f;
    sellOperation.avgCostFifo                     = 100000.0f;
    sellOperation.costFifo.units                  = 0;
    sellOperation.costFifo.nano                   = 0;
    sellOperation.costWavg.units                  = 0;
    sellOperation.costWavg.nano                   = 0;
    sellOperation.commission                      = -44.0f;
    sellOperation.yield                           = 10000.0f;
    sellOperation.yieldWithCommission             = 9956.0f;
    sellOperation.yieldWithCommissionPercent      = 9.956f;
    sellOperation.inputMoney.units                = 1000000;
    sellOperation.inputMoney.nano                 = 0;
    sellOperation.maxInputMoney.units             = 1000000;
    sellOperation.maxInputMoney.nano              = 0;
    sellOperation.totalYieldWithCommission.units  = 9916;
    sellOperation.totalYieldWithCommission.nano   = 2122;
    sellOperation.totalYieldWithCommissionPercent = 0.9915f;
    sellOperation.remainedMoney.units             = 1009916;
    sellOperation.remainedMoney.nano              = 2122;
    sellOperation.totalMoney.units                = 1009916;
    sellOperation.totalMoney.nano                 = 2122;
    sellOperation.pricePrecision                  = 1;
    sellOperation.paymentPrecision                = 1;
    sellOperation.commissionPrecision             = 1;

    sellOperations.append(sellOperation);

    LogEntry sellEntry1;
    LogEntry sellEntry2;
    LogEntry sellEntry3;
    LogEntry sellEntry4;

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
    sellEntry2.message          = "Order to sell 500 created with a price 220.0 \u20BD";

    sellEntry3.timestamp        = QDateTime::currentMSecsSinceEpoch();
    sellEntry3.level            = LOG_LEVEL_VERBOSE;
    sellEntry3.instrumentId     = "aaaaa";
    sellEntry3.instrumentLogo   = &logo;
    sellEntry3.instrumentTicker = "ABBA";
    sellEntry3.instrumentName   = "Abstract Basics";
    sellEntry3.message          = "Order completed. 500 sold with a price 220.0 \u20BD";

    sellEntry4.timestamp        = QDateTime::currentMSecsSinceEpoch();
    sellEntry4.level            = LOG_LEVEL_VERBOSE;
    sellEntry4.instrumentId     = "aaaaa";
    sellEntry4.instrumentLogo   = &logo;
    sellEntry4.instrumentTicker = "ABBA";
    sellEntry4.instrumentName   = "Abstract Basics";
    sellEntry4.message          = "Trade completed successfully";

    Portfolio             sellPortfolio;
    PortfolioCategoryItem sellCategory1;
    PortfolioCategoryItem sellCategory2;
    PortfolioItem         sellItem;

    sellItem.instrumentId       = RUBLE_UID;
    sellItem.instrumentLogo     = &logo;
    sellItem.instrumentTicker   = "RUBLE";
    sellItem.instrumentName     = "Ruble";
    sellItem.showPrices         = false;
    sellItem.available          = 1009916;
    sellItem.price              = 1.0f;
    sellItem.avgPriceFifo       = 1.0f;
    sellItem.avgPriceWavg       = 1.0f;
    sellItem.cost               = 1009916;
    sellItem.part               = 100.0;
    sellItem.yield              = 0.0f;
    sellItem.yieldPercent       = 0.0f;
    sellItem.dailyYield         = 0.0f;
    sellItem.priceForDailyYield = 0.0f;
    sellItem.costForDailyYield  = 0.0;
    sellItem.dailyYieldPercent  = 0.0f;
    sellItem.pricePrecision     = 2;

    sellCategory1.id   = 0;
    sellCategory1.name = "Currency and metals";
    sellCategory1.cost = 1009916;
    sellCategory1.part = 100.0;
    sellCategory1.items.append(sellItem);

    sellCategory2.id   = 1;
    sellCategory2.name = "Share";
    sellCategory2.cost = 0.0;
    sellCategory2.part = 0.0;

    sellPortfolio.positions << sellCategory1 << sellCategory2;

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*decisionMakerMock, makeDecision(Ge(1704056400000), buyPortfolio, stocks, 0, false))
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
    EXPECT_CALL(*operationsDatabaseMock, appendOperations(IsOperationsEqWithoutTimeout(sellOperations)));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimeout(sellEntry1)));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimeout(sellEntry2)));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimeout(sellEntry3)));
    EXPECT_CALL(*logsDatabaseMock, appendLog(IsLogEntryEqWithoutTimeout(sellEntry4)));
    EXPECT_CALL(*portfolioDatabaseMock, writePortfolio(sellPortfolio));

    thread->run();
}

TEST_F(Test_SimulatorDecisionMakerThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(readability-magic-numbers)
