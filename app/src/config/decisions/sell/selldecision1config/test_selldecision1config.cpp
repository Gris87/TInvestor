#include "src/config/decisions/sell/selldecision1config/selldecision1config.h"

#include <gtest/gtest.h>

#include "src/config/settingseditor/isettingseditor_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_SellDecision1Config, Test_constructor_and_destructor)
{
    SellDecision1Config config;
}

TEST(Test_SellDecision1Config, Test_assign)
{
    SellDecision1Config config;
    SellDecision1Config config2;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);

    config2.setEnabled(true);
    config2.setIncomeAbove(1.6f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),       false);
    ASSERT_EQ(config.getIncomeAbove(),  2.5f);
    ASSERT_EQ(config2.isEnabled(),      true);
    ASSERT_EQ(config2.getIncomeAbove(), 1.6f);
    // clang-format on

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),       true);
    ASSERT_EQ(config.getIncomeAbove(),  1.6f);
    ASSERT_EQ(config2.isEnabled(),      true);
    ASSERT_EQ(config2.getIncomeAbove(), 1.6f);
    // clang-format on
}

TEST(Test_SellDecision1Config, Test_makeDefault)
{
    SellDecision1Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);
    // clang-format on

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getIncomeAbove(), 1.0f);
    // clang-format on
}

TEST(Test_SellDecision1Config, Test_save)
{
    SellDecision1Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),     QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/IncomeAbove"), QVariant(2.5f)));
    // clang-format on

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_SellDecision1Config, Test_load)
{
    SellDecision1Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),     QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/IncomeAbove"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    // clang-format on

    config.load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getIncomeAbove(), 1.7f);
    // clang-format on
}

TEST(Test_SellDecision1Config, Test_setEnabled_and_isEnabled)
{
    SellDecision1Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_SellDecision1Config, Test_setIncomeAbove_and_getIncomeAbove)
{
    SellDecision1Config config;

    config.setIncomeAbove(0.1f);
    ASSERT_EQ(config.getIncomeAbove(), 0.1f);

    config.setIncomeAbove(0.5f);
    ASSERT_EQ(config.getIncomeAbove(), 0.5f);
}
