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
    config->setNotificationsEnabled(false);
    config->setFilterSystem(false);
    config->setFilterPortfolio(false);
    config->setFilterHugeSell(false);
    config->setFilterDividends(false);
    config->setFilterPulseNeutral(false);
    config->setFilterPulseBuy(false);
    config->setFilterPulseSell(false);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),              false);
    ASSERT_EQ(config->getServerAddress(),       "yandex.ru");
    ASSERT_EQ(config->getServerPort(),          80);
    ASSERT_EQ(config->isNotificationsEnabled(), false);
    ASSERT_EQ(config->isFilterSystem(),         false);
    ASSERT_EQ(config->isFilterPortfolio(),      false);
    ASSERT_EQ(config->isFilterHugeSell(),       false);
    ASSERT_EQ(config->isFilterDividends(),      false);
    ASSERT_EQ(config->isFilterPulseNeutral(),   false);
    ASSERT_EQ(config->isFilterPulseBuy(),       false);
    ASSERT_EQ(config->isFilterPulseSell(),      false);
    // clang-format on

    config2.setAutorun(true);
    config2.setServerAddress("mail.ru");
    config2.setServerPort(443);
    config2.setNotificationsEnabled(true);
    config2.setFilterSystem(true);
    config2.setFilterPortfolio(true);
    config2.setFilterHugeSell(true);
    config2.setFilterDividends(true);
    config2.setFilterPulseNeutral(true);
    config2.setFilterPulseBuy(true);
    config2.setFilterPulseSell(true);

    // clang-format off
    ASSERT_EQ(config2.isAutorun(),              true);
    ASSERT_EQ(config2.getServerAddress(),       "mail.ru");
    ASSERT_EQ(config2.getServerPort(),          443);
    ASSERT_EQ(config2.isNotificationsEnabled(), true);
    ASSERT_EQ(config2.isFilterSystem(),         true);
    ASSERT_EQ(config2.isFilterPortfolio(),      true);
    ASSERT_EQ(config2.isFilterHugeSell(),       true);
    ASSERT_EQ(config2.isFilterDividends(),      true);
    ASSERT_EQ(config2.isFilterPulseNeutral(),   true);
    ASSERT_EQ(config2.isFilterPulseBuy(),       true);
    ASSERT_EQ(config2.isFilterPulseSell(),      true);
    // clang-format on

    config->assign(&config2);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),              true);
    ASSERT_EQ(config->getServerAddress(),       "mail.ru");
    ASSERT_EQ(config->getServerPort(),          443);
    ASSERT_EQ(config->isNotificationsEnabled(), true);
    ASSERT_EQ(config->isFilterSystem(),         true);
    ASSERT_EQ(config->isFilterPortfolio(),      true);
    ASSERT_EQ(config->isFilterHugeSell(),       true);
    ASSERT_EQ(config->isFilterDividends(),      true);
    ASSERT_EQ(config->isFilterPulseNeutral(),   true);
    ASSERT_EQ(config->isFilterPulseBuy(),       true);
    ASSERT_EQ(config->isFilterPulseSell(),      true);
    // clang-format on

    // clang-format off
    ASSERT_EQ(config2.isAutorun(),              true);
    ASSERT_EQ(config2.getServerAddress(),       "mail.ru");
    ASSERT_EQ(config2.getServerPort(),          443);
    ASSERT_EQ(config2.isNotificationsEnabled(), true);
    ASSERT_EQ(config2.isFilterSystem(),         true);
    ASSERT_EQ(config2.isFilterPortfolio(),      true);
    ASSERT_EQ(config2.isFilterHugeSell(),       true);
    ASSERT_EQ(config2.isFilterDividends(),      true);
    ASSERT_EQ(config2.isFilterPulseNeutral(),   true);
    ASSERT_EQ(config2.isFilterPulseBuy(),       true);
    ASSERT_EQ(config2.isFilterPulseSell(),      true);
    // clang-format on
}

