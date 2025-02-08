#include "src/config/decisions/buy/buydecision2config/buydecision2config.h"

#include <gtest/gtest.h>

#include "src/config/settingseditor/isettingseditor_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_BuyDecision2Config, Test_constructor_and_destructor)
{
    BuyDecision2Config config;
}

TEST(Test_BuyDecision2Config, Test_assign)
{
    BuyDecision2Config config;
    BuyDecision2Config config2;

    config.setEnabled(false);
    config.setPriceDiff(2.5f);
    config.setAmountOfTimes(2);
    config.setDuration(123);

    config2.setEnabled(true);
    config2.setPriceDiff(1.6f);
    config2.setAmountOfTimes(6);
    config2.setDuration(321);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),         false);
    ASSERT_EQ(config.getPriceDiff(),      2.5f);
    ASSERT_EQ(config.getAmountOfTimes(),  2);
    ASSERT_EQ(config.getDuration(),       123);
    ASSERT_EQ(config2.isEnabled(),        true);
    ASSERT_EQ(config2.getPriceDiff(),     1.6f);
    ASSERT_EQ(config2.getAmountOfTimes(), 6);
    ASSERT_EQ(config2.getDuration(),      321);
    // clang-format on

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),         true);
    ASSERT_EQ(config.getPriceDiff(),      1.6f);
    ASSERT_EQ(config.getAmountOfTimes(),  6);
    ASSERT_EQ(config.getDuration(),       321);
    ASSERT_EQ(config2.isEnabled(),        true);
    ASSERT_EQ(config2.getPriceDiff(),     1.6f);
    ASSERT_EQ(config2.getAmountOfTimes(), 6);
    ASSERT_EQ(config2.getDuration(),      321);
    // clang-format on
}

TEST(Test_BuyDecision2Config, Test_makeDefault)
{
    BuyDecision2Config config;

    config.setEnabled(false);
    config.setPriceDiff(2.5f);
    config.setAmountOfTimes(5);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        false);
    ASSERT_EQ(config.getPriceDiff(),     2.5f);
    ASSERT_EQ(config.getAmountOfTimes(), 5);
    ASSERT_EQ(config.getDuration(),      123);
    // clang-format on

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        true);
    ASSERT_EQ(config.getPriceDiff(),     1.0f);
    ASSERT_EQ(config.getAmountOfTimes(), 2);
    ASSERT_EQ(config.getDuration(),      15);
    // clang-format on
}

TEST(Test_BuyDecision2Config, Test_save)
{
    BuyDecision2Config config;

    config.setEnabled(false);
    config.setPriceDiff(2.5f);
    config.setAmountOfTimes(2);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        false);
    ASSERT_EQ(config.getPriceDiff(),     2.5f);
    ASSERT_EQ(config.getAmountOfTimes(), 2);
    ASSERT_EQ(config.getDuration(),      123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),       QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/PriceDiff"),     QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/AmountOfTimes"), QVariant(2)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"),      QVariant(123)));
    // clang-format on

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_BuyDecision2Config, Test_load)
{
    BuyDecision2Config config;

    config.setEnabled(false);
    config.setPriceDiff(2.5f);
    config.setAmountOfTimes(2);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        false);
    ASSERT_EQ(config.getPriceDiff(),     2.5f);
    ASSERT_EQ(config.getAmountOfTimes(), 2);
    ASSERT_EQ(config.getDuration(),      123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),       QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/PriceDiff"),     QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/AmountOfTimes"), QVariant(2))).WillOnce(Return(QVariant(7)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"),      QVariant(123))).WillOnce(Return(QVariant(321)));
    // clang-format on

    config.load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config.isEnabled(),        true);
    ASSERT_EQ(config.getPriceDiff(),     1.7f);
    ASSERT_EQ(config.getAmountOfTimes(), 7);
    ASSERT_EQ(config.getDuration(),      321);
    // clang-format on
}

TEST(Test_BuyDecision2Config, Test_setEnabled_and_isEnabled)
{
    BuyDecision2Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_BuyDecision2Config, Test_setPriceDiff_and_getPriceDiff)
{
    BuyDecision2Config config;

    config.setPriceDiff(0.1f);
    ASSERT_EQ(config.getPriceDiff(), 0.1f);

    config.setPriceDiff(0.5f);
    ASSERT_EQ(config.getPriceDiff(), 0.5f);
}

TEST(Test_BuyDecision2Config, Test_setAmountOfTimes_and_getAmountOfTimes)
{
    BuyDecision2Config config;

    config.setAmountOfTimes(1);
    ASSERT_EQ(config.getAmountOfTimes(), 1);

    config.setAmountOfTimes(5);
    ASSERT_EQ(config.getAmountOfTimes(), 5);
}

TEST(Test_BuyDecision2Config, Test_setDuration_and_getDuration)
{
    BuyDecision2Config config;

    config.setDuration(100);
    ASSERT_EQ(config.getDuration(), 100);

    config.setDuration(20);
    ASSERT_EQ(config.getDuration(), 20);
}
