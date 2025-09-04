#include "src/threads/highliquidity/highliquiditythread.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_HighLiquidityThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        thread = new HighLiquidityThread();
    }

    void TearDown() override
    {
        delete thread;
    }

    HighLiquidityThread* thread;
};



TEST_F(Test_HighLiquidityThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_HighLiquidityThread, Test_run)
{
}

TEST_F(Test_HighLiquidityThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
