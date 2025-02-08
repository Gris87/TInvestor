#include "src/config/decisions/buy/buydecision3config/buydecision3config.h"

#include <gtest/gtest.h>

#include "src/config/settingseditor/isettingseditor_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_BuyDecision3Config, Test_constructor_and_destructor)
{
    BuyDecision3Config config;
}

TEST(Test_BuyDecision3Config, Test_assign)
{
    BuyDecision3Config config;
    BuyDecision3Config config2;

    config.setEnabled(false);
    config.setPriceRise(2.5f);
    config.setDuration(123);

    config2.setEnabled(true);
    config2.setPriceRise(1.6f);
    config2.setDuration(321);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     false);
    ASSERT_EQ(config.getPriceRise(),  2.5f);
    ASSERT_EQ(config.getDuration(),   123);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getPriceRise(), 1.6f);
    ASSERT_EQ(config2.getDuration(),  321);
    // clang-format on

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     true);
    ASSERT_EQ(config.getPriceRise(),  1.6f);
    ASSERT_EQ(config.getDuration(),   321);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getPriceRise(), 1.6f);
    ASSERT_EQ(config2.getDuration(),  321);
    // clang-format on
}

TEST(Test_BuyDecision3Config, Test_makeDefault)
{
    BuyDecision3Config config;

    config.setEnabled(false);
    config.setPriceRise(2.5f);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getPriceRise(), 2.5f);
    ASSERT_EQ(config.getDuration(),  123);
    // clang-format on

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getPriceRise(), 2.0f);
    ASSERT_EQ(config.getDuration(),  1);
    // clang-format on
}

TEST(Test_BuyDecision3Config, Test_save)
{
    BuyDecision3Config config;

    config.setEnabled(false);
    config.setPriceRise(2.5f);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getPriceRise(), 2.5f);
    ASSERT_EQ(config.getDuration(),  123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),   QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/PriceRise"), QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"),  QVariant(123)));
    // clang-format on

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_BuyDecision3Config, Test_load)
{
    BuyDecision3Config config;

    config.setEnabled(false);
    config.setPriceRise(2.5f);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getPriceRise(), 2.5f);
    ASSERT_EQ(config.getDuration(),  123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),   QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/PriceRise"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"),  QVariant(123))).WillOnce(Return(QVariant(321)));
    // clang-format on

    config.load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getPriceRise(), 1.7f);
    ASSERT_EQ(config.getDuration(),  321);
    // clang-format on
}

TEST(Test_BuyDecision3Config, Test_setEnabled_and_isEnabled)
{
    BuyDecision3Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_BuyDecision3Config, Test_setPriceRise_and_getPriceRise)
{
    BuyDecision3Config config;

    config.setPriceRise(0.1f);
    ASSERT_EQ(config.getPriceRise(), 0.1f);

    config.setPriceRise(0.5f);
    ASSERT_EQ(config.getPriceRise(), 0.5f);
}

TEST(Test_BuyDecision3Config, Test_setDuration_and_getDuration)
{
    BuyDecision3Config config;

    config.setDuration(100);
    ASSERT_EQ(config.getDuration(), 100);

    config.setDuration(20);
    ASSERT_EQ(config.getDuration(), 20);
}
