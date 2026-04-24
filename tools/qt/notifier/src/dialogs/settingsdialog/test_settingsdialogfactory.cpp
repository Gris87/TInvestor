#include "src/dialogs/settingsdialog/settingsdialogfactory.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SettingsDialogFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new SettingsDialogFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    SettingsDialogFactory* factory;
};



TEST_F(Test_SettingsDialogFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_SettingsDialogFactory, Test_newInstance)
{
    const InSequence seq;

    StrictMock<ConfigMock> configMock;

    const std::shared_ptr<ISettingsDialog> dialog = factory->newInstance(&configMock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
