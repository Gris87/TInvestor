#include "src/widgets/notificationstablewidget/notificationstablewidget.h"
#include "ui_notificationstablewidget.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodel_mock.h"
#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodelfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_NotificationsTableWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // const InSequence seq;

        notificationsTableModelFactoryMock = new StrictMock<NotificationsTableModelFactoryMock>();
        settingsEditorMock                 = new StrictMock<SettingsEditorMock>();

        notificationsTableModelMock = new StrictMock<NotificationsTableModelMock>();

        EXPECT_CALL(*notificationsTableModelFactoryMock, newInstance(NotNull())).WillOnce(Return(notificationsTableModelMock));
        EXPECT_CALL(*notificationsTableModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
        EXPECT_CALL(*notificationsTableModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(NOTIFICATIONS_COLUMN_COUNT));

        notificationsTableWidget = new NotificationsTableWidget(notificationsTableModelFactoryMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete notificationsTableWidget;
        delete notificationsTableModelFactoryMock;
        delete settingsEditorMock;
        delete notificationsTableModelMock;
    }

    NotificationsTableWidget*                       notificationsTableWidget;
    StrictMock<NotificationsTableModelFactoryMock>* notificationsTableModelFactoryMock;
    StrictMock<SettingsEditorMock>*                 settingsEditorMock;
    StrictMock<NotificationsTableModelMock>*        notificationsTableModelMock;
};



TEST_F(Test_NotificationsTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_NotificationsTableWidget, Test_setFilter)
{
    const InSequence seq;

    const Filter filter;

    EXPECT_CALL(*notificationsTableModelMock, setFilter(filter));

    notificationsTableWidget->setFilter(filter);
}

TEST_F(Test_NotificationsTableWidget, Test_notificationsRead)
{
    const InSequence seq;

    const QList<NotificationInfo> notifications;

    EXPECT_CALL(*notificationsTableModelMock, notificationsRead(notifications));

    notificationsTableWidget->notificationsRead(notifications);

    // clang-format off
    ASSERT_EQ(notificationsTableWidget->ui->tableView->horizontalHeader()->sortIndicatorSection(), NOTIFICATIONS_TIME_COLUMN);
    ASSERT_EQ(notificationsTableWidget->ui->tableView->horizontalHeader()->sortIndicatorOrder(),   Qt::DescendingOrder);
    // clang-format on
}

TEST_F(Test_NotificationsTableWidget, Test_notificationsAdded)
{
    const InSequence seq;

    const QList<NotificationInfo> notifications;

    EXPECT_CALL(*notificationsTableModelMock, notificationsAdded(notifications));

    notificationsTableWidget->notificationsAdded(notifications);
}

TEST_F(Test_NotificationsTableWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Time"), _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Type"), _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Text"), _));
    // clang-format on

    notificationsTableWidget->saveWindowState("AAAAA");
}

TEST_F(Test_NotificationsTableWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Time"), _)).WillOnce(Return(QVariant(110)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Type"), _)).WillOnce(Return(QVariant(103)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Text"), _)).WillOnce(Return(QVariant(900)));
    // clang-format on

    notificationsTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