TEST_F(Test_Config, Test_makeDefault)
{
    config->setAutorun(false);
    config->setServerAddress("yandex.ru");
    config->setServerPort(80);
    config->setNotificationsEnabled(false);
    config->setFilterSystem(false);
    config->setFilterPortfolio(false);
    config->setFilterHugeSell(false);
    config->setFilterDividends(false);
    config->setFilterPulseNeutral(false);
    config->setFilterPulseBuy(false);
    config->setFilterPulseSell(false);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),              false);
    ASSERT_EQ(config->getServerAddress(),       "yandex.ru");
    ASSERT_EQ(config->getServerPort(),          80);
    ASSERT_EQ(config->isNotificationsEnabled(), false);
    ASSERT_EQ(config->isFilterSystem(),         false);
    ASSERT_EQ(config->isFilterPortfolio(),      false);
    ASSERT_EQ(config->isFilterHugeSell(),       false);
    ASSERT_EQ(config->isFilterDividends(),      false);
    ASSERT_EQ(config->isFilterPulseNeutral(),   false);
    ASSERT_EQ(config->isFilterPulseBuy(),       false);
    ASSERT_EQ(config->isFilterPulseSell(),      false);
    // clang-format on

    config->makeDefault();

    // clang-format off
    ASSERT_EQ(config->isAutorun(),              true);
    ASSERT_EQ(config->getServerAddress(),       "localhost");
    ASSERT_EQ(config->getServerPort(),          8041);
    ASSERT_EQ(config->isNotificationsEnabled(), true);
    ASSERT_EQ(config->isFilterSystem(),         true);
    ASSERT_EQ(config->isFilterPortfolio(),      true);
    ASSERT_EQ(config->isFilterHugeSell(),       true);
    ASSERT_EQ(config->isFilterDividends(),      true);
    ASSERT_EQ(config->isFilterPulseNeutral(),   true);
    ASSERT_EQ(config->isFilterPulseBuy(),       true);
    ASSERT_EQ(config->isFilterPulseSell(),      true);
    // clang-format on
}

TEST_F(Test_Config, Test_save)
{
    const InSequence seq;

    config->setAutorun(false);
    config->setServerAddress("yandex.ru");
    config->setServerPort(80);
    config->setNotificationsEnabled(false);
    config->setFilterSystem(false);
    config->setFilterPortfolio(false);
    config->setFilterHugeSell(false);
    config->setFilterDividends(false);
    config->setFilterPulseNeutral(false);
    config->setFilterPulseBuy(false);
    config->setFilterPulseSell(false);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),              false);
    ASSERT_EQ(config->getServerAddress(),       "yandex.ru");
    ASSERT_EQ(config->getServerPort(),          80);
    ASSERT_EQ(config->isNotificationsEnabled(), false);
    ASSERT_EQ(config->isFilterSystem(),         false);
    ASSERT_EQ(config->isFilterPortfolio(),      false);
    ASSERT_EQ(config->isFilterHugeSell(),       false);
    ASSERT_EQ(config->isFilterDividends(),      false);
    ASSERT_EQ(config->isFilterPulseNeutral(),   false);
    ASSERT_EQ(config->isFilterPulseBuy(),       false);
    ASSERT_EQ(config->isFilterPulseSell(),      false);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/Autorun"),              QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/ServerAddress"),        QVariant("yandex.ru")));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/ServerPort"),           QVariant(80)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/NotificationsEnabled"), QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/FilterSystem"),         QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/FilterPortfolio"),      QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/FilterHugeSell"),       QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/FilterDividends"),      QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/FilterPulseNeutral"),   QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/FilterPulseBuy"),       QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/FilterPulseSell"),      QVariant(false)));
    // clang-format on

    config->save(&settingsEditorMock);
}

