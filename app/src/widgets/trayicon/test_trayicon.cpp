#include "src/widgets/trayicon/trayicon.h"

#include <QMenu>
#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
TEST(Test_TrayIcon, Test_constructor_and_destructor)
{
    const TrayIcon trayIcon;

    QMenu* trayIconMenu = trayIcon.contextMenu();
    ASSERT_TRUE(trayIconMenu != nullptr);

    const QList<QAction*> actions = trayIconMenu->actions();
    ASSERT_EQ(actions.size(), 3);

    ASSERT_EQ(actions.at(0)->isSeparator(), false);
    ASSERT_EQ(actions.at(1)->isSeparator(), true);
    ASSERT_EQ(actions.at(2)->isSeparator(), false);

    ASSERT_EQ(actions.at(0)->text(), "Show");
    ASSERT_EQ(actions.at(2)->text(), "Exit");

    ASSERT_EQ(trayIconMenu->defaultAction(), actions.at(0));

    ASSERT_EQ(trayIcon.toolTip(), "TInvestor");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
