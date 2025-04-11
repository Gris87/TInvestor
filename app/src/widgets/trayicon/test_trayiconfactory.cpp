#include "src/widgets/trayicon/trayiconfactory.h"

#include <gtest/gtest.h>



TEST(Test_TrayIconFactory, Test_constructor_and_destructor)
{
    const TrayIconFactory factory;
}

TEST(Test_TrayIconFactory, Test_newInstance)
{
    const TrayIconFactory factory;

    const ITrayIcon* trayIcon = factory.newInstance(nullptr);
    ASSERT_TRUE(trayIcon != nullptr);

    delete trayIcon;
}
