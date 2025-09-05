#include "src/threads/bidirtradingcontrol/bidirtradingcontrolthread.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_BiDirTradingControlThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        thread = new BiDirTradingControlThread();
    }

    void TearDown()
    {
        delete thread;
    }

    BiDirTradingControlThread* thread;
};



TEST_F(Test_BiDirTradingControlThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirTradingControlThread, Test_run)
{
}

TEST_F(Test_BiDirTradingControlThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
