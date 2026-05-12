#include "src/widgets/tablemodels/notificationstablemodel/notificationstablemodel.h"

#include <QBrush>
#include <gtest/gtest.h>



class Test_NotificationsTableModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        model = new NotificationsTableModel();
    }

    void TearDown() override
    {
        delete model;
    }

    NotificationsTableModel* model;
};



TEST_F(Test_NotificationsTableModel, Test_constructor_and_destructor)
{
}

TEST_F(Test_NotificationsTableModel, Test_rowCount)
{
    ASSERT_EQ(model->rowCount(), 0);

    QList<NotificationInfo> entries;

    entries.resize(3);

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    entries.resize(5);

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 5);

    entries.clear();

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 0);
}

TEST_F(Test_NotificationsTableModel, Test_columnCount)
{
    ASSERT_EQ(model->columnCount(), NOTIFICATIONS_COLUMN_COUNT);
}

TEST_F(Test_NotificationsTableModel, Test_headerData)
{
    // clang-format off
    ASSERT_EQ(model->headerData(NOTIFICATIONS_TIME_COLUMN, Qt::Horizontal, Qt::DisplayRole),           QVariant("Time"));
    ASSERT_EQ(model->headerData(NOTIFICATIONS_TYPE_COLUMN, Qt::Horizontal, Qt::DisplayRole),           QVariant("Type"));
    ASSERT_EQ(model->headerData(NOTIFICATIONS_TEXT_COLUMN, Qt::Horizontal, Qt::DisplayRole),           QVariant("Text"));
    ASSERT_EQ(model->headerData(0,                         Qt::Vertical,   Qt::DisplayRole),           QVariant(1));
    ASSERT_EQ(model->headerData(1,                         Qt::Vertical,   Qt::DisplayRole),           QVariant(2));
    ASSERT_EQ(model->headerData(2,                         Qt::Vertical,   Qt::DisplayRole),           QVariant(3));
    ASSERT_EQ(model->headerData(0,                         Qt::Horizontal, Qt::WhatsThisPropertyRole), QVariant());
    // clang-format on
}

TEST_F(Test_NotificationsTableModel, Test_data)
{
    QList<NotificationInfo> entries;

    NotificationInfo entry1;
    NotificationInfo entry2;
    NotificationInfo entry3;

    entry1.requestTimestamp = QDateTime::currentMSecsSinceEpoch() + 100000;
    entry1.timestamp        = 1704056520000;
    entry1.messageType      = MESSAGE_TYPE_SYSTEM;
    entry1.text             = "A basket full";

    entry2.requestTimestamp = QDateTime::currentMSecsSinceEpoch() + 100000;
    entry2.timestamp        = 1704056460000;
    entry2.messageType      = MESSAGE_TYPE_PORTFOLIO;
    entry2.text             = "Ball shall levitate";

    entry3.requestTimestamp = 1704056400000;
    entry3.timestamp        = 1704056400000;
    entry3.messageType      = MESSAGE_TYPE_DIVIDENDS;
    entry3.text             = "Can you eat fish?";

    entries << entry1 << entry2 << entry3;

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole),           QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole),           QVariant("System"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole),           QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole),           QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole),           QVariant("Portfolio"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole),           QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole),           QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole),           QVariant("Dividends"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole),           QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::BackgroundRole),        QVariant(QBrush(QColor("#918A00"))));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::BackgroundRole),        QVariant(QBrush(QColor("#918A00"))));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::BackgroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, 0),                         Qt::WhatsThisPropertyRole), QVariant());
    // clang-format on
}

TEST_F(Test_NotificationsTableModel, Test_sort)
{
    QList<NotificationInfo> entries;

    NotificationInfo entry1;
    NotificationInfo entry2;
    NotificationInfo entry3;

    entry1.requestTimestamp = QDateTime::currentMSecsSinceEpoch() + 100000;
    entry1.timestamp        = 1704056520000;
    entry1.messageType      = MESSAGE_TYPE_SYSTEM;
    entry1.text             = "A basket full";

    entry2.requestTimestamp = QDateTime::currentMSecsSinceEpoch() + 100000;
    entry2.timestamp        = 1704056460000;
    entry2.messageType      = MESSAGE_TYPE_PORTFOLIO;
    entry2.text             = "Ball shall levitate";

    entry3.requestTimestamp = 1704056400000;
    entry3.timestamp        = 1704056400000;
    entry3.messageType      = MESSAGE_TYPE_DIVIDENDS;
    entry3.text             = "Can you eat fish?";

    entries << entry1 << entry2 << entry3;

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Portfolio"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    // clang-format on

    model->sort(NOTIFICATIONS_TIME_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));

    model->sort(NOTIFICATIONS_TIME_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));

    model->sort(NOTIFICATIONS_TYPE_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Portfolio"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));

    model->sort(NOTIFICATIONS_TYPE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Portfolio"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));

    model->sort(NOTIFICATIONS_TEXT_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));

    model->sort(NOTIFICATIONS_TEXT_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));

    model->sort(NOTIFICATIONS_TIME_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));

    model->sort(NOTIFICATIONS_TYPE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Portfolio"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));

    model->sort(NOTIFICATIONS_TEXT_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));

    model->sort(NOTIFICATIONS_TIME_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
}

