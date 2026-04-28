#include "src/domain/notification/notificationinfo.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
class Test_NotificationInfo : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_NotificationInfo, Test_constructor_and_destructor)
{
    const NotificationInfo info;

    // clang-format off
    ASSERT_EQ(info.timestamp,   0);
    ASSERT_EQ(info.messageType, MESSAGE_TYPE_NONE);
    ASSERT_EQ(info.text,        "");
    // clang-format on

    const NotificationInfo info2(1, MESSAGE_TYPE_PORTFOLIO, "a");

    // clang-format off
    ASSERT_EQ(info2.timestamp,   1);
    ASSERT_EQ(info2.messageType, MESSAGE_TYPE_PORTFOLIO);
    ASSERT_EQ(info2.text,        "a");
    // clang-format on
}

TEST_F(Test_NotificationInfo, Test_copy_constructor)
{
    NotificationInfo info;

    info.timestamp   = 1;
    info.messageType = MESSAGE_TYPE_PORTFOLIO;
    info.text        = "a";

    const NotificationInfo info2(info);

    // clang-format off
    ASSERT_EQ(info2.timestamp,   1);
    ASSERT_EQ(info2.messageType, MESSAGE_TYPE_PORTFOLIO);
    ASSERT_EQ(info2.text,        "a");
    // clang-format on
}

TEST_F(Test_NotificationInfo, Test_assign)
{
    NotificationInfo info;
    NotificationInfo info2;

    info.timestamp   = 1;
    info.messageType = MESSAGE_TYPE_PORTFOLIO;
    info.text        = "a";

    info2 = info;

    // clang-format off
    ASSERT_EQ(info2.timestamp,   1);
    ASSERT_EQ(info2.messageType, MESSAGE_TYPE_PORTFOLIO);
    ASSERT_EQ(info2.text,        "a");
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
