#include "src/threads/logs/logsthread.h"

#include <gtest/gtest.h>

#include "src/db/logs/ilogsdatabase_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
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

TEST_F(Test_LogsThread, Test_run_and_addLog_and_terminateThread)
{
    // const InSequence seq;

    QList<LogEntry> entries;
    Instruments     instruments;
    Logo            logo;

    EXPECT_CALL(*logsDatabaseMock, readLogs()).WillOnce(Return(entries));

    thread->start();

    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*logsDatabaseMock, appendLog(_));

    thread->addLog(LOG_LEVEL_WARNING, "aaaaa", "Sell just a million items");

    thread->terminateThread();
    thread->wait();
}

TEST_F(Test_LogsThread, Test_setAccountId)
{
    const InSequence seq;

    EXPECT_CALL(*logsDatabaseMock, setAccount(QString("Vaseline service")));

    thread->setAccountId("Vaseline service", "aaaaaa");
}

TEST_F(Test_LogsThread, Test_getTimestamp)
{
    ASSERT_EQ(thread->getTimestamp(1000), 1000);
    ASSERT_EQ(thread->getTimestamp(1000), 1001);
    ASSERT_EQ(thread->getTimestamp(1000), 1002);
    ASSERT_EQ(thread->getTimestamp(1200), 1200);
    ASSERT_EQ(thread->getTimestamp(1200), 1201);
}

TEST_F(Test_LogsThread, Test_optimize)
{
    const InSequence seq;

    QList<LogEntry> entries;
    QList<LogEntry> optimizedEntries;

    entries.resizeForOverwrite(11);
    optimizedEntries.resizeForOverwrite(5);

    thread->testSetLimitLogs(entries.size() - 1);
    thread->testSetOptimizeSize(optimizedEntries.size());

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

    EXPECT_CALL(*logsDatabaseMock, readLogs()).WillOnce(Return(entries));
    EXPECT_CALL(*logsDatabaseMock, readLogs()).WillOnce(Return(entries));
    EXPECT_CALL(*logsDatabaseMock, writeLogs(optimizedEntries));

    thread->testTerminateWithoutTerminate();
    thread->run();
}
