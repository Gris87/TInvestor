#include "src/threads/logs/logsthread.h"

#include <gtest/gtest.h>

#include "src/db/logs/ilogsdatabase_mock.h"



using ::testing::StrictMock;



class Test_LogsThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        logsDatabaseMock = new StrictMock<LogsDatabaseMock>();

        thread = new LogsThread(logsDatabaseMock);
    }

    void TearDown() override
    {
        delete thread;
        delete logsDatabaseMock;
    }

    LogsThread*                   thread;
    StrictMock<LogsDatabaseMock>* logsDatabaseMock;
};



TEST_F(Test_LogsThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogsThread, Test_run)
{
    // thread->run();
}
