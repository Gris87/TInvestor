#include "src/storage/notifications/notificationsstorage.h"

#include <gtest/gtest.h>

#include "src/db/notifications/inotificationsdatabase_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Ne;
using ::testing::Return;
using ::testing::StrictMock;



class Test_NotificationsStorage : public ::testing::Test
{
protected:
    void SetUp() override
    {
        notificationsDatabaseMock = new StrictMock<NotificationsDatabaseMock>();

        storage = new NotificationsStorage(notificationsDatabaseMock);
    }

    void TearDown() override
    {
        delete storage;
        delete notificationsDatabaseMock;
    }

    NotificationsStorage*                  storage;
    StrictMock<NotificationsDatabaseMock>* notificationsDatabaseMock;
};



TEST_F(Test_NotificationsStorage, Test_constructor_and_destructor)
{
}

TEST_F(Test_NotificationsStorage, Test_readFromDatabase_and_getNotifications)
{
    const InSequence seq;

    QList<NotificationInfo> notifications = storage->getNotifications();
    ASSERT_EQ(notifications.size(), 0);

    NotificationInfo notification1;
    NotificationInfo notification2;
    NotificationInfo notification3;

    notification1.timestamp   = 1000;
    notification1.messageType = MESSAGE_TYPE_SYSTEM;
    notification1.text        = "aaaaa";

    notification2.timestamp   = 2000;
    notification2.messageType = MESSAGE_TYPE_PORTFOLIO;
    notification2.text        = "bbbbb";

    notification3.timestamp   = 3000;
    notification3.messageType = MESSAGE_TYPE_DIVIDENDS;
    notification3.text        = "ccccc";

    notifications << notification1 << notification2 << notification3;

    EXPECT_CALL(*notificationsDatabaseMock, readNotifications()).WillOnce(Return(notifications));

    storage->readFromDatabase();
    notifications = storage->getNotifications();

    // clang-format off
    ASSERT_EQ(notifications.size(),            3);
    ASSERT_EQ(notifications.at(0).timestamp,   1000);
    ASSERT_EQ(notifications.at(0).messageType, MESSAGE_TYPE_SYSTEM);
    ASSERT_EQ(notifications.at(0).text,        "aaaaa");
    ASSERT_EQ(notifications.at(1).timestamp,   2000);
    ASSERT_EQ(notifications.at(1).messageType, MESSAGE_TYPE_PORTFOLIO);
    ASSERT_EQ(notifications.at(1).text,        "bbbbb");
    ASSERT_EQ(notifications.at(2).timestamp,   3000);
    ASSERT_EQ(notifications.at(2).messageType, MESSAGE_TYPE_DIVIDENDS);
    ASSERT_EQ(notifications.at(2).text,        "ccccc");
    // clang-format on
}

TEST_F(Test_NotificationsStorage, Test_lock_and_unlock)
{
    storage->writeLock();
    storage->writeUnlock();
    storage->readLock();
    storage->readUnlock();
}
