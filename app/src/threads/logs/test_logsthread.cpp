#include "src/threads/logs/logsthread.h"

#include <gtest/gtest.h>

#include "src/db/logs/ilogsdatabase_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"



using ::testing::StrictMock;



class Test_LogsThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        logsDatabaseMock       = new StrictMock<LogsDatabaseMock>();
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        logosStorageMock       = new StrictMock<LogosStorageMock>();

        thread = new LogsThread(logsDatabaseMock, instrumentsStorageMock, logosStorageMock);
    }

    void TearDown() override
    {
        delete thread;
        delete logsDatabaseMock;
        delete instrumentsStorageMock;
        delete logosStorageMock;
    }

    LogsThread*                         thread;
    StrictMock<LogsDatabaseMock>*       logsDatabaseMock;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<LogosStorageMock>*       logosStorageMock;
};



TEST_F(Test_LogsThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogsThread, Test_run)
{
    // thread->run();
}
