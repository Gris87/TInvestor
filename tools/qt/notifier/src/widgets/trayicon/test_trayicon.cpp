#include "src/widgets/trayicon/trayicon.h"

#include <QMenu>
#include <gtest/gtest.h>



class Test_TrayIcon : public ::testing::Test
{
protected:
    void SetUp() override
    {
        trayIcon = new TrayIcon();
    }

    void TearDown() override
    {
        delete trayIcon;
    }

    TrayIcon* trayIcon;
};



// NOLINTBEGIN(readability-function-cognitive-complexity)
TEST_F(Test_TrayIcon, Test_constructor_and_destructor)
{
    QMenu* trayIconMenu = trayIcon->contextMenu();
    ASSERT_NE(trayIconMenu, nullptr);

    const QList<QAction*> actions = trayIconMenu->actions();
    ASSERT_EQ(actions.size(), 3);

    ASSERT_EQ(actions.at(0)->isSeparator(), false);
    ASSERT_EQ(actions.at(1)->isSeparator(), true);
    ASSERT_EQ(actions.at(2)->isSeparator(), false);

    ASSERT_EQ(actions.at(0)->text(), "Show");
    ASSERT_EQ(actions.at(2)->text(), "Exit");

    ASSERT_EQ(trayIconMenu->defaultAction(), actions.at(0));

    ASSERT_EQ(trayIcon->toolTip(), "TInvestor notifier");
}

TEST_F(Test_TrayIcon, Test_setFilter)
{
    const Filter filter;

    trayIcon->setFilter(filter);
}

TEST_F(Test_TrayIcon, Test_handleConnection_and_handleDisconnection)
{
    trayIcon->handleConnection();
    trayIcon->handleDisconnection();
}

TEST_F(Test_TrayIcon, Test_notificationsAdded_and_resetCounter)
{
    QList<NotificationInfo> notifications;
    NotificationInfo        notification;

    notification.requestTimestamp = 1704056400000;
    notification.timestamp        = 1704056400000;
    notification.messageType      = MESSAGE_TYPE_SYSTEM;
    notification.text             = "aaaaa";
    notification.data             = "bbbbb";

    notifications << notification;

    trayIcon->notificationsAdded(notifications);

    trayIcon->resetCounter();
}
// NOLINTEND(readability-function-cognitive-complexity)
