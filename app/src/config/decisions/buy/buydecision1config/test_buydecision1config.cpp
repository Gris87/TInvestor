#include "src/config/decisions/buy/buydecision1config/buydecision1config.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_BuyDecision1Config, Test_constructor_and_destructor)
{
    BuyDecision1Config config;
}

TEST(Test_BuyDecision1Config, Test_assign)
{
    BuyDecision1Config config;
    BuyDecision1Config config2;

    config.setEnabled(false);
    config.setPriceFall(2.5f);
    config.setDuration(123);

    config2.setEnabled(true);
    config2.setPriceFall(1.6f);
    config2.setDuration(321);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     false);
    ASSERT_EQ(config.getPriceFall(),  2.5f);
    ASSERT_EQ(config.getDuration(),   123);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getPriceFall(), 1.6f);
    ASSERT_EQ(config2.getDuration(),  321);
    // clang-format on

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),     true);
    ASSERT_EQ(config.getPriceFall(),  1.6f);
    ASSERT_EQ(config.getDuration(),   321);
    ASSERT_EQ(config2.isEnabled(),    true);
    ASSERT_EQ(config2.getPriceFall(), 1.6f);
    ASSERT_EQ(config2.getDuration(),  321);
    // clang-format on
}

TEST(Test_BuyDecision1Config, Test_makeDefault)
{
    BuyDecision1Config config;

    config.setEnabled(false);
    config.setPriceFall(2.5f);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getPriceFall(), 2.5f);
    ASSERT_EQ(config.getDuration(),  123);
    // clang-format on

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getPriceFall(), 1.0f);
    ASSERT_EQ(config.getDuration(),  5);
    // clang-format on
}

TEST(Test_BuyDecision1Config, Test_save)
{
    BuyDecision1Config config;

    config.setEnabled(false);
    config.setPriceFall(2.5f);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getPriceFall(), 2.5f);
    ASSERT_EQ(config.getDuration(),  123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Enabled"),   QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/PriceFall"), QVariant(2.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("BLAH/Duration"),  QVariant(123)));
    // clang-format on

    config.save(&settingsEditorMock, "BLAH");
}

TEST(Test_BuyDecision1Config, Test_load)
{
    BuyDecision1Config config;

    config.setEnabled(false);
    config.setPriceFall(2.5f);
    config.setDuration(123);

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    false);
    ASSERT_EQ(config.getPriceFall(), 2.5f);
    ASSERT_EQ(config.getDuration(),  123);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Enabled"),   QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/PriceFall"), QVariant(2.5f))).WillOnce(Return(QVariant(1.7f)));
    EXPECT_CALL(settingsEditorMock, value(QString("BLAH/Duration"),  QVariant(123))).WillOnce(Return(QVariant(321)));
    // clang-format on

    config.load(&settingsEditorMock, "BLAH");

    // clang-format off
    ASSERT_EQ(config.isEnabled(),    true);
    ASSERT_EQ(config.getPriceFall(), 1.7f);
    ASSERT_EQ(config.getDuration(),  321);
    // clang-format on
}

TEST(Test_BuyDecision1Config, Test_setEnabled_and_isEnabled)
{
    BuyDecision1Config config;

    config.setEnabled(false);
    ASSERT_EQ(config.isEnabled(), false);

    config.setEnabled(true);
    ASSERT_EQ(config.isEnabled(), true);
}

TEST(Test_BuyDecision1Config, Test_setPriceFall_and_getPriceFall)
{
    BuyDecision1Config config;

    config.setPriceFall(0.1f);
    ASSERT_EQ(config.getPriceFall(), 0.1f);

    config.setPriceFall(0.5f);
    ASSERT_EQ(config.getPriceFall(), 0.5f);
}

TEST(Test_BuyDecision1Config, Test_setDuration_and_getDuration)
{
    BuyDecision1Config config;

    config.setDuration(100);
    ASSERT_EQ(config.getDuration(), 100);

    config.setDuration(20);
    ASSERT_EQ(config.getDuration(), 20);
}
