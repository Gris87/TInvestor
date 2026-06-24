#include "src/db/notifications/notificationsdatabase.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_NotificationsDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        appDir = qApp->applicationDirPath();

        dirFactoryMock  = new StrictMock<DirFactoryMock>();
        fileFactoryMock = new StrictMock<FileFactoryMock>();

        database = new NotificationsDatabase(dirFactoryMock, fileFactoryMock);
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
        delete fileFactoryMock;
    }

    NotificationsDatabase*       database;
    StrictMock<DirFactoryMock>*  dirFactoryMock;
    StrictMock<FileFactoryMock>* fileFactoryMock;
    QString                      appDir;
};



TEST_F(Test_NotificationsDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_NotificationsDatabase, Test_readNotifications)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in readNotifications function
    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in readNotifications function
    StrictMock<FileMock>* fileMock3 = new StrictMock<FileMock>(); // Will be deleted in readNotifications function
    StrictMock<FileMock>* fileMock4 = new StrictMock<FileMock>(); // Will be deleted in readNotifications function

    QList<NotificationInfo> notifications;

    QString notificationsStr =
        "{\"data\":\"bbbbb\",\"requestTimestamp\":1000,\"text\":\"aaaaa\",\"timestamp\":1000,\"type\":\"system\"},\n{\"data\":"
        "\"bbbbb\",\"requestTimestamp\":2000,\"text\":\"aaaaa\",\"timestamp\":2000,\"type\":\"system\"},\n{\"data\":\"bbbbb\","
        "\"requestTimestamp\":3000,\"text\":\"aaaaa\",\"timestamp\":3000,\"type\":\"system\"}";
    QByteArray notificationsBytes = notificationsStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/notifications/notifications.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock1)));
    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, readAll()).WillOnce(Return(notificationsBytes));
    EXPECT_CALL(*fileMock1, close());

    notifications = database->readNotifications();

    ASSERT_EQ(notifications.size(), 3);

    for (int i = 0; i < notifications.size(); ++i)
    {
        // clang-format off
        ASSERT_EQ(notifications.at(i).requestTimestamp, 3000 - i * 1000);
        ASSERT_EQ(notifications.at(i).timestamp,        3000 - i * 1000);
        ASSERT_EQ(notifications.at(i).messageType,      MESSAGE_TYPE_SYSTEM);
        ASSERT_EQ(notifications.at(i).text,             "aaaaa");
        ASSERT_EQ(notifications.at(i).data,             "bbbbb");
        // clang-format on
    }

    QString largeNotificationsStr;

    for (int i = 0; i < 1000; ++i)
    {
        if (i > 0)
        {
            largeNotificationsStr += ",\n";
        }

        largeNotificationsStr +=
            QString("{\"data\":\"bbbbb\",\"requestTimestamp\":%1,\"text\":\"aaaaa\",\"timestamp\":%1,\"type\":\"system\"}")
                .arg(i * 1000);
    }

    QByteArray largeNotificationsBytes = largeNotificationsStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/notifications/notifications.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock2)));
    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, readAll()).WillOnce(Return(largeNotificationsBytes));
    EXPECT_CALL(*fileMock2, close());

    notifications = database->readNotifications();

    ASSERT_EQ(notifications.size(), 1000);

    for (int i = 0; i < notifications.size(); ++i)
    {
        // clang-format off
        ASSERT_EQ(notifications.at(i).requestTimestamp, 999000 - i * 1000);
        ASSERT_EQ(notifications.at(i).timestamp,        999000 - i * 1000);
        ASSERT_EQ(notifications.at(i).messageType,      MESSAGE_TYPE_SYSTEM);
        ASSERT_EQ(notifications.at(i).text,             "aaaaa");
        ASSERT_EQ(notifications.at(i).data,             "bbbbb");
        // clang-format on
    }

    notificationsStr   += ",\n{Bad content ::::: 555";
    notificationsBytes  = notificationsStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/notifications/notifications.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock3)));
    EXPECT_CALL(*fileMock3, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock3, readAll()).WillOnce(Return(notificationsBytes));
    EXPECT_CALL(*fileMock3, close());

    notifications = database->readNotifications();

    ASSERT_EQ(notifications.size(), 0);

    largeNotificationsStr   += ",\n{Bad content ::::: 555";
    largeNotificationsBytes  = largeNotificationsStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/notifications/notifications.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock4)));
    EXPECT_CALL(*fileMock4, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock4, readAll()).WillOnce(Return(largeNotificationsBytes));
    EXPECT_CALL(*fileMock4, close());

    notifications = database->readNotifications();

    ASSERT_EQ(notifications.size(), 1001);

    for (int i = 1; i < notifications.size(); ++i)
    {
        // clang-format off
        ASSERT_EQ(notifications.at(i).requestTimestamp, 1000000 - i * 1000);
        ASSERT_EQ(notifications.at(i).timestamp,        1000000 - i * 1000);
        ASSERT_EQ(notifications.at(i).messageType,      MESSAGE_TYPE_SYSTEM);
        ASSERT_EQ(notifications.at(i).text,             "aaaaa");
        ASSERT_EQ(notifications.at(i).data,             "bbbbb");
        // clang-format on
    }
}

