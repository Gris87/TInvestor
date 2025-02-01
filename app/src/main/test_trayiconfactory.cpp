#include "src/main/trayiconfactory.h"

#include <gtest/gtest.h>



TEST(Test_TrayIconFactory, Test_constructor_and_destructor)
{
    TrayIconFactory factory;
}

TEST(Test_TrayIconFactory, Test_newInstance)
{
    TrayIconFactory factory;

    ITrayIcon *trayIcon = factory.newInstance(nullptr);
    ASSERT_TRUE(trayIcon != nullptr);

    delete trayIcon;
}