TEST_F(Test_Config, Test_load)
{
    const InSequence seq;

    config->setAutorun(false);
    config->setServerAddress("yandex.ru");
    config->setServerPort(80);
    config->setNotificationsEnabled(false);
    config->setFilterSystem(false);
    config->setFilterPortfolio(false);
    config->setFilterHugeSell(false);
    config->setFilterDividends(false);
    config->setFilterPulseNeutral(false);
    config->setFilterPulseBuy(false);
    config->setFilterPulseSell(false);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),              false);
    ASSERT_EQ(config->getServerAddress(),       "yandex.ru");
    ASSERT_EQ(config->getServerPort(),          80);
    ASSERT_EQ(config->isNotificationsEnabled(), false);
    ASSERT_EQ(config->isFilterSystem(),         false);
    ASSERT_EQ(config->isFilterPortfolio(),      false);
    ASSERT_EQ(config->isFilterHugeSell(),       false);
    ASSERT_EQ(config->isFilterDividends(),      false);
    ASSERT_EQ(config->isFilterPulseNeutral(),   false);
    ASSERT_EQ(config->isFilterPulseBuy(),       false);
    ASSERT_EQ(config->isFilterPulseSell(),      false);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("Config/Autorun"),              QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/ServerAddress"),        QVariant("yandex.ru"))).WillOnce(Return(QVariant("mail.ru")));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/ServerPort"),           QVariant(80))).WillOnce(Return(QVariant(443)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/NotificationsEnabled"), QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/FilterSystem"),         QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/FilterPortfolio"),      QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/FilterHugeSell"),       QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/FilterDividends"),      QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/FilterPulseNeutral"),   QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/FilterPulseBuy"),       QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/FilterPulseSell"),      QVariant(false))).WillOnce(Return(QVariant(true)));
    // clang-format on

    config->load(&settingsEditorMock);

    // clang-format off
    ASSERT_EQ(config->isAutorun(),              true);
    ASSERT_EQ(config->getServerAddress(),       "mail.ru");
    ASSERT_EQ(config->getServerPort(),          443);
    ASSERT_EQ(config->isNotificationsEnabled(), true);
    ASSERT_EQ(config->isFilterSystem(),         true);
    ASSERT_EQ(config->isFilterPortfolio(),      true);
    ASSERT_EQ(config->isFilterHugeSell(),       true);
    ASSERT_EQ(config->isFilterDividends(),      true);
    ASSERT_EQ(config->isFilterPulseNeutral(),   true);
    ASSERT_EQ(config->isFilterPulseBuy(),       true);
    ASSERT_EQ(config->isFilterPulseSell(),      true);
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

TEST_F(Test_Config, Test_setNotificationsEnabled_and_isNotificationsEnabled)
{
    config->setNotificationsEnabled(false);
    ASSERT_EQ(config->isNotificationsEnabled(), false);

    config->setNotificationsEnabled(true);
    ASSERT_EQ(config->isNotificationsEnabled(), true);
}

TEST_F(Test_Config, Test_setFilterSystem_and_isFilterSystem)
{
    config->setFilterSystem(false);
    ASSERT_EQ(config->isFilterSystem(), false);

    config->setFilterSystem(true);
    ASSERT_EQ(config->isFilterSystem(), true);
}

TEST_F(Test_Config, Test_setFilterPortfolio_and_isFilterPortfolio)
{
    config->setFilterPortfolio(false);
    ASSERT_EQ(config->isFilterPortfolio(), false);

    config->setFilterPortfolio(true);
    ASSERT_EQ(config->isFilterPortfolio(), true);
}

TEST_F(Test_Config, Test_setFilterHugeSell_and_isFilterHugeSell)
{
    config->setFilterHugeSell(false);
    ASSERT_EQ(config->isFilterHugeSell(), false);

    config->setFilterHugeSell(true);
    ASSERT_EQ(config->isFilterHugeSell(), true);
}

TEST_F(Test_Config, Test_setFilterDividends_and_isFilterDividends)
{
    config->setFilterDividends(false);
    ASSERT_EQ(config->isFilterDividends(), false);

    config->setFilterDividends(true);
    ASSERT_EQ(config->isFilterDividends(), true);
}

TEST_F(Test_Config, Test_setFilterPulseNeutral_and_isFilterPulseNeutral)
{
    config->setFilterPulseNeutral(false);
    ASSERT_EQ(config->isFilterPulseNeutral(), false);

    config->setFilterPulseNeutral(true);
    ASSERT_EQ(config->isFilterPulseNeutral(), true);
}

TEST_F(Test_Config, Test_setFilterPulseBuy_and_isFilterPulseBuy)
{
    config->setFilterPulseBuy(false);
    ASSERT_EQ(config->isFilterPulseBuy(), false);

    config->setFilterPulseBuy(true);
    ASSERT_EQ(config->isFilterPulseBuy(), true);
}

TEST_F(Test_Config, Test_setFilterPulseSell_and_isFilterPulseSell)
{
    config->setFilterPulseSell(false);
    ASSERT_EQ(config->isFilterPulseSell(), false);

    config->setFilterPulseSell(true);
    ASSERT_EQ(config->isFilterPulseSell(), true);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
