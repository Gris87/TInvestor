#include "src/domain/filter/filter.h"

#include <gtest/gtest.h>



class Test_Filter : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_Filter, Test_constructor_and_destructor)
{
    const Filter filter;

    ASSERT_EQ(filter.messageTypeMask, 255ULL);
}

TEST_F(Test_Filter, Test_copy_constructor)
{
    Filter filter;

    filter.messageTypeMask = 1ULL;

    const Filter filter2(filter);

    ASSERT_EQ(filter2.messageTypeMask, 1ULL);
}

TEST_F(Test_Filter, Test_assign)
{
    Filter filter;
    Filter filter2;

    filter.messageTypeMask = 1ULL;

    filter2 = filter;

    ASSERT_EQ(filter2.messageTypeMask, 1ULL);
}

TEST_F(Test_Filter, Test_setMessageTypeMask)
{
    Filter filter;

    ASSERT_EQ(filter.messageTypeMask, 255ULL);

    filter.setMessageTypeMask(QList<MessageType>());

    ASSERT_EQ(filter.messageTypeMask, 1ULL);

    filter.setMessageTypeMask(QList<MessageType>() << MESSAGE_TYPE_SYSTEM);

    ASSERT_EQ(filter.messageTypeMask, 3ULL);

    filter.setMessageTypeMask(QList<MessageType>() << MESSAGE_TYPE_SYSTEM << MESSAGE_TYPE_DIVIDENDS);

    ASSERT_EQ(filter.messageTypeMask, 19ULL);
}

TEST_F(Test_Filter, Test_isActive)
{
    Filter filter;

    ASSERT_EQ(filter.isActive(), false);

    filter.messageTypeMask = 1ULL;
    ASSERT_EQ(filter.isActive(), true);
    filter.messageTypeMask = 255ULL;
    ASSERT_EQ(filter.isActive(), false);
}

TEST_F(Test_Filter, Test_isFiltered)
{
    Filter           filter;
    NotificationInfo notification;

    filter.messageTypeMask = 5ULL;

    notification.requestTimestamp = 1;
    notification.timestamp        = 2;
    notification.messageType      = MESSAGE_TYPE_PORTFOLIO;
    notification.text             = "a";

    ASSERT_EQ(filter.isFiltered(notification), true);

    notification.messageType = MESSAGE_TYPE_SYSTEM;

    ASSERT_EQ(filter.isFiltered(notification), false);
}

TEST_F(Test_Filter, Test_equals)
{
    Filter filter;
    Filter filter2;

    filter.messageTypeMask = 1ULL;

    filter2.messageTypeMask = 1ULL;

    ASSERT_EQ(filter, filter2);

    filter.messageTypeMask = 2ULL;
    ASSERT_NE(filter, filter2);
    filter.messageTypeMask = 1ULL;
    ASSERT_EQ(filter, filter2);
}
