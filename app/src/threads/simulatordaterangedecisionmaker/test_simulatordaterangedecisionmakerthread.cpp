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
    StrictMock<DirMock>*                dirMock   = new StrictMock<DirMock>();  // Will be deleted in initConfigs function
    StrictMock<FileMock>*               fileMock1 = new StrictMock<FileMock>(); // Will be deleted in initConfigs function
    StrictMock<FileMock>*               fileMock2 = new StrictMock<FileMock>(); // Will be deleted in initConfigs function
    StrictMock<FileMock>*               fileMock3 = new StrictMock<FileMock>(); // Will be deleted in initConfigs function
    StrictMock<FileMock>*               fileMock4 = new StrictMock<FileMock>(); // Will be deleted in initConfigs function

    QList<StrictMock<FileMock>*> configFileMocksForWriting = {fileMock1, fileMock2, fileMock3, fileMock4};

    thread->reset();

    Instruments instruments;
    Instrument  instrument1;
    Instrument  instrument2;
    Instrument  instrument3;

    instrument1.ticker         = "RUBLE";
    instrument1.name           = "Ruble";
    instrument1.lot            = 1;
    instrument1.pricePrecision = 2;

    instrument2.ticker         = "ABBA";
    instrument2.name           = "Abstract Basics";
    instrument2.lot            = 5;
    instrument2.pricePrecision = 1;

    instrument3.ticker         = "BASE";
    instrument3.name           = "Basketball enhancement";
    instrument3.lot            = 10;
    instrument3.pricePrecision = 2;

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
    EXPECT_CALL(*settingsEditorMock, setValue(QString("Options/AmountOfBuyDecisions"), QVariant(4)));
    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
    EXPECT_CALL(*dirMock, mkpath(appDir + "/data/simulator")).WillOnce(Return(true));

    for (int i = 0; i < configFileMocksForWriting.size(); ++i)
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

    for (int i = 0; i < configFileMocksForWriting.size(); ++i)
    {
        EXPECT_CALL(*operationsDatabaseMock, deleteOperations(i));
        EXPECT_CALL(*logsDatabaseMock, deleteLogs(i));
        EXPECT_CALL(*portfolioDatabaseMock, deletePortfolio(i));
    }

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*settingsEditorMock, value(QString("Options/Step"), QVariant(0))).WillOnce(Return(QVariant(1000))); // TODO: Use 0

    thread->run();
}

TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
