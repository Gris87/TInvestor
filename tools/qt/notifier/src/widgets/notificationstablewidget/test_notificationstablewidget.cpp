#include "src/widgets/notificationstablewidget/notificationstablewidget.h"
#include "ui_notificationstablewidget.h"

#include <QCoreApplication>
#include <QDir>
#include <QMouseEvent>
#include <gtest/gtest.h>

#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablemodels/modelroles.h"
#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodel_mock.h"
#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodelfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Ne;
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
        httpClientMock                     = new StrictMock<HttpClientMock>();
        settingsEditorMock                 = new StrictMock<SettingsEditorMock>();

        notificationsTableModelMock = new StrictMock<NotificationsTableModelMock>();

        EXPECT_CALL(*notificationsTableModelFactoryMock, newInstance(NotNull())).WillOnce(Return(notificationsTableModelMock));
        EXPECT_CALL(*notificationsTableModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(1));
        EXPECT_CALL(*notificationsTableModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(NOTIFICATIONS_COLUMN_COUNT));

        notificationsTableWidget =
            new NotificationsTableWidget(notificationsTableModelFactoryMock, httpClientMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete notificationsTableWidget;
        delete notificationsTableModelFactoryMock;
        delete httpClientMock;
        delete settingsEditorMock;
        delete notificationsTableModelMock;
    }

    NotificationsTableWidget*                       notificationsTableWidget;
    StrictMock<NotificationsTableModelFactoryMock>* notificationsTableModelFactoryMock;
    StrictMock<HttpClientMock>*                     httpClientMock;
    StrictMock<SettingsEditorMock>*                 settingsEditorMock;
    StrictMock<NotificationsTableModelMock>*        notificationsTableModelMock;
};



TEST_F(Test_NotificationsTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_NotificationsTableWidget, Test_eventFilter)
{
    const InSequence seq;

    QModelIndex index = notificationsTableModelMock->index(0, NOTIFICATIONS_TEXT_COLUMN);

    EXPECT_CALL(*notificationsTableModelMock, data(_, Ne(ROLE_URL))).WillRepeatedly(Return(QVariant()));
    EXPECT_CALL(*notificationsTableModelMock, data(index, ROLE_URL)).WillOnce(Return(QVariant("")));

    QMouseEvent mouseEvent(
        QEvent::MouseButtonRelease,
        notificationsTableWidget->ui->tableView->visualRect(index).center(),
        QPointF(0, 0),
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::NoModifier
    );

    ASSERT_EQ(notificationsTableWidget->eventFilter(notificationsTableWidget->ui->tableView->viewport(), &mouseEvent), false);

    EXPECT_CALL(*notificationsTableModelMock, data(index, ROLE_URL)).WillOnce(Return(QVariant("Some url")));
    EXPECT_CALL(*httpClientMock, openInBrowser(QUrl("Some url"))).WillOnce(Return(true));

    ASSERT_EQ(notificationsTableWidget->eventFilter(notificationsTableWidget->ui->tableView->viewport(), &mouseEvent), false);
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

TEST_F(Test_NotificationsTableWidget, Test_refreshBackground)
{
    const InSequence seq;

    EXPECT_CALL(*notificationsTableModelMock, refreshBackground());

    notificationsTableWidget->refreshBackground();
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
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Type"), _)).WillOnce(Return(QVariant(128)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Text"), _)).WillOnce(Return(QVariant(1590)));
    // clang-format on

    notificationsTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
