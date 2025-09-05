#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <gtest/gtest.h>



class Test_BiDirTradingThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        thread = new BiDirTradingThread();
    }

    void TearDown() override
    {
        delete thread;
    }

    BiDirTradingThread* thread;
};



TEST_F(Test_BiDirTradingThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirTradingThread, Test_run)
{
}

TEST_F(Test_BiDirTradingThread, Test_terminateThread)
{
    thread->terminateThread();
}