TEST_F(Test_NotificationsDatabase, Test_writeNotifications)
{
    const InSequence seq;

    StrictMock<DirMock>*  dirMock  = new StrictMock<DirMock>();  // Will be deleted in writeNotifications function
    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in writeNotifications function

    QList<NotificationInfo> notifications;
    NotificationInfo        notification;

    notification.requestTimestamp = 3000;
    notification.timestamp        = 3000;
    notification.messageType      = MESSAGE_TYPE_SYSTEM;
    notification.text             = "aaaaa";
    notification.data             = "bbbbb";

    notifications << notification;

    notification.requestTimestamp = 2000;
    notification.timestamp        = 2000;
    notifications << notification;

    notification.requestTimestamp = 1000;
    notification.timestamp        = 1000;
    notifications << notification;

    const QString notificationsStr =
        "{\"data\":\"bbbbb\",\"requestTimestamp\":1000,\"text\":\"aaaaa\",\"timestamp\":1000,\"type\":\"system\"},\n{\"data\":"
        "\"bbbbb\",\"requestTimestamp\":2000,\"text\":\"aaaaa\",\"timestamp\":2000,\"type\":\"system\"},\n{\"data\":\"bbbbb\","
        "\"requestTimestamp\":3000,\"text\":\"aaaaa\",\"timestamp\":3000,\"type\":\"system\"}";
    const QByteArray notificationsBytes = notificationsStr.toUtf8();

    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
    EXPECT_CALL(*dirMock, mkpath(appDir + "/data/notifications")).WillOnce(Return(true));
    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/notifications/notifications.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));
    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, write(notificationsBytes)).WillOnce(Return(notificationsBytes.size()));
    EXPECT_CALL(*fileMock, close());

    database->writeNotifications(notifications);
}

TEST_F(Test_NotificationsDatabase, Test_appendNotifications)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in appendNotifications function

    QList<NotificationInfo> notifications;
    NotificationInfo        notification;

    notification.requestTimestamp = 6000;
    notification.timestamp        = 6000;
    notification.messageType      = MESSAGE_TYPE_SYSTEM;
    notification.text             = "aaaaa";
    notification.data             = "bbbbb";

    notifications << notification;

    notification.requestTimestamp = 5000;
    notification.timestamp        = 5000;
    notifications << notification;

    notification.requestTimestamp = 4000;
    notification.timestamp        = 4000;
    notifications << notification;

    const QString notification1Str =
        "{\"data\":\"bbbbb\",\"requestTimestamp\":4000,\"text\":\"aaaaa\",\"timestamp\":4000,\"type\":\"system\"}";
    const QString notification2Str =
        "{\"data\":\"bbbbb\",\"requestTimestamp\":5000,\"text\":\"aaaaa\",\"timestamp\":5000,\"type\":\"system\"}";
    const QString notification3Str =
        "{\"data\":\"bbbbb\",\"requestTimestamp\":6000,\"text\":\"aaaaa\",\"timestamp\":6000,\"type\":\"system\"}";
    const QByteArray notification1Bytes = notification1Str.toUtf8();
    const QByteArray notification2Bytes = notification2Str.toUtf8();
    const QByteArray notification3Bytes = notification3Str.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/notifications/notifications.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));
    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::Append))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock, write(notification1Bytes)).WillOnce(Return(notification1Bytes.size()));
    EXPECT_CALL(*fileMock, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock, write(notification2Bytes)).WillOnce(Return(notification2Bytes.size()));
    EXPECT_CALL(*fileMock, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock, write(notification3Bytes)).WillOnce(Return(notification3Bytes.size()));
    EXPECT_CALL(*fileMock, close());

    database->appendNotifications(notifications);
}
