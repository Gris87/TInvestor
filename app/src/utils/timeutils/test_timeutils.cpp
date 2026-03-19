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

TEST_F(Test_TimeUtils, Test_isWorkingHours)
{
    ASSERT_EQ(timeUtils->isWorkingHours(1704056400000), false); // 00:00 MSK
    ASSERT_EQ(timeUtils->isWorkingHours(1704092340000), false); // 09:59 MSK
    ASSERT_EQ(timeUtils->isWorkingHours(1704092400000), true);  // 10:00 MSK
    ASSERT_EQ(timeUtils->isWorkingHours(1704123540000), true);  // 18:39 MSK
    ASSERT_EQ(timeUtils->isWorkingHours(1704123600000), false); // 18:40 MSK
    ASSERT_EQ(timeUtils->isWorkingHours(1704123660000), false); // 18:41 MSK
    ASSERT_EQ(timeUtils->isWorkingHours(1704542400000), false); // 15:00 MSK Sat
    ASSERT_EQ(timeUtils->isWorkingHours(1704628800000), false); // 15:00 MSK Sun
}

TEST_F(Test_TimeUtils, Test_isNormalOrEveningSession)
{
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704056400000), false); // 00:00 MSK
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704092340000), false); // 09:59 MSK
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704092400000), true);  // 10:00 MSK
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704123540000), true);  // 18:39 MSK
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704123600000), true);  // 18:40 MSK
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704123660000), true);  // 18:41 MSK
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704139140000), true);  // 22:59 MSK
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704139200000), false); // 23:00 MSK
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704139260000), false); // 23:01 MSK
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704542400000), true);  // 15:00 MSK Sat
    ASSERT_EQ(timeUtils->isNormalOrEveningSession(1704628800000), true);  // 15:00 MSK Sun
}

TEST_F(Test_TimeUtils, Test_isTimeBetween)
{
    ASSERT_EQ(timeUtils->isTimeBetween(QTime(10, 0), QTime(9, 0), QTime(11, 0)), true);
    ASSERT_EQ(timeUtils->isTimeBetween(QTime(8, 0), QTime(9, 0), QTime(11, 0)), false);
    ASSERT_EQ(timeUtils->isTimeBetween(QTime(10, 0), QTime(11, 0), QTime(9, 0)), false);
    ASSERT_EQ(timeUtils->isTimeBetween(QTime(8, 0), QTime(11, 0), QTime(9, 0)), true);
}
