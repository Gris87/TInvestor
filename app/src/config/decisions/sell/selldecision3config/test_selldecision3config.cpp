#include "src/config/decisions/sell/selldecision3config/selldecision3config.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_SellDecision3Config, Test_constructor_and_destructor)
{
    const SellDecision3Config config;
}

TEST(Test_SellDecision3Config, Test_assign)
{
    SellDecision3Config config;
    SellDecision3Config config2;

    config.setEnabled(false);
    config.setLoseIncome(3.7f);
    config.setDuration(2);

    config2.setEnabled(true);
    config2.setLoseIncome(4.2f);
    config2.setDuration(6);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      false);
    ASSERT_EQ(config.getLoseIncome(),  3.7f);
    ASSERT_EQ(config.getDuration(),    2);
    ASSERT_EQ(config2.isEnabled(),     true);
    ASSERT_EQ(config2.getLoseIncome(), 4.2f);
    ASSERT_EQ(config2.getDuration(),   6);
    // clang-format on

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),      true);
    ASSERT_EQ(config.getLoseIncome(),  4.2f);
    ASSERT_EQ(config.getDuration(),    6);
    ASSERT_EQ(config2.isEnabled(),     true);
    ASSERT_EQ(config2.getLoseIncome(), 4.2f);
    ASSERT_EQ(config2.getDuration(),   6);
    // clang-format on
}

TEST(Test_SellDecision3Config, Test_makeDefault)
{
    SellDecision3Config config;

    config.setEnabled(false);
    config.setLoseIncome(4.2f);
    config.setDuration(2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     false);
    ASSERT_EQ(config.getLoseIncome(), 4.2f);
    ASSERT_EQ(config.getDuration(),   2);
    // clang-format on

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     true);
    ASSERT_EQ(config.getLoseIncome(), 5.0f);
    ASSERT_EQ(config.getDuration(),   3);
    // clang-format on
}

TEST(Test_SellDecision3Config, Test_save)
{
    const InSequence seq;

    SellDecision3Config config;

    config.setEnabled(false);
    config.setLoseIncome(4.2f);
    config.setDuration(2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     false);
    ASSERT_EQ(config.getLoseIncome(), 4.2f);
    ASSERT_EQ(config.getDuration(),   2);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),    QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/LoseIncome"), QVariant(4.2f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"),   QVariant(2)));
    // clang-format on

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_SellDecision3Config, Test_load)
{
    const InSequence seq;

    SellDecision3Config config;

    config.setEnabled(false);
    config.setLoseIncome(4.2f);
    config.setDuration(2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     false);
    ASSERT_EQ(config.getLoseIncome(), 4.2f);
    ASSERT_EQ(config.getDuration(),   2);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),    QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/LoseIncome"), QVariant(4.2f))).WillOnce(Return(QVariant(8.9f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"),   QVariant(2))).WillOnce(Return(QVariant(1)));
    // clang-format on

    config.load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     true);
    ASSERT_EQ(config.getLoseIncome(), 8.9f);
    ASSERT_EQ(config.getDuration(),   1);
    // clang-format on
}

TEST(Test_SellDecision3Config, Test_setEnabled_and_isEnabled)
{
    SellDecision3Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_SellDecision3Config, Test_setLoseIncome_and_getLoseIncome)
{
    SellDecision3Config config;

    config.setLoseIncome(0.1f);
    ASSERT_EQ(config.getLoseIncome(), 0.1f);

    config.setLoseIncome(0.5f);
    ASSERT_EQ(config.getLoseIncome(), 0.5f);
}

TEST(Test_SellDecision3Config, Test_setDuration_and_getDuration)
{
    SellDecision3Config config;

    config.setDuration(1);
    ASSERT_EQ(config.getDuration(), 1);

    config.setDuration(5);
    ASSERT_EQ(config.getDuration(), 5);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
