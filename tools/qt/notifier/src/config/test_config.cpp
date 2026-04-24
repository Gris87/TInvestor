#include "src/config/config.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config = new Config();
    }

    void TearDown() override
    {
        delete config;
    }

    Config* config;
};



TEST_F(Test_Config, Test_constructor_and_destructor)
{
}

TEST_F(Test_Config, Test_assign)
{
    Config config2;

    config->setAutorun(false);
    config->setServerAddress("yandex.ru");
    config->setServerPort(80);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),        false);
    ASSERT_EQ(config->getServerAddress(), "yandex.ru");
    ASSERT_EQ(config->getServerPort(),    80);
    // clang-format on

    config2.setAutorun(true);
    config2.setServerAddress("mail.ru");
    config2.setServerPort(443);

    // clang-format off
    ASSERT_EQ(config2.isAutorun(),        true);
    ASSERT_EQ(config2.getServerAddress(), "mail.ru");
    ASSERT_EQ(config2.getServerPort(),    443);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),        true);
    ASSERT_EQ(config->getServerAddress(), "mail.ru");
    ASSERT_EQ(config->getServerPort(),    443);
    // clang-format on

    // clang-format off
    ASSERT_EQ(config2.isAutorun(),        true);
    ASSERT_EQ(config2.getServerAddress(), "mail.ru");
    ASSERT_EQ(config2.getServerPort(),    443);
    // clang-format on
}

TEST_F(Test_Config, Test_makeDefault)
{
    config->setAutorun(false);
    config->setServerAddress("yandex.ru");
    config->setServerPort(80);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),        false);
    ASSERT_EQ(config->getServerAddress(), "yandex.ru");
    ASSERT_EQ(config->getServerPort(),    80);
    // clang-format on

    config->makeDefault();

    // clang-format off
    ASSERT_EQ(config->isAutorun(),        true);
    ASSERT_EQ(config->getServerAddress(), "localhost");
    ASSERT_EQ(config->getServerPort(),    8041);
    // clang-format on
}

TEST_F(Test_Config, Test_save)
{
    const InSequence seq;

    config->setAutorun(false);
    config->setServerAddress("yandex.ru");
    config->setServerPort(80);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),        false);
    ASSERT_EQ(config->getServerAddress(), "yandex.ru");
    ASSERT_EQ(config->getServerPort(),    80);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/Autorun"),       QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/ServerAddress"), QVariant("yandex.ru")));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/ServerPort"),    QVariant(80)));
    // clang-format on

    config->save(&settingsEditorMock);
}

TEST_F(Test_Config, Test_load)
{
    const InSequence seq;

    config->setAutorun(false);
    config->setServerAddress("yandex.ru");
    config->setServerPort(80);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),        false);
    ASSERT_EQ(config->getServerAddress(), "yandex.ru");
    ASSERT_EQ(config->getServerPort(),    80);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("Config/Autorun"),       QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/ServerAddress"), QVariant("yandex.ru"))).WillOnce(Return(QVariant("mail.ru")));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/ServerPort"),    QVariant(80))).WillOnce(Return(QVariant(443)));
    // clang-format on

    config->load(&settingsEditorMock);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),        true);
    ASSERT_EQ(config->getServerAddress(), "mail.ru");
    ASSERT_EQ(config->getServerPort(),    443);
    // clang-format on
}

TEST_F(Test_Config, Test_setAutorun_and_isAutorun)
{
    config->setAutorun(false);
    ASSERT_EQ(config->isAutorun(), false);

    config->setAutorun(true);
    ASSERT_EQ(config->isAutorun(), true);
}

TEST_F(Test_Config, Test_setServerAddress_and_getServerAddress)
{
    config->setServerAddress("yandex.ru");
    ASSERT_EQ(config->getServerAddress(), "yandex.ru");

    config->setServerAddress("mail.ru");
    ASSERT_EQ(config->getServerAddress(), "mail.ru");
}

TEST_F(Test_Config, Test_setServerPort_and_getServerPort)
{
    config->setServerPort(80);
    ASSERT_EQ(config->getServerPort(), 80);

    config->setServerPort(443);
    ASSERT_EQ(config->getServerPort(), 443);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
