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

    ASSERT_EQ(filter.messageTypeMask, 255);
}

TEST_F(Test_Filter, Test_copy_constructor)
{
    Filter filter;

    filter.messageTypeMask = 1;

    const Filter filter2(filter);

    ASSERT_EQ(filter2.messageTypeMask, 1);
}

TEST_F(Test_Filter, Test_assign)
{
    Filter filter;
    Filter filter2;

    filter.messageTypeMask = 1;

    filter2 = filter;

    ASSERT_EQ(filter2.messageTypeMask, 1);
}

TEST_F(Test_Filter, Test_setMessageTypeMask)
{
    Filter filter;

    ASSERT_EQ(filter.messageTypeMask, 255);

    filter.setMessageTypeMask(QList<MessageType>());

    ASSERT_EQ(filter.messageTypeMask, 1);

    filter.setMessageTypeMask(QList<MessageType>() << MESSAGE_TYPE_SYSTEM);

    ASSERT_EQ(filter.messageTypeMask, 3);

    filter.setMessageTypeMask(QList<MessageType>() << MESSAGE_TYPE_SYSTEM << MESSAGE_TYPE_DIVIDENDS);

    ASSERT_EQ(filter.messageTypeMask, 19);
}

TEST_F(Test_Filter, Test_isActive)
{
    Filter filter;

    ASSERT_EQ(filter.isActive(), false);

    filter.messageTypeMask = 1;
    ASSERT_EQ(filter.isActive(), true);
    filter.messageTypeMask = 255;
    ASSERT_EQ(filter.isActive(), false);
}

TEST_F(Test_Filter, Test_isFiltered)
{
    Filter           filter;
    NotificationInfo notification;

    filter.messageTypeMask = 5;

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

    filter.messageTypeMask = 1;

    filter2.messageTypeMask = 1;

    ASSERT_EQ(filter, filter2);

    filter.messageTypeMask = 2;
    ASSERT_NE(filter, filter2);
    filter.messageTypeMask = 1;
    ASSERT_EQ(filter, filter2);
}
