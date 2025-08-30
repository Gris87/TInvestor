#include "src/threads/simulatordaterangedecisionmaker/simulatordaterangedecisionmakerthread.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/iconfig_mock.h"
#include "src/db/logs/ilogsdatabase_mock.h"
#include "src/db/operations/ioperationsdatabase_mock.h"
#include "src/db/portfolio/iportfoliodatabase_mock.h"
#include "src/decisions/idecisionmaker_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"
#include "src/utils/optimizer/ioptimizer_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr int AMOUNT_OF_BUY_DECISIONS = 4;



using ::testing::_;
using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_SimulatorDateRangeDecisionMakerThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        appDir = qApp->applicationDirPath();

        dirFactoryMock         = new StrictMock<DirFactoryMock>();
        fileFactoryMock        = new StrictMock<FileFactoryMock>();
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

        thread = new SimulatorDateRangeDecisionMakerThread(
            dirFactoryMock,
            fileFactoryMock,
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
        delete dirFactoryMock;
        delete fileFactoryMock;
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

    SimulatorDateRangeDecisionMakerThread* thread;
    StrictMock<DirFactoryMock>*            dirFactoryMock;
    StrictMock<FileFactoryMock>*           fileFactoryMock;
    StrictMock<SettingsEditorMock>*        settingsEditorMock;
    StrictMock<OperationsDatabaseMock>*    operationsDatabaseMock;
    StrictMock<LogsDatabaseMock>*          logsDatabaseMock;
    StrictMock<PortfolioDatabaseMock>*     portfolioDatabaseMock;
    StrictMock<InstrumentsStorageMock>*    instrumentsStorageMock;
    StrictMock<LogosStorageMock>*          logosStorageMock;
    StrictMock<UserStorageMock>*           userStorageMock;
    StrictMock<StocksStorageMock>*         stocksStorageMock;
    StrictMock<ConfigMock>*                configMock;
    StrictMock<DecisionMakerMock>*         decisionMakerMock;
    StrictMock<OptimizerMock>*             optimizerMock;
    QString                                appDir;
};



TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_run)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DirMock>*                dirMock1 = new StrictMock<DirMock>();  // Will be deleted in initConfigs function
    StrictMock<DirMock>*                dirMock2 = new StrictMock<DirMock>();  // Will be deleted in initConfigs function
    StrictMock<FileMock>* configFileWriteMock1   = new StrictMock<FileMock>(); // Will be deleted in initConfigs function
    StrictMock<FileMock>* configFileWriteMock2   = new StrictMock<FileMock>(); // Will be deleted in initConfigs function
    StrictMock<FileMock>* configFileWriteMock3   = new StrictMock<FileMock>(); // Will be deleted in initConfigs function
    StrictMock<FileMock>* configFileWriteMock4   = new StrictMock<FileMock>(); // Will be deleted in initConfigs function
    StrictMock<FileMock>* configFileWriteMock    = new StrictMock<FileMock>(); // Will be deleted in initConfigs function

    QList<StrictMock<FileMock>*> configFileMocksForWriting = {
        configFileWriteMock1, configFileWriteMock2, configFileWriteMock3, configFileWriteMock4
    };

    StrictMock<ConfigMock>              clonedConfigMock;
    StrictMock<DecisionMakerConfigMock> clonedSimulatorConfigMock;

    // Will be deleted in simulationWithBestConfigStep1 function
    StrictMock<FileMock>* totalConfigFileWriteMock = new StrictMock<FileMock>();

    // Will be deleted in simulationWithBestConfigStep2 function
    StrictMock<FileMock>* totalConfigFileReadMock1 = new StrictMock<FileMock>();
    StrictMock<FileMock>* totalConfigFileReadMock2 = new StrictMock<FileMock>();

    StrictMock<FileMock>* configFileReadMock1 = new StrictMock<FileMock>(); // Will be deleted in loadConfigs function
    StrictMock<FileMock>* configFileReadMock2 = new StrictMock<FileMock>(); // Will be deleted in loadConfigs function
    StrictMock<FileMock>* configFileReadMock3 = new StrictMock<FileMock>(); // Will be deleted in loadConfigs function
    StrictMock<FileMock>* configFileReadMock4 = new StrictMock<FileMock>(); // Will be deleted in loadConfigs function

    QList<StrictMock<FileMock>*> configFileMocksForReading = {
        configFileReadMock1, configFileReadMock2, configFileReadMock3, configFileReadMock4
    };

    thread->reset();

    Instruments instruments;
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

    instruments[RUBLE_UID] = instrument1;
    instruments["aaaaa"]   = instrument2;
    instruments["bbbbb"]   = instrument3;

    Logo logo;

    QStringList configVariants = {
        R"([{"b1":{"enabled":true},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"s1":{"enabled":true},"s2":{"enabled":false},"s3":{"enabled":false},"s4":{"enabled":false}}])",
        R"([{"b1":{"enabled":false},"b2":{"enabled":true},"b3":{"enabled":false},"b4":{"enabled":false},"s1":{"enabled":false},"s2":{"enabled":true},"s3":{"enabled":false},"s4":{"enabled":false}}])",
        R"([{"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":true},"b4":{"enabled":false},"s1":{"enabled":true},"s2":{"enabled":false},"s3":{"enabled":false},"s4":{"enabled":false}}])",
        R"([{"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":true},"s1":{"enabled":false},"s2":{"enabled":true},"s3":{"enabled":false},"s4":{"enabled":false}}])"
    };

    QString configVariant = {
        R"({"b1":{"enabled":true},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"s1":{"enabled":true},"s2":{"enabled":false},"s3":{"enabled":false},"s4":{"enabled":false}})"
    };

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
    stock3.meta.instrumentId        = "bbbbb";
    stock3.meta.instrumentTicker    = "BASE";
    stock3.meta.instrumentName      = "Basketball enhancement";
    stock3.meta.forQualInvestorFlag = false;
    stock3.meta.minPriceIncrement   = 0.1f;
    stock3.meta.turnover            = 2000000;
    stock3.meta.pricePrecision      = 1;
    stock3.data << stockData;

    stocks << &stock1 << &stock2 << &stock3;

    QList<Operation> bestLocalOperations;

    Operation operation;

    operation.timestamp                       = 1704056400001;
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

    bestLocalOperations << operation;

    QList<LogEntry> bestLocalEntries;

    Portfolio             bestLocalPortfolio;
    PortfolioCategoryItem bestLocalCategory1;
    PortfolioCategoryItem bestLocalCategory2;
    PortfolioItem         bestLocalItem;

    bestLocalItem.instrumentId       = RUBLE_UID;
    bestLocalItem.instrumentLogo     = &logo;
    bestLocalItem.instrumentTicker   = "RUBLE";
    bestLocalItem.instrumentName     = "Ruble";
    bestLocalItem.showPrices         = false;
    bestLocalItem.available          = 1000000;
    bestLocalItem.price              = 1.0f;
    bestLocalItem.avgPriceFifo       = 1.0f;
    bestLocalItem.avgPriceWavg       = 1.0f;
    bestLocalItem.cost               = 1000000;
    bestLocalItem.part               = 100.0;
    bestLocalItem.yield              = 0.0f;
    bestLocalItem.yieldPercent       = 0.0f;
    bestLocalItem.dailyYield         = 0.0f;
    bestLocalItem.priceForDailyYield = 0.0f;
    bestLocalItem.costForDailyYield  = 0.0;
    bestLocalItem.dailyYieldPercent  = 0.0f;
    bestLocalItem.pricePrecision     = 2;

    bestLocalCategory1.id   = 0;
    bestLocalCategory1.name = "Currency and metals";
    bestLocalCategory1.cost = 1000000;
    bestLocalCategory1.part = 100.0;
    bestLocalCategory1.items << bestLocalItem;

    bestLocalCategory2.id   = 1;
    bestLocalCategory2.name = "Share";
    bestLocalCategory2.cost = 0.0;
    bestLocalCategory2.part = 0.0;

    bestLocalPortfolio.positions << bestLocalCategory1 << bestLocalCategory2;

    QList<Operation> bestOperations;

    operation.timestamp                       = 1704056400000;
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

    bestOperations.prepend(operation);

    QList<LogEntry> bestEntries;

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

    InstrumentsForTrading instrumentsForTrading1;
    TradingInfo           tradingInfo;

    tradingInfo.asap         = false;
    tradingInfo.avgPrice     = -1.0f;
    tradingInfo.price        = 200.0f;
    tradingInfo.expectedCost = 100000.0;
    tradingInfo.cause        = "I want to buy";

    instrumentsForTrading1["aaaaa"] = tradingInfo;

    operation.timestamp                       = 1704056400001;
    operation.instrumentId                    = "aaaaa";
    operation.instrumentLogo                  = &logo;
    operation.instrumentTicker                = "ABBA";
    operation.instrumentName                  = "Abstract Basics";
    operation.description                     = "Purchase of shares";
    operation.price                           = 200.0f;
    operation.avgPriceFifo                    = 200.0f;
    operation.avgPriceWavg                    = 200.0f;
    operation.quantity                        = 500;
    operation.remainedQuantity                = 500;
    operation.payment                         = -100000.0f;
    operation.avgCostFifo                     = 100000.0f;
    operation.costFifo.units                  = 100000;
    operation.costFifo.nano                   = 0;
    operation.costWavg.units                  = 100000;
    operation.costWavg.nano                   = 0;
    operation.commission                      = -40.0f;
    operation.yield                           = 0.0f;
    operation.yieldWithCommission             = -40.0f;
    operation.yieldWithCommissionPercent      = -0.04f;
    operation.inputMoney.units                = 1000000;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = 1000000;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission.units  = -39;
    operation.totalYieldWithCommission.nano   = -999998989;
    operation.totalYieldWithCommissionPercent = -0.004f;
    operation.remainedMoney.units             = 899960;
    operation.remainedMoney.nano              = 1010;
    operation.totalMoney.units                = 999960;
    operation.totalMoney.nano                 = 1010;
    operation.pricePrecision                  = 1;
    operation.paymentPrecision                = 1;
    operation.commissionPrecision             = 1;

    bestOperations.prepend(operation);

    LogEntry buyEntry1;
    LogEntry buyEntry2;
    LogEntry buyEntry3;
    LogEntry buyEntry4;

    buyEntry1.timestamp        = 1704056400002;
    buyEntry1.level            = LOG_LEVEL_DEBUG;
    buyEntry1.instrumentId     = "aaaaa";
    buyEntry1.instrumentLogo   = &logo;
    buyEntry1.instrumentTicker = "ABBA";
    buyEntry1.instrumentName   = "Abstract Basics";
    buyEntry1.message          = "I want to buy";

    buyEntry2.timestamp        = 1704056400003;
    buyEntry2.level            = LOG_LEVEL_VERBOSE;
    buyEntry2.instrumentId     = "aaaaa";
    buyEntry2.instrumentLogo   = &logo;
    buyEntry2.instrumentTicker = "ABBA";
    buyEntry2.instrumentName   = "Abstract Basics";
    buyEntry2.message          = "Order to buy 500 created with a price 200.0 \u20BD";

    buyEntry3.timestamp        = 1704056400004;
    buyEntry3.level            = LOG_LEVEL_VERBOSE;
    buyEntry3.instrumentId     = "aaaaa";
    buyEntry3.instrumentLogo   = &logo;
    buyEntry3.instrumentTicker = "ABBA";
    buyEntry3.instrumentName   = "Abstract Basics";
    buyEntry3.message          = "Order completed. 500 bought with a price 200.0 \u20BD";

    buyEntry4.timestamp        = 1704056400005;
    buyEntry4.level            = LOG_LEVEL_VERBOSE;
    buyEntry4.instrumentId     = "aaaaa";
    buyEntry4.instrumentLogo   = &logo;
    buyEntry4.instrumentTicker = "ABBA";
    buyEntry4.instrumentName   = "Abstract Basics";
    buyEntry4.message          = "Trade completed successfully";

    bestEntries.prepend(buyEntry1);
    bestEntries.prepend(buyEntry2);
    bestEntries.prepend(buyEntry3);
    bestEntries.prepend(buyEntry4);

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
    buyCategory1.part = 100.0;
    buyCategory1.items << buyItem1;

    buyCategory2.id   = 1;
    buyCategory2.name = "Share";
    buyCategory2.cost = 0.0;
    buyCategory2.part = 0.0;
    buyCategory2.items << buyItem2;

    buyPortfolio.positions << buyCategory1 << buyCategory2;

    InstrumentSells buyInstrumentSells;

    InstrumentsForTrading instrumentsForTrading2 = instrumentsForTrading1;

    tradingInfo.asap         = false;
    tradingInfo.avgPrice     = -1.0f;
    tradingInfo.price        = 200.0f;
    tradingInfo.expectedCost = 100000.0;
    tradingInfo.cause        = "I want to buy";

    instrumentsForTrading2["aaaaa"] = tradingInfo;

    tradingInfo.asap         = false;
    tradingInfo.avgPrice     = -1.0f;
    tradingInfo.price        = 200.0f;
    tradingInfo.expectedCost = 150000.0;
    tradingInfo.cause        = "I want to buy";

    instrumentsForTrading2["bbbbb"] = tradingInfo;

    operation.timestamp                       = 1704056460001;
    operation.instrumentId                    = "bbbbb";
    operation.instrumentLogo                  = &logo;
    operation.instrumentTicker                = "BASE";
    operation.instrumentName                  = "Basketball enhancement";
    operation.description                     = "Purchase of shares";
    operation.price                           = 200.0f;
    operation.avgPriceFifo                    = 200.0f;
    operation.avgPriceWavg                    = 200.0f;
    operation.quantity                        = 750;
    operation.remainedQuantity                = 750;
    operation.payment                         = -150000.0f;
    operation.avgCostFifo                     = 150000.0f;
    operation.costFifo.units                  = 150000;
    operation.costFifo.nano                   = 0;
    operation.costWavg.units                  = 150000;
    operation.costWavg.nano                   = 0;
    operation.commission                      = -60.0f;
    operation.yield                           = 0.0f;
    operation.yieldWithCommission             = -60.0f;
    operation.yieldWithCommissionPercent      = -0.04f;
    operation.inputMoney.units                = 1000000;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = 1000000;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission.units  = -99;
    operation.totalYieldWithCommission.nano   = -999997473;
    operation.totalYieldWithCommissionPercent = -0.01f;
    operation.remainedMoney.units             = 749900;
    operation.remainedMoney.nano              = 2526;
    operation.totalMoney.units                = 999900;
    operation.totalMoney.nano                 = 2526;
    operation.pricePrecision                  = 2;
    operation.paymentPrecision                = 2;
    operation.commissionPrecision             = 2;

    bestOperations.prepend(operation);

    buyEntry1.timestamp        = 1704056460002;
    buyEntry1.level            = LOG_LEVEL_DEBUG;
    buyEntry1.instrumentId     = "bbbbb";
    buyEntry1.instrumentLogo   = &logo;
    buyEntry1.instrumentTicker = "BASE";
    buyEntry1.instrumentName   = "Basketball enhancement";
    buyEntry1.message          = "I want to buy";

    buyEntry2.timestamp        = 1704056460003;
    buyEntry2.level            = LOG_LEVEL_VERBOSE;
    buyEntry2.instrumentId     = "bbbbb";
    buyEntry2.instrumentLogo   = &logo;
    buyEntry2.instrumentTicker = "BASE";
    buyEntry2.instrumentName   = "Basketball enhancement";
    buyEntry2.message          = "Order to buy 750 created with a price 200.00 \u20BD";

    buyEntry3.timestamp        = 1704056460004;
    buyEntry3.level            = LOG_LEVEL_VERBOSE;
    buyEntry3.instrumentId     = "bbbbb";
    buyEntry3.instrumentLogo   = &logo;
    buyEntry3.instrumentTicker = "BASE";
    buyEntry3.instrumentName   = "Basketball enhancement";
    buyEntry3.message          = "Order completed. 750 bought with a price 200.00 \u20BD";

    buyEntry4.timestamp        = 1704056460005;
    buyEntry4.level            = LOG_LEVEL_VERBOSE;
    buyEntry4.instrumentId     = "bbbbb";
    buyEntry4.instrumentLogo   = &logo;
    buyEntry4.instrumentTicker = "BASE";
    buyEntry4.instrumentName   = "Basketball enhancement";
    buyEntry4.message          = "Trade completed successfully";

    bestEntries.prepend(buyEntry1);
    bestEntries.prepend(buyEntry2);
    bestEntries.prepend(buyEntry3);
    bestEntries.prepend(buyEntry4);

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

    InstrumentSells buyInstrumentSells2;

    InstrumentsForTrading instrumentsForTrading3 = instrumentsForTrading2;

    tradingInfo.asap         = false;
    tradingInfo.avgPrice     = 250.0f;
    tradingInfo.price        = 300.0f;
    tradingInfo.expectedCost = 0.0;
    tradingInfo.cause        = "I want to sell";

    instrumentsForTrading3["bbbbb"] = tradingInfo;

    operation.timestamp                       = 1704056520001;
    operation.instrumentId                    = "bbbbb";
    operation.instrumentLogo                  = &logo;
    operation.instrumentTicker                = "BASE";
    operation.instrumentName                  = "Basketball enhancement";
    operation.description                     = "Sale of shares";
    operation.price                           = 300.0f;
    operation.avgPriceFifo                    = 200.0f;
    operation.avgPriceWavg                    = 200.0f;
    operation.quantity                        = 750;
    operation.remainedQuantity                = 0;
    operation.payment                         = 225000.0f;
    operation.avgCostFifo                     = 150000.0f;
    operation.costFifo.units                  = 0;
    operation.costFifo.nano                   = 0;
    operation.costWavg.units                  = 0;
    operation.costWavg.nano                   = 0;
    operation.commission                      = -90.0f;
    operation.yield                           = 75000.0f;
    operation.yieldWithCommission             = 74910.0f;
    operation.yieldWithCommissionPercent      = 49.94f;
    operation.inputMoney.units                = 1000000;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = 1000000;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission.units  = 74810;
    operation.totalYieldWithCommission.nano   = 4799;
    operation.totalYieldWithCommissionPercent = 7.481f;
    operation.remainedMoney.units             = 974810;
    operation.remainedMoney.nano              = 4799;
    operation.totalMoney.units                = 1074810;
    operation.totalMoney.nano                 = 4799;
    operation.pricePrecision                  = 2;
    operation.paymentPrecision                = 2;
    operation.commissionPrecision             = 2;

    bestOperations.prepend(operation);

    LogEntry sellEntry1;
    LogEntry sellEntry2;
    LogEntry sellEntry3;
    LogEntry sellEntry4;

    sellEntry1.timestamp        = 1704056520002;
    sellEntry1.level            = LOG_LEVEL_DEBUG;
    sellEntry1.instrumentId     = "bbbbb";
    sellEntry1.instrumentLogo   = &logo;
    sellEntry1.instrumentTicker = "BASE";
    sellEntry1.instrumentName   = "Basketball enhancement";
    sellEntry1.message          = "I want to sell";

    sellEntry2.timestamp        = 1704056520003;
    sellEntry2.level            = LOG_LEVEL_VERBOSE;
    sellEntry2.instrumentId     = "bbbbb";
    sellEntry2.instrumentLogo   = &logo;
    sellEntry2.instrumentTicker = "BASE";
    sellEntry2.instrumentName   = "Basketball enhancement";
    sellEntry2.message          = "Order to sell 750 created with a price 300.00 \u20BD";

    sellEntry3.timestamp        = 1704056520004;
    sellEntry3.level            = LOG_LEVEL_VERBOSE;
    sellEntry3.instrumentId     = "bbbbb";
    sellEntry3.instrumentLogo   = &logo;
    sellEntry3.instrumentTicker = "BASE";
    sellEntry3.instrumentName   = "Basketball enhancement";
    sellEntry3.message          = "Order completed. 750 sold with a price 300.00 \u20BD";

    sellEntry4.timestamp        = 1704056520005;
    sellEntry4.level            = LOG_LEVEL_VERBOSE;
    sellEntry4.instrumentId     = "bbbbb";
    sellEntry4.instrumentLogo   = &logo;
    sellEntry4.instrumentTicker = "BASE";
    sellEntry4.instrumentName   = "Basketball enhancement";
    sellEntry4.message          = "Trade completed successfully";

    bestEntries.prepend(sellEntry1);
    bestEntries.prepend(sellEntry2);
    bestEntries.prepend(sellEntry3);
    bestEntries.prepend(sellEntry4);

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

    InstrumentSells sellInstrumentSells;
    sellInstrumentSells["bbbbb"] = 1704056520006;

    InstrumentsForTrading instrumentsForTrading4 = instrumentsForTrading3;

    tradingInfo.asap         = false;
    tradingInfo.avgPrice     = 1990.0f;
    tradingInfo.price        = 2000.0f;
    tradingInfo.expectedCost = 0.0;
    tradingInfo.cause        = "I want to sell";

    instrumentsForTrading4["aaaaa"] = tradingInfo;

    operation.timestamp                       = 1704056580001;
    operation.instrumentId                    = "aaaaa";
    operation.instrumentLogo                  = &logo;
    operation.instrumentTicker                = "ABBA";
    operation.instrumentName                  = "Abstract Basics";
    operation.description                     = "Sale of shares";
    operation.price                           = 2000.0f;
    operation.avgPriceFifo                    = 200.0f;
    operation.avgPriceWavg                    = 200.0f;
    operation.quantity                        = 500;
    operation.remainedQuantity                = 0;
    operation.payment                         = 100000.0f;
    operation.avgCostFifo                     = 100000.0f;
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
    operation.totalYieldWithCommission.units  = 74810;
    operation.totalYieldWithCommission.nano   = 4799;
    operation.totalYieldWithCommissionPercent = 7.481f;
    operation.remainedMoney.units             = 1074810;
    operation.remainedMoney.nano              = 4799;
    operation.totalMoney.units                = 1074810;
    operation.totalMoney.nano                 = 4799;
    operation.pricePrecision                  = 1;
    operation.paymentPrecision                = 1;
    operation.commissionPrecision             = 1;

    bestOperations.prepend(operation);

    sellEntry1.timestamp        = 1704056580002;
    sellEntry1.level            = LOG_LEVEL_DEBUG;
    sellEntry1.instrumentId     = "aaaaa";
    sellEntry1.instrumentLogo   = &logo;
    sellEntry1.instrumentTicker = "ABBA";
    sellEntry1.instrumentName   = "Abstract Basics";
    sellEntry1.message          = "I want to sell";

    sellEntry2.timestamp        = 1704056580003;
    sellEntry2.level            = LOG_LEVEL_VERBOSE;
    sellEntry2.instrumentId     = "aaaaa";
    sellEntry2.instrumentLogo   = &logo;
    sellEntry2.instrumentTicker = "ABBA";
    sellEntry2.instrumentName   = "Abstract Basics";
    sellEntry2.message          = "Order to sell 500 created with a price 2000.0 \u20BD";

    sellEntry3.timestamp        = 1704056580004;
    sellEntry3.level            = LOG_LEVEL_VERBOSE;
    sellEntry3.instrumentId     = "aaaaa";
    sellEntry3.instrumentLogo   = &logo;
    sellEntry3.instrumentTicker = "ABBA";
    sellEntry3.instrumentName   = "Abstract Basics";
    sellEntry3.message          = "Order completed. 500 sold with a price 2000.0 \u20BD";

    sellEntry4.timestamp        = 1704056580005;
    sellEntry4.level            = LOG_LEVEL_VERBOSE;
    sellEntry4.instrumentId     = "aaaaa";
    sellEntry4.instrumentLogo   = &logo;
    sellEntry4.instrumentTicker = "ABBA";
    sellEntry4.instrumentName   = "Abstract Basics";
    sellEntry4.message          = "Trade completed successfully";

    bestEntries.prepend(sellEntry1);
    bestEntries.prepend(sellEntry2);
    bestEntries.prepend(sellEntry3);
    bestEntries.prepend(sellEntry4);

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

    InstrumentSells sellInstrumentSells2;
    sellInstrumentSells2["aaaaa"] = 1704056580006;
    sellInstrumentSells2["bbbbb"] = 1704056520006;

    InstrumentsForTrading emptyInstrumentsForTrading;

    QStringList bestConfigs = {
        R"({"b1":{"enabled":true},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"s1":{"enabled":true},"s2":{"enabled":false},"s3":{"enabled":false},"s4":{"enabled":false}})",
        R"({"b1":{"enabled":false},"b2":{"enabled":true},"b3":{"enabled":false},"b4":{"enabled":false},"s1":{"enabled":false},"s2":{"enabled":true},"s3":{"enabled":false},"s4":{"enabled":false}})",
        R"({"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":true},"b4":{"enabled":false},"s1":{"enabled":true},"s2":{"enabled":false},"s3":{"enabled":false},"s4":{"enabled":false}})",
        R"({"b1":{"enabled":false},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":true},"s1":{"enabled":false},"s2":{"enabled":true},"s3":{"enabled":false},"s4":{"enabled":false}})"
    };

    QString bestConfigsExtended = "[{}]";

    EXPECT_CALL(*settingsEditorMock, value(QString("Options/StartMoney"), QVariant(0))).WillOnce(Return(QVariant(1000000)));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/FromDate"), QVariant(""))).WillOnce(Return(QVariant("2024-01-01")));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/ToDate"), QVariant(""))).WillOnce(Return(QVariant("2024-01-02")));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/BestConfig"), QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(*configMock, setSimulatorConfigCommon(true));
    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(false));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(&simulatorConfigMock));
    EXPECT_CALL(simulatorConfigMock, variantsToJsonStringList()).WillOnce(Return(configVariants));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/AmountOfBuyDecisions"), QVariant(AMOUNT_OF_BUY_DECISIONS)));
    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock1)));
    EXPECT_CALL(*dirMock1, mkpath(appDir + "/data/simulator")).WillOnce(Return(true));

    for (int i = 0; i < AMOUNT_OF_BUY_DECISIONS; ++i)
    {
        StrictMock<FileMock>* fileMock = configFileMocksForWriting.at(i);

        EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/data/simulator/configs%2.json").arg(appDir, QString::number(i))))
            .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));
        EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
        EXPECT_CALL(*fileMock, write(configVariants.at(i).toUtf8())).WillOnce(Return(configVariants.at(i).size()));
        EXPECT_CALL(*fileMock, close());
        EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/LastConfigId%1").arg(i), QVariant(0)));
    }

    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/LastConfigId"), QVariant(0)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/Step"), QVariant(0)));
    EXPECT_CALL(*operationsDatabaseMock, deleteOperations(-1));
    EXPECT_CALL(*logsDatabaseMock, deleteLogs(-1));
    EXPECT_CALL(*portfolioDatabaseMock, deletePortfolio(-1));

    for (int i = 0; i < AMOUNT_OF_BUY_DECISIONS; ++i)
    {
        EXPECT_CALL(*operationsDatabaseMock, deleteOperations(i));
        EXPECT_CALL(*logsDatabaseMock, deleteLogs(i));
        EXPECT_CALL(*portfolioDatabaseMock, deletePortfolio(i));
    }

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/Step"), QVariant(0))).WillOnce(Return(QVariant(0)));
    EXPECT_CALL(*operationsDatabaseMock, readOperations(0)).WillOnce(Return(bestLocalOperations));
    EXPECT_CALL(*logsDatabaseMock, readLogs(0)).WillOnce(Return(bestLocalEntries));
    EXPECT_CALL(*portfolioDatabaseMock, readPortfolio(0)).WillOnce(Return(bestLocalPortfolio));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/LastConfigId0"), QVariant(0))).WillOnce(Return(QVariant(0)));
    EXPECT_CALL(*configMock, clone()).WillOnce(Return(&clonedConfigMock));
    EXPECT_CALL(clonedConfigMock, getSimulatorConfig()).WillOnce(Return(&clonedSimulatorConfigMock));
    EXPECT_CALL(clonedSimulatorConfigMock, fromJsonObject(_));

    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(), 1704056400000, &clonedConfigMock, instrumentSells, portfolio, stocks, false, 0, true, false
        )
    )
        .WillOnce(Return(instrumentsForTrading1));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(),
            1704056460000,
            &clonedConfigMock,
            buyInstrumentSells,
            buyPortfolio,
            stocks,
            false,
            0,
            true,
            false
        )
    )
        .WillOnce(Return(instrumentsForTrading2));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("bbbbb"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(),
            1704056520000,
            &clonedConfigMock,
            buyInstrumentSells2,
            buyPortfolio2,
            stocks,
            false,
            0,
            true,
            false
        )
    )
        .WillOnce(Return(instrumentsForTrading3));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("bbbbb"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(),
            1704056580000,
            &clonedConfigMock,
            sellInstrumentSells,
            sellPortfolio,
            stocks,
            false,
            0,
            true,
            false
        )
    )
        .WillOnce(Return(instrumentsForTrading4));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(),
            Ge(1704056640000),
            &clonedConfigMock,
            sellInstrumentSells2,
            sellPortfolio2,
            stocks,
            false,
            0,
            true,
            false
        )
    )
        .Times(24 * 60 - 4) // Amount of minutes in a day and skip previous steps
        .WillRepeatedly(Return(emptyInstrumentsForTrading));

    EXPECT_CALL(*operationsDatabaseMock, writeOperations(bestOperations, 0));
    EXPECT_CALL(*logsDatabaseMock, writeLogs(bestEntries, 0));
    EXPECT_CALL(*portfolioDatabaseMock, writePortfolio(sellPortfolio2, 0));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/BestConfigId0"), QVariant(0)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/LastConfigId0"), QVariant(1)));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/BestConfigId0"), QVariant(0))).WillOnce(Return(QVariant(0)));
    EXPECT_CALL(clonedConfigMock, deleteRecursively());

    for (int i = 1; i < AMOUNT_OF_BUY_DECISIONS; ++i)
    {
        EXPECT_CALL(*operationsDatabaseMock, readOperations(i)).WillOnce(Return(bestLocalOperations));
        EXPECT_CALL(*logsDatabaseMock, readLogs(i)).WillOnce(Return(bestLocalEntries));
        EXPECT_CALL(*portfolioDatabaseMock, readPortfolio(i)).WillOnce(Return(bestLocalPortfolio));
        EXPECT_CALL(*settingsEditorMock, value(QString("Options/LastConfigId%1").arg(i), QVariant(0)))
            .WillOnce(Return(QVariant(1)));
        EXPECT_CALL(*configMock, clone()).WillOnce(Return(&clonedConfigMock));
        EXPECT_CALL(*settingsEditorMock, value(QString("Options/BestConfigId%1").arg(i), QVariant(0)))
            .WillOnce(Return(QVariant(0)));
        EXPECT_CALL(clonedConfigMock, deleteRecursively());
    }

    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(&simulatorConfigMock));
    EXPECT_CALL(simulatorConfigMock, variantsToJsonStringListExtendedBySellDecisions(bestConfigs))
        .WillOnce(Return(bestConfigsExtended));
    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/configs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(totalConfigFileWriteMock)));
    EXPECT_CALL(*totalConfigFileWriteMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*totalConfigFileWriteMock, write(bestConfigsExtended.toUtf8())).WillOnce(Return(bestConfigsExtended.size()));
    EXPECT_CALL(*totalConfigFileWriteMock, close());
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/Step"), QVariant(AMOUNT_OF_BUY_DECISIONS)));

    for (int i = 0; i < AMOUNT_OF_BUY_DECISIONS; ++i)
    {
        EXPECT_CALL(*operationsDatabaseMock, readOperations(i)).WillOnce(Return(bestLocalOperations));
    }

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/configs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(totalConfigFileReadMock1)));
    EXPECT_CALL(*totalConfigFileReadMock1, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*totalConfigFileReadMock1, readAll()).WillOnce(Return(bestConfigsExtended.toUtf8()));
    EXPECT_CALL(*totalConfigFileReadMock1, close());
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/LastConfigId"), QVariant(0))).WillOnce(Return(QVariant(1)));
    EXPECT_CALL(*configMock, clone()).WillOnce(Return(&clonedConfigMock));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/BestConfigId"), QVariant(0))).WillOnce(Return(QVariant(0)));
    EXPECT_CALL(clonedConfigMock, deleteRecursively());
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(&simulatorConfigMock));
    EXPECT_CALL(simulatorConfigMock, fromJsonObject(_));

    thread->run();

    EXPECT_CALL(*settingsEditorMock, value(QString("Options/StartMoney"), QVariant(0))).WillOnce(Return(QVariant(1000000)));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/FromDate"), QVariant(""))).WillOnce(Return(QVariant("2024-01-01")));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/ToDate"), QVariant(""))).WillOnce(Return(QVariant("2024-01-02")));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/BestConfig"), QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(*configMock, setSimulatorConfigCommon(true));
    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(false));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*operationsDatabaseMock, readOperations(-1)).WillOnce(Return(bestOperations));
    EXPECT_CALL(*logsDatabaseMock, readLogs(-1)).WillOnce(Return(bestEntries));
    EXPECT_CALL(*portfolioDatabaseMock, readPortfolio(-1)).WillOnce(Return(buyPortfolio2));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/AmountOfBuyDecisions"), QVariant(0)))
        .WillOnce(Return(QVariant(AMOUNT_OF_BUY_DECISIONS)));

    for (int i = 0; i < AMOUNT_OF_BUY_DECISIONS; ++i)
    {
        StrictMock<FileMock>* fileMock = configFileMocksForReading.at(i);

        EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/data/simulator/configs%2.json").arg(appDir, QString::number(i))))
            .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));
        EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
        EXPECT_CALL(*fileMock, readAll()).WillOnce(Return(configVariants.at(i).toUtf8()));
        EXPECT_CALL(*fileMock, close());
    }

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/Step"), QVariant(0)))
        .WillOnce(Return(QVariant(AMOUNT_OF_BUY_DECISIONS)));

    for (int i = 0; i < AMOUNT_OF_BUY_DECISIONS; ++i)
    {
        EXPECT_CALL(*operationsDatabaseMock, readOperations(i)).WillOnce(Return(bestLocalOperations));
    }

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/configs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(totalConfigFileReadMock2)));
    EXPECT_CALL(*totalConfigFileReadMock2, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(false));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());

    thread->run();

    thread->reset();

    EXPECT_CALL(*settingsEditorMock, value(QString("Options/StartMoney"), QVariant(0))).WillOnce(Return(QVariant(1000000)));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/FromDate"), QVariant(""))).WillOnce(Return(QVariant("2024-01-01")));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/ToDate"), QVariant(""))).WillOnce(Return(QVariant("2024-01-02")));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/BestConfig"), QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(&simulatorConfigMock));
    EXPECT_CALL(simulatorConfigMock, toJsonString()).WillOnce(Return(configVariant));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/AmountOfBuyDecisions"), QVariant(1)));
    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock2)));
    EXPECT_CALL(*dirMock2, mkpath(appDir + "/data/simulator")).WillOnce(Return(true));
    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/data/simulator/configs0.json").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(configFileWriteMock)));
    EXPECT_CALL(*configFileWriteMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*configFileWriteMock, write(QString("[\n" + configVariant + "\n]").toUtf8()))
        .WillOnce(Return(configVariant.size() + 4));
    EXPECT_CALL(*configFileWriteMock, close());
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/LastConfigId0"), QVariant(0)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/LastConfigId"), QVariant(0)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/Step"), QVariant(0)));
    EXPECT_CALL(*operationsDatabaseMock, deleteOperations(-1));
    EXPECT_CALL(*logsDatabaseMock, deleteLogs(-1));
    EXPECT_CALL(*portfolioDatabaseMock, deletePortfolio(-1));
    EXPECT_CALL(*operationsDatabaseMock, deleteOperations(0));
    EXPECT_CALL(*logsDatabaseMock, deleteLogs(0));
    EXPECT_CALL(*portfolioDatabaseMock, deletePortfolio(0));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/LastConfigId"), QVariant(0))).WillOnce(Return(QVariant(0)));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());

    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(), 1704056400000, configMock, instrumentSells, portfolio, stocks, false, 0, true, true
        )
    )
        .WillOnce(Return(instrumentsForTrading1));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(), 1704056460000, configMock, buyInstrumentSells, buyPortfolio, stocks, false, 0, true, true
        )
    )
        .WillOnce(Return(instrumentsForTrading2));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("bbbbb"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(), 1704056520000, configMock, buyInstrumentSells2, buyPortfolio2, stocks, false, 0, true, true
        )
    )
        .WillOnce(Return(instrumentsForTrading3));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("bbbbb"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(), 1704056580000, configMock, sellInstrumentSells, sellPortfolio, stocks, false, 0, true, true
        )
    )
        .WillOnce(Return(instrumentsForTrading4));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    EXPECT_CALL(
        *decisionMakerMock,
        makeDecision(
            QThread::currentThread(),
            Ge(1704056640000),
            configMock,
            sellInstrumentSells2,
            sellPortfolio2,
            stocks,
            false,
            0,
            true,
            true
        )
    )
        .Times(24 * 60 - 4) // Amount of minutes in a day and skip previous steps
        .WillRepeatedly(Return(emptyInstrumentsForTrading));

    EXPECT_CALL(*operationsDatabaseMock, writeOperations(bestOperations, -1));
    EXPECT_CALL(*logsDatabaseMock, writeLogs(bestEntries, -1));
    EXPECT_CALL(*portfolioDatabaseMock, writePortfolio(sellPortfolio2, -1));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/LastConfigId"), QVariant(1)));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());

    thread->run();
}

TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_terminateThread)
{
    thread->terminateThread();
}

TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_notifyResult)
{
    thread->notifyResult(1000000, 10000000);
    thread->notifyResult(1000000, 1000000);
    thread->notifyResult(1000000, 100000);
}

TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_notifyBestResult)
{
    thread->notifyBestResult(1000000, 10000000);
    thread->notifyBestResult(1000000, 1000000);
    thread->notifyBestResult(1000000, 100000);
}

TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_splitConfigVariants)
{
    thread->splitConfigVariants("{Bad content ::::: 555");
}

TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_applyToConfig)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(&simulatorConfigMock));

    thread->applyToConfig(configMock, "{Bad content ::::: 555");
}

TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_optimizeOperations_and_optimizeLogs)
{
    const InSequence seq;

    StrictMock<FileMock>* configFileReadMock = new StrictMock<FileMock>(); // Will be deleted in loadConfigs function

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
        operation1.costFifo.units                  = 0;
        operation1.costFifo.nano                   = 0;
        operation1.costWavg.units                  = 0;
        operation1.costWavg.nano                   = 0;
        operation1.commission                      = -1.4f;
        operation1.yield                           = 267.0f;
        operation1.yieldWithCommission             = 265.6f;
        operation1.yieldWithCommissionPercent      = 10.4856f;
        operation1.inputMoney.units                = 200000;
        operation1.inputMoney.nano                 = 0;
        operation1.maxInputMoney.units             = 200000;
        operation1.maxInputMoney.nano              = 0;
        operation1.totalYieldWithCommission.units  = 265;
        operation1.totalYieldWithCommission.nano   = -666500000;
        operation1.totalYieldWithCommissionPercent = 0.1321667f;
        operation1.remainedMoney.units             = 200300;
        operation1.remainedMoney.nano              = 0;
        operation1.totalMoney.units                = 200300;
        operation1.totalMoney.nano                 = 0;
        operation1.pricePrecision                  = 2;
        operation1.paymentPrecision                = 2;
        operation1.commissionPrecision             = 2;

        operation2.timestamp                       = operations.size() - i - 1;
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
        operation2.costFifo.units                  = 2533;
        operation2.costFifo.nano                   = 0;
        operation2.costWavg.units                  = 2533;
        operation2.costWavg.nano                   = 0;
        operation2.commission                      = -1.2665f;
        operation2.yield                           = 0.0f;
        operation2.yieldWithCommission             = -1.2665f;
        operation2.yieldWithCommissionPercent      = -0.05f;
        operation2.inputMoney.units                = 200000;
        operation2.inputMoney.nano                 = 0;
        operation2.maxInputMoney.units             = 200000;
        operation2.maxInputMoney.nano              = 0;
        operation2.totalYieldWithCommission.units  = -1;
        operation2.totalYieldWithCommission.nano   = -266500000;
        operation2.totalYieldWithCommissionPercent = -0.0006332f;
        operation2.remainedMoney.units             = 197466;
        operation2.remainedMoney.nano              = -266500000;
        operation2.totalMoney.units                = 199999;
        operation2.totalMoney.nano                 = -266500000;
        operation2.pricePrecision                  = 2;
        operation2.paymentPrecision                = 2;
        operation2.commissionPrecision             = 4;
    }

    for (int i = 0; i < optimizedOperations.size() - 1; i += 2)
    {
        Operation& operation1 = optimizedOperations[i];
        Operation& operation2 = optimizedOperations[i + 1];

        operation1.timestamp                       = operations.size() - i;
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
        operation1.costFifo.units                  = 0;
        operation1.costFifo.nano                   = 0;
        operation1.costWavg.units                  = 0;
        operation1.costWavg.nano                   = 0;
        operation1.commission                      = -1.4f;
        operation1.yield                           = 267.0f;
        operation1.yieldWithCommission             = 265.6f;
        operation1.yieldWithCommissionPercent      = 10.4856f;
        operation1.inputMoney.units                = 200000;
        operation1.inputMoney.nano                 = 0;
        operation1.maxInputMoney.units             = 200000;
        operation1.maxInputMoney.nano              = 0;
        operation1.totalYieldWithCommission.units  = 265;
        operation1.totalYieldWithCommission.nano   = -666500000;
        operation1.totalYieldWithCommissionPercent = 0.1321667f;
        operation1.remainedMoney.units             = 200300;
        operation1.remainedMoney.nano              = 0;
        operation1.totalMoney.units                = 200300;
        operation1.totalMoney.nano                 = 0;
        operation1.pricePrecision                  = 2;
        operation1.paymentPrecision                = 2;
        operation1.commissionPrecision             = 2;

        operation2.timestamp                       = operations.size() - i - 1;
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
        operation2.costFifo.units                  = 2533;
        operation2.costFifo.nano                   = 0;
        operation2.costWavg.units                  = 2533;
        operation2.costWavg.nano                   = 0;
        operation2.commission                      = -1.2665f;
        operation2.yield                           = 0.0f;
        operation2.yieldWithCommission             = -1.2665f;
        operation2.yieldWithCommissionPercent      = -0.05f;
        operation2.inputMoney.units                = 200000;
        operation2.inputMoney.nano                 = 0;
        operation2.maxInputMoney.units             = 200000;
        operation2.maxInputMoney.nano              = 0;
        operation2.totalYieldWithCommission.units  = -1;
        operation2.totalYieldWithCommission.nano   = -266500000;
        operation2.totalYieldWithCommissionPercent = -0.0006332f;
        operation2.remainedMoney.units             = 197466;
        operation2.remainedMoney.nano              = -266500000;
        operation2.totalMoney.units                = 199999;
        operation2.totalMoney.nano                 = -266500000;
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

    Instruments instruments;

    QString configVariant = {
        R"([{"b1":{"enabled":true},"b2":{"enabled":false},"b3":{"enabled":false},"b4":{"enabled":false},"s1":{"enabled":true},"s2":{"enabled":false},"s3":{"enabled":false},"s4":{"enabled":false}}])"
    };

    QList<Stock*> stocks;

    EXPECT_CALL(*settingsEditorMock, value(QString("Options/StartMoney"), QVariant(0))).WillOnce(Return(QVariant(1000000)));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/FromDate"), QVariant(""))).WillOnce(Return(QVariant("2024-01-01")));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/ToDate"), QVariant(""))).WillOnce(Return(QVariant("2024-01-02")));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/BestConfig"), QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*operationsDatabaseMock, readOperations(-1)).WillOnce(Return(operations));
    EXPECT_CALL(*logsDatabaseMock, readLogs(-1)).WillOnce(Return(entries));
    EXPECT_CALL(*portfolioDatabaseMock, readPortfolio(-1)).WillOnce(Return(portfolio));
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/AmountOfBuyDecisions"), QVariant(0))).WillOnce(Return(QVariant(1)));
    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/data/simulator/configs0.json").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(configFileReadMock)));
    EXPECT_CALL(*configFileReadMock, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*configFileReadMock, readAll()).WillOnce(Return(configVariant.toUtf8()));
    EXPECT_CALL(*configFileReadMock, close());
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/LastConfigId"), QVariant(0))).WillOnce(Return(QVariant(1)));
    EXPECT_CALL(*optimizerMock, optimizeOperations(operations, 5, QStringList())).WillOnce(Return(optimizedOperations));
    EXPECT_CALL(*operationsDatabaseMock, writeOperations(optimizedOperations, -1));
    EXPECT_CALL(*optimizerMock, optimizeLogs(entries, 5)).WillOnce(Return(optimizedEntries));
    EXPECT_CALL(*logsDatabaseMock, writeLogs(optimizedEntries, -1));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());

    thread->run();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
