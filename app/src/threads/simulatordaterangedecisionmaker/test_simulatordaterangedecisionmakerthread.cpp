#include "src/threads/simulatordaterangedecisionmaker/simulatordaterangedecisionmakerthread.h"

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
#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_SimulatorDateRangeDecisionMakerThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
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
            decisionMakerMock
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
};



TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_run)
{
}

TEST_F(Test_SimulatorDateRangeDecisionMakerThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
