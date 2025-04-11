#include "src/utils/timeutils/timeutils.h"

#include <gtest/gtest.h>



TEST(Test_TimeUtils, Test_constructor_and_destructor)
{
    const TimeUtils timeUtils;
}

TEST(Test_TimeUtils, Test_interruptibleSleep)
{
    TimeUtils timeUtils;

    ASSERT_EQ(timeUtils.interruptibleSleep(100, QThread::currentThread()), false);
}
