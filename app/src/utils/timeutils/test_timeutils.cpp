#include "src/utils/timeutils/timeutils.h"

#include <gtest/gtest.h>



class Test_TimeUtils : public ::testing::Test
{
protected:
    void SetUp() override
    {
        timeUtils = new TimeUtils();
    }

    void TearDown() override
    {
        delete timeUtils;
    }

    TimeUtils* timeUtils;
};



TEST_F(Test_TimeUtils, Test_constructor_and_destructor)
{
}

TEST_F(Test_TimeUtils, Test_interruptibleSleep)
{
    ASSERT_EQ(timeUtils->interruptibleSleep(100, QThread::currentThread()), false);
}
