#include "src/widgets/trayicon/trayiconfactory.h"

#include <gtest/gtest.h>



class Test_TrayIconFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new TrayIconFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    TrayIconFactory* factory;
};



TEST_F(Test_TrayIconFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_TrayIconFactory, Test_newInstance)
{
    const ITrayIcon* trayIcon = factory->newInstance(nullptr);
    ASSERT_NE(trayIcon, nullptr);

    delete trayIcon;
}
