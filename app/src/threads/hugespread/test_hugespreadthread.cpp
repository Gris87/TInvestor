#include "src/threads/hugespread/hugespreadthread.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_HugeSpreadThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        thread = new HugeSpreadThread();
    }

    void TearDown() override
    {
        delete thread;
    }

    HugeSpreadThread* thread;
};



TEST_F(Test_HugeSpreadThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_HugeSpreadThread, Test_run)
{
}

TEST_F(Test_HugeSpreadThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