TEST_F(Test_NotificationsTableModel, Test_setFilter)
{
    QList<NotificationInfo> entries;

    NotificationInfo entry1;
    NotificationInfo entry2;
    NotificationInfo entry3;

    entry1.requestTimestamp = QDateTime::currentMSecsSinceEpoch() + 100000;
    entry1.timestamp        = 1704056520000;
    entry1.messageType      = MESSAGE_TYPE_SYSTEM;
    entry1.text             = "A basket full";

    entry2.requestTimestamp = QDateTime::currentMSecsSinceEpoch() + 100000;
    entry2.timestamp        = 1704056460000;
    entry2.messageType      = MESSAGE_TYPE_PORTFOLIO;
    entry2.text             = "Ball shall levitate";

    entry3.requestTimestamp = 1704056400000;
    entry3.timestamp        = 1704056400000;
    entry3.messageType      = MESSAGE_TYPE_DIVIDENDS;
    entry3.text             = "Can you eat fish?";

    entries << entry1 << entry2 << entry3;

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Portfolio"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    // clang-format on

    Filter filter;
    filter.setMessageTypeMask(QList<MessageType>() << MESSAGE_TYPE_SYSTEM << MESSAGE_TYPE_DIVIDENDS);

    model->setFilter(filter);
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    // clang-format on
}

TEST_F(Test_NotificationsTableModel, Test_notificationsAdded)
{
    QList<NotificationInfo> entries;

    NotificationInfo entry1;
    NotificationInfo entry2;
    NotificationInfo entry3;

    entry1.requestTimestamp = QDateTime::currentMSecsSinceEpoch() + 100000;
    entry1.timestamp        = 1704056520000;
    entry1.messageType      = MESSAGE_TYPE_SYSTEM;
    entry1.text             = "A basket full";

    entry2.requestTimestamp = QDateTime::currentMSecsSinceEpoch() + 100000;
    entry2.timestamp        = 1704056460000;
    entry2.messageType      = MESSAGE_TYPE_PORTFOLIO;
    entry2.text             = "Ball shall levitate";

    entry3.requestTimestamp = 1704056400000;
    entry3.timestamp        = 1704056400000;
    entry3.messageType      = MESSAGE_TYPE_DIVIDENDS;
    entry3.text             = "Can you eat fish?";

    entries << entry1 << entry2 << entry3;

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Portfolio"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    // clang-format on

    NotificationInfo newEntry;

    newEntry.requestTimestamp = 1704056580000;
    newEntry.timestamp        = 1704056580000;
    newEntry.messageType      = MESSAGE_TYPE_HUGE_SELL;
    newEntry.text             = "Do you like pizdyuli?";

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 4);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:03:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(3, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    model->sort(NOTIFICATIONS_TIME_COLUMN, Qt::AscendingOrder);

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 4);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(3, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:03:00"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    model->sort(NOTIFICATIONS_TYPE_COLUMN, Qt::AscendingOrder);

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 4);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Portfolio"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Huge sell"));
    ASSERT_EQ(model->data(model->index(3, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    model->sort(NOTIFICATIONS_TYPE_COLUMN, Qt::DescendingOrder);

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 4);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Huge sell"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Portfolio"));
    ASSERT_EQ(model->data(model->index(3, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    model->sort(NOTIFICATIONS_TEXT_COLUMN, Qt::AscendingOrder);

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 4);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(3, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Do you like pizdyuli?"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    model->sort(NOTIFICATIONS_TEXT_COLUMN, Qt::DescendingOrder);

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 4);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Do you like pizdyuli?"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(3, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    Filter filter;
    filter.setMessageTypeMask(QList<MessageType>() << MESSAGE_TYPE_SYSTEM << MESSAGE_TYPE_HUGE_SELL << MESSAGE_TYPE_DIVIDENDS);

    model->setFilter(filter);
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));
    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    // clang-format on

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 3);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:03:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 2);

    model->sort(NOTIFICATIONS_TIME_COLUMN, Qt::AscendingOrder);

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 3);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:03:00"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 2);

    model->sort(NOTIFICATIONS_TYPE_COLUMN, Qt::AscendingOrder);

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 3);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Huge sell"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 2);

    model->sort(NOTIFICATIONS_TYPE_COLUMN, Qt::DescendingOrder);

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 3);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Dividends"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("Huge sell"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TYPE_COLUMN), Qt::DisplayRole), QVariant("System"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 2);

    model->sort(NOTIFICATIONS_TEXT_COLUMN, Qt::AscendingOrder);

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 3);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Do you like pizdyuli?"));

    model->notificationsRead(entries);
    ASSERT_EQ(model->rowCount(), 2);

    model->sort(NOTIFICATIONS_TEXT_COLUMN, Qt::DescendingOrder);

    model->notificationsAdded(QList<NotificationInfo>() << newEntry);
    ASSERT_EQ(model->rowCount(), 3);

    ASSERT_EQ(model->data(model->index(0, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Do you like pizdyuli?"));
    ASSERT_EQ(model->data(model->index(1, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(2, NOTIFICATIONS_TEXT_COLUMN), Qt::DisplayRole), QVariant("A basket full"));
}

TEST_F(Test_NotificationsTableModel, Test_refreshBackground)
{
    model->refreshBackground();
}
