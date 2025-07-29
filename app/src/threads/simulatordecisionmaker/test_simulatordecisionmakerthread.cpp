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



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
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

    InstrumentsForTrading instrumentsForTrading;

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

    thread->run();
}

TEST_F(Test_SimulatorDecisionMakerThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
