#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision1config.h"
#include "src/config/isettingseditor_mock.h"



using ::testing::StrictMock;
using ::testing::Return;



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

    ASSERT_EQ(config.isEnabled(),       false);
    ASSERT_EQ(config.getIncomeAbove(),  2.5f);
    ASSERT_EQ(config2.isEnabled(),      true);
    ASSERT_EQ(config2.getIncomeAbove(), 1.6f);

    config.assign(&config2);

    ASSERT_EQ(config.isEnabled(),       true);
    ASSERT_EQ(config.getIncomeAbove(),  1.6f);
    ASSERT_EQ(config2.isEnabled(),      true);
    ASSERT_EQ(config2.getIncomeAbove(), 1.6f);
}

TEST(Test_SellDecision1Config, Test_makeDefault)
{
    SellDecision1Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);

    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);

    config.makeDefault();

    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getIncomeAbove(), 1.0f);
}

TEST(Test_SellDecision1Config, Test_save)
{
    SellDecision1Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);

    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);

    StrictMock<SettingsEditorMock> settingsEditorMock;

    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),     QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/IncomeAbove"), QVariant(2.5f)));

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_SellDecision1Config, Test_load)
{
    SellDecision1Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);

    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);

    StrictMock<SettingsEditorMock> settingsEditorMock;

    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),     QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/IncomeAbove"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));

    config.load(&settingsEditorMock, "BLAH");

    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getIncomeAbove(), 1.7f);
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
