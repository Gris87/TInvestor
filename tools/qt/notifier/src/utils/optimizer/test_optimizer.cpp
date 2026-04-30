#include "src/utils/optimizer/optimizer.h"

#include <gtest/gtest.h>



class Test_Optimizer : public ::testing::Test
{
protected:
    void SetUp() override
    {
        optimizer = new Optimizer();
    }

    void TearDown() override
    {
        delete optimizer;
    }

    Optimizer* optimizer;
};



TEST_F(Test_Optimizer, Test_constructor_and_destructor)
{
}

TEST_F(Test_Optimizer, Test_optimizeNotifications)
{
    QList<NotificationInfo> notifications;
    QList<NotificationInfo> optimizedNotifications;

    notifications.resizeForOverwrite(11);
    optimizedNotifications.resizeForOverwrite(5);

    for (int i = 0; i < notifications.size(); ++i)
    {
        NotificationInfo& info = notifications[i];

        info.timestamp   = notifications.size() - i;
        info.messageType = MESSAGE_TYPE_HUGE_SELL;
        info.text        = "aaaaa";
    }

    for (int i = 0; i < optimizedNotifications.size(); ++i)
    {
        NotificationInfo& info = optimizedNotifications[i];

        info.timestamp   = notifications.size() - i;
        info.messageType = MESSAGE_TYPE_HUGE_SELL;
        info.text        = "aaaaa";
    }

    QList<NotificationInfo> result = optimizer->optimizeNotifications(notifications, 5);

    ASSERT_EQ(result, optimizedNotifications);
}
