#include "src/config/decisions/sell/selldecision2config/selldecision2config.h"

#include <gtest/gtest.h>

#include "src/config/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



TEST(Test_SellDecision2Config, Test_constructor_and_destructor)
{
    SellDecision2Config config;
}

TEST(Test_SellDecision2Config, Test_assign)
{
    SellDecision2Config config;
    SellDecision2Config config2;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);
    config.setLoseIncome(3.7f);

    config2.setEnabled(true);
    config2.setIncomeAbove(1.6f);
    config2.setLoseIncome(4.2f);

    ASSERT_EQ(config.isEnabled(),       false);
    ASSERT_EQ(config.getIncomeAbove(),  2.5f);
    ASSERT_EQ(config.getLoseIncome(),   3.7f);
    ASSERT_EQ(config2.isEnabled(),      true);
    ASSERT_EQ(config2.getIncomeAbove(), 1.6f);
    ASSERT_EQ(config2.getLoseIncome(),  4.2f);

    config.assign(&config2);

    ASSERT_EQ(config.isEnabled(),       true);
    ASSERT_EQ(config.getIncomeAbove(),  1.6f);
    ASSERT_EQ(config.getLoseIncome(),   4.2f);
    ASSERT_EQ(config2.isEnabled(),      true);
    ASSERT_EQ(config2.getIncomeAbove(), 1.6f);
    ASSERT_EQ(config2.getLoseIncome(),  4.2f);
}

TEST(Test_SellDecision2Config, Test_makeDefault)
{
    SellDecision2Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);
    config.setLoseIncome(4.2f);

    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);
    ASSERT_EQ(config.getLoseIncome(),  4.2f);

    config.makeDefault();

    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getIncomeAbove(), 1.0f);
    ASSERT_EQ(config.getLoseIncome(),  0.1f);
}

TEST(Test_SellDecision2Config, Test_save)
{
    SellDecision2Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);
    config.setLoseIncome(4.2f);

    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);
    ASSERT_EQ(config.getLoseIncome(),  4.2f);

    StrictMock<SettingsEditorMock> settingsEditorMock;

    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),     QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/IncomeAbove"), QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/LoseIncome"),  QVariant(4.2f)));

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_SellDecision2Config, Test_load)
{
    SellDecision2Config config;

    config.setEnabled(false);
    config.setIncomeAbove(2.5f);
    config.setLoseIncome(4.2f);

    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getIncomeAbove(), 2.5f);
    ASSERT_EQ(config.getLoseIncome(),  4.2f);

    StrictMock<SettingsEditorMock> settingsEditorMock;

    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),     QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/IncomeAbove"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/LoseIncome"),  QVariant(4.2f))).WillOnce(Return(QVariant(8.9f)));

    config.load(&settingsEditorMock, "BLAH");

    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getIncomeAbove(), 1.7f);
    ASSERT_EQ(config.getLoseIncome(),  8.9f);
}

TEST(Test_SellDecision2Config, Test_setEnabled_and_isEnabled)
{
    SellDecision2Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_SellDecision2Config, Test_setIncomeAbove_and_getIncomeAbove)
{
    SellDecision2Config config;

    config.setIncomeAbove(0.1f);
    ASSERT_EQ(config.getIncomeAbove(), 0.1f);

    config.setIncomeAbove(0.5f);
    ASSERT_EQ(config.getIncomeAbove(), 0.5f);
}

TEST(Test_SellDecision2Config, Test_setLoseIncome_and_getLoseIncome)
{
    SellDecision2Config config;

    config.setLoseIncome(0.1f);
    ASSERT_EQ(config.getLoseIncome(), 0.1f);

    config.setLoseIncome(0.5f);
    ASSERT_EQ(config.getLoseIncome(), 0.5f);
}
