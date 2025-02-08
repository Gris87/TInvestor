#include "src/config/config.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/settingseditor/isettingseditor_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_Config, Test_constructor_and_destructor)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);
}

TEST(Test_Config, Test_assign)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock2;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock2;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);
    Config config2(&simulatorConfigMock2, &autoPilotConfigMock2);

    config.setAutorun(false);
    config.setMakeDecisionTimeout(5);
    config.setUseSchedule(false);
    config.setScheduleStartHour(8);
    config.setScheduleStartMinute(15);
    config.setScheduleEndHour(20);
    config.setScheduleEndMinute(50);
    config.setLimitPurchasesPerDay(false);
    config.setAmountOfPurchasesPerDay(100);
    config.setLimitPurchasesPerStock(false);
    config.setAmountOfPurchasesPerStock(25);
    config.setCommission(0.5f);
    config.setLimitStockPurchase(false);
    config.setAmountOfStockPurchase(50000);
    config.setStorageMonthLimit(36);
    config.setSimulatorConfigCommon(false);
    config.setAutoPilotConfigCommon(true);

    // clang-format off
    ASSERT_EQ(config.isAutorun(),                    false);
    ASSERT_EQ(config.getMakeDecisionTimeout(),       5);
    ASSERT_EQ(config.isUseSchedule(),                false);
    ASSERT_EQ(config.getScheduleStartHour(),         8);
    ASSERT_EQ(config.getScheduleStartMinute(),       15);
    ASSERT_EQ(config.getScheduleEndHour(),           20);
    ASSERT_EQ(config.getScheduleEndMinute(),         50);
    ASSERT_EQ(config.isLimitPurchasesPerDay(),       false);
    ASSERT_EQ(config.getAmountOfPurchasesPerDay(),   100);
    ASSERT_EQ(config.isLimitPurchasesPerStock(),     false);
    ASSERT_EQ(config.getAmountOfPurchasesPerStock(), 25);
    ASSERT_EQ(config.getCommission(),                0.5f);
    ASSERT_EQ(config.isLimitStockPurchase(),         false);
    ASSERT_EQ(config.getAmountOfStockPurchase(),     50000);
    ASSERT_EQ(config.getStorageMonthLimit(),         36);
    ASSERT_EQ(config.isSimulatorConfigCommon(),      false);
    ASSERT_EQ(config.isAutoPilotConfigCommon(),      true);
    // clang-format on

    config2.setAutorun(true);
    config2.setMakeDecisionTimeout(30);
    config2.setUseSchedule(true);
    config2.setScheduleStartHour(7);
    config2.setScheduleStartMinute(5);
    config2.setScheduleEndHour(21);
    config2.setScheduleEndMinute(37);
    config2.setLimitPurchasesPerDay(true);
    config2.setAmountOfPurchasesPerDay(250);
    config2.setLimitPurchasesPerStock(true);
    config2.setAmountOfPurchasesPerStock(123);
    config2.setCommission(0.8f);
    config2.setLimitStockPurchase(true);
    config2.setAmountOfStockPurchase(75000);
    config2.setStorageMonthLimit(12);
    config2.setSimulatorConfigCommon(true);
    config2.setAutoPilotConfigCommon(false);

    // clang-format off
    ASSERT_EQ(config2.isAutorun(),                    true);
    ASSERT_EQ(config2.getMakeDecisionTimeout(),       30);
    ASSERT_EQ(config2.isUseSchedule(),                true);
    ASSERT_EQ(config2.getScheduleStartHour(),         7);
    ASSERT_EQ(config2.getScheduleStartMinute(),       5);
    ASSERT_EQ(config2.getScheduleEndHour(),           21);
    ASSERT_EQ(config2.getScheduleEndMinute(),         37);
    ASSERT_EQ(config2.isLimitPurchasesPerDay(),       true);
    ASSERT_EQ(config2.getAmountOfPurchasesPerDay(),   250);
    ASSERT_EQ(config2.isLimitPurchasesPerStock(),     true);
    ASSERT_EQ(config2.getAmountOfPurchasesPerStock(), 123);
    ASSERT_EQ(config2.getCommission(),                0.8f);
    ASSERT_EQ(config2.isLimitStockPurchase(),         true);
    ASSERT_EQ(config2.getAmountOfStockPurchase(),     75000);
    ASSERT_EQ(config2.getStorageMonthLimit(),         12);
    ASSERT_EQ(config2.isSimulatorConfigCommon(),      true);
    ASSERT_EQ(config2.isAutoPilotConfigCommon(),      false);
    // clang-format on

    EXPECT_CALL(simulatorConfigMock, assign(&simulatorConfigMock2));
    EXPECT_CALL(autoPilotConfigMock, assign(&autoPilotConfigMock2));

    config.assign(&config2);

    // clang-format off
    ASSERT_EQ(config.isAutorun(),                    true);
    ASSERT_EQ(config.getMakeDecisionTimeout(),       30);
    ASSERT_EQ(config.isUseSchedule(),                true);
    ASSERT_EQ(config.getScheduleStartHour(),         7);
    ASSERT_EQ(config.getScheduleStartMinute(),       5);
    ASSERT_EQ(config.getScheduleEndHour(),           21);
    ASSERT_EQ(config.getScheduleEndMinute(),         37);
    ASSERT_EQ(config.isLimitPurchasesPerDay(),       true);
    ASSERT_EQ(config.getAmountOfPurchasesPerDay(),   250);
    ASSERT_EQ(config.isLimitPurchasesPerStock(),     true);
    ASSERT_EQ(config.getAmountOfPurchasesPerStock(), 123);
    ASSERT_EQ(config.getCommission(),                0.8f);
    ASSERT_EQ(config.isLimitStockPurchase(),         true);
    ASSERT_EQ(config.getAmountOfStockPurchase(),     75000);
    ASSERT_EQ(config.getStorageMonthLimit(),         12);
    ASSERT_EQ(config.isSimulatorConfigCommon(),      true);
    ASSERT_EQ(config.isAutoPilotConfigCommon(),      false);
    // clang-format on

    // clang-format off
    ASSERT_EQ(config2.isAutorun(),                    true);
    ASSERT_EQ(config2.getMakeDecisionTimeout(),       30);
    ASSERT_EQ(config2.isUseSchedule(),                true);
    ASSERT_EQ(config2.getScheduleStartHour(),         7);
    ASSERT_EQ(config2.getScheduleStartMinute(),       5);
    ASSERT_EQ(config2.getScheduleEndHour(),           21);
    ASSERT_EQ(config2.getScheduleEndMinute(),         37);
    ASSERT_EQ(config2.isLimitPurchasesPerDay(),       true);
    ASSERT_EQ(config2.getAmountOfPurchasesPerDay(),   250);
    ASSERT_EQ(config2.isLimitPurchasesPerStock(),     true);
    ASSERT_EQ(config2.getAmountOfPurchasesPerStock(), 123);
    ASSERT_EQ(config2.getCommission(),                0.8f);
    ASSERT_EQ(config2.isLimitStockPurchase(),         true);
    ASSERT_EQ(config2.getAmountOfStockPurchase(),     75000);
    ASSERT_EQ(config2.getStorageMonthLimit(),         12);
    ASSERT_EQ(config2.isSimulatorConfigCommon(),      true);
    ASSERT_EQ(config2.isAutoPilotConfigCommon(),      false);
    // clang-format on
}

TEST(Test_Config, Test_makeDefault)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setAutorun(false);
    config.setMakeDecisionTimeout(5);
    config.setUseSchedule(false);
    config.setScheduleStartHour(8);
    config.setScheduleStartMinute(15);
    config.setScheduleEndHour(20);
    config.setScheduleEndMinute(50);
    config.setLimitPurchasesPerDay(false);
    config.setAmountOfPurchasesPerDay(100);
    config.setLimitPurchasesPerStock(false);
    config.setAmountOfPurchasesPerStock(25);
    config.setCommission(0.5f);
    config.setLimitStockPurchase(false);
    config.setAmountOfStockPurchase(50000);
    config.setStorageMonthLimit(36);
    config.setSimulatorConfigCommon(false);
    config.setAutoPilotConfigCommon(true);

    // clang-format off
    ASSERT_EQ(config.isAutorun(),                    false);
    ASSERT_EQ(config.getMakeDecisionTimeout(),       5);
    ASSERT_EQ(config.isUseSchedule(),                false);
    ASSERT_EQ(config.getScheduleStartHour(),         8);
    ASSERT_EQ(config.getScheduleStartMinute(),       15);
    ASSERT_EQ(config.getScheduleEndHour(),           20);
    ASSERT_EQ(config.getScheduleEndMinute(),         50);
    ASSERT_EQ(config.isLimitPurchasesPerDay(),       false);
    ASSERT_EQ(config.getAmountOfPurchasesPerDay(),   100);
    ASSERT_EQ(config.isLimitPurchasesPerStock(),     false);
    ASSERT_EQ(config.getAmountOfPurchasesPerStock(), 25);
    ASSERT_EQ(config.getCommission(),                0.5f);
    ASSERT_EQ(config.isLimitStockPurchase(),         false);
    ASSERT_EQ(config.getAmountOfStockPurchase(),     50000);
    ASSERT_EQ(config.getStorageMonthLimit(),         36);
    ASSERT_EQ(config.isSimulatorConfigCommon(),      false);
    ASSERT_EQ(config.isAutoPilotConfigCommon(),      true);
    // clang-format on

    EXPECT_CALL(simulatorConfigMock, makeDefault());
    EXPECT_CALL(autoPilotConfigMock, makeDefault());

    config.makeDefault();

    // clang-format off
    ASSERT_EQ(config.isAutorun(),                    true);
    ASSERT_EQ(config.getMakeDecisionTimeout(),       1);
    ASSERT_EQ(config.isUseSchedule(),                true);
    ASSERT_EQ(config.getScheduleStartHour(),         10);
    ASSERT_EQ(config.getScheduleStartMinute(),       0);
    ASSERT_EQ(config.getScheduleEndHour(),           18);
    ASSERT_EQ(config.getScheduleEndMinute(),         40);
    ASSERT_EQ(config.isLimitPurchasesPerDay(),       true);
    ASSERT_EQ(config.getAmountOfPurchasesPerDay(),   50);
    ASSERT_EQ(config.isLimitPurchasesPerStock(),     true);
    ASSERT_EQ(config.getAmountOfPurchasesPerStock(), 10);
    ASSERT_EQ(config.getCommission(),                0.3f);
    ASSERT_EQ(config.isLimitStockPurchase(),         true);
    ASSERT_EQ(config.getAmountOfStockPurchase(),     10000);
    ASSERT_EQ(config.getStorageMonthLimit(),         12);
    ASSERT_EQ(config.isSimulatorConfigCommon(),      true);
    ASSERT_EQ(config.isAutoPilotConfigCommon(),      false);
    // clang-format on
}

TEST(Test_Config, Test_save)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setAutorun(false);
    config.setMakeDecisionTimeout(5);
    config.setUseSchedule(false);
    config.setScheduleStartHour(8);
    config.setScheduleStartMinute(15);
    config.setScheduleEndHour(20);
    config.setScheduleEndMinute(50);
    config.setLimitPurchasesPerDay(false);
    config.setAmountOfPurchasesPerDay(100);
    config.setLimitPurchasesPerStock(false);
    config.setAmountOfPurchasesPerStock(25);
    config.setCommission(0.5f);
    config.setLimitStockPurchase(false);
    config.setAmountOfStockPurchase(50000);
    config.setStorageMonthLimit(36);
    config.setSimulatorConfigCommon(false);
    config.setAutoPilotConfigCommon(true);

    // clang-format off
    ASSERT_EQ(config.isAutorun(),                    false);
    ASSERT_EQ(config.getMakeDecisionTimeout(),       5);
    ASSERT_EQ(config.isUseSchedule(),                false);
    ASSERT_EQ(config.getScheduleStartHour(),         8);
    ASSERT_EQ(config.getScheduleStartMinute(),       15);
    ASSERT_EQ(config.getScheduleEndHour(),           20);
    ASSERT_EQ(config.getScheduleEndMinute(),         50);
    ASSERT_EQ(config.isLimitPurchasesPerDay(),       false);
    ASSERT_EQ(config.getAmountOfPurchasesPerDay(),   100);
    ASSERT_EQ(config.isLimitPurchasesPerStock(),     false);
    ASSERT_EQ(config.getAmountOfPurchasesPerStock(), 25);
    ASSERT_EQ(config.getCommission(),                0.5f);
    ASSERT_EQ(config.isLimitStockPurchase(),         false);
    ASSERT_EQ(config.getAmountOfStockPurchase(),     50000);
    ASSERT_EQ(config.getStorageMonthLimit(),         36);
    ASSERT_EQ(config.isSimulatorConfigCommon(),      false);
    ASSERT_EQ(config.isAutoPilotConfigCommon(),      true);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    EXPECT_CALL(simulatorConfigMock, save(&settingsEditorMock, QString("Simulator")));
    EXPECT_CALL(autoPilotConfigMock, save(&settingsEditorMock, QString("AutoPilot")));

    // clang-format off
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/Autorun"),                   QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/MakeDecisionTimeout"),       QVariant(5)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/UseSchedule"),               QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/ScheduleStartHour"),         QVariant(8)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/ScheduleStartMinute"),       QVariant(15)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/ScheduleEndHour"),           QVariant(20)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/ScheduleEndMinute"),         QVariant(50)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitPurchasesPerDay"),      QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/AmountOfPurchasesPerDay"),   QVariant(100)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitPurchasesPerStock"),    QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/AmountOfPurchasesPerStock"), QVariant(25)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/Commission"),                QVariant(0.5f)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/LimitStockPurchase"),        QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/AmountOfStockPurchase"),     QVariant(50000)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/StorageMonthLimit"),         QVariant(36)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/SimulatorConfigCommon"),     QVariant(false)));
    EXPECT_CALL(settingsEditorMock, setValue(QString("Config/AutoPilotConfigCommon"),     QVariant(true)));
    // clang-format on

    config.save(&settingsEditorMock);
}

TEST(Test_Config, Test_load)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setAutorun(false);
    config.setMakeDecisionTimeout(5);
    config.setUseSchedule(false);
    config.setScheduleStartHour(8);
    config.setScheduleStartMinute(15);
    config.setScheduleEndHour(20);
    config.setScheduleEndMinute(50);
    config.setLimitPurchasesPerDay(false);
    config.setAmountOfPurchasesPerDay(100);
    config.setLimitPurchasesPerStock(false);
    config.setAmountOfPurchasesPerStock(25);
    config.setCommission(0.5f);
    config.setLimitStockPurchase(false);
    config.setAmountOfStockPurchase(50000);
    config.setStorageMonthLimit(36);
    config.setSimulatorConfigCommon(false);
    config.setAutoPilotConfigCommon(true);

    // clang-format off
    ASSERT_EQ(config.isAutorun(),                    false);
    ASSERT_EQ(config.getMakeDecisionTimeout(),       5);
    ASSERT_EQ(config.isUseSchedule(),                false);
    ASSERT_EQ(config.getScheduleStartHour(),         8);
    ASSERT_EQ(config.getScheduleStartMinute(),       15);
    ASSERT_EQ(config.getScheduleEndHour(),           20);
    ASSERT_EQ(config.getScheduleEndMinute(),         50);
    ASSERT_EQ(config.isLimitPurchasesPerDay(),       false);
    ASSERT_EQ(config.getAmountOfPurchasesPerDay(),   100);
    ASSERT_EQ(config.isLimitPurchasesPerStock(),     false);
    ASSERT_EQ(config.getAmountOfPurchasesPerStock(), 25);
    ASSERT_EQ(config.getCommission(),                0.5f);
    ASSERT_EQ(config.isLimitStockPurchase(),         false);
    ASSERT_EQ(config.getAmountOfStockPurchase(),     50000);
    ASSERT_EQ(config.getStorageMonthLimit(),         36);
    ASSERT_EQ(config.isSimulatorConfigCommon(),      false);
    ASSERT_EQ(config.isAutoPilotConfigCommon(),      true);
    // clang-format on

    StrictMock<SettingsEditorMock> settingsEditorMock;

    EXPECT_CALL(simulatorConfigMock, load(&settingsEditorMock, QString("Simulator")));
    EXPECT_CALL(autoPilotConfigMock, load(&settingsEditorMock, QString("AutoPilot")));

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("Config/Autorun"),                   QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/MakeDecisionTimeout"),       QVariant(5))).WillOnce(Return(QVariant(30)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/UseSchedule"),               QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/ScheduleStartHour"),         QVariant(8))).WillOnce(Return(QVariant(7)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/ScheduleStartMinute"),       QVariant(15))).WillOnce(Return(QVariant(5)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/ScheduleEndHour"),           QVariant(20))).WillOnce(Return(QVariant(21)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/ScheduleEndMinute"),         QVariant(50))).WillOnce(Return(QVariant(37)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitPurchasesPerDay"),      QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/AmountOfPurchasesPerDay"),   QVariant(100))).WillOnce(Return(QVariant(250)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitPurchasesPerStock"),    QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/AmountOfPurchasesPerStock"), QVariant(25))).WillOnce(Return(QVariant(123)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/Commission"),                QVariant(0.5f))).WillOnce(Return(QVariant(0.8f)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/LimitStockPurchase"),        QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/AmountOfStockPurchase"),     QVariant(50000))).WillOnce(Return(QVariant(75000)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/StorageMonthLimit"),         QVariant(36))).WillOnce(Return(QVariant(12)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/SimulatorConfigCommon"),     QVariant(false))).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(settingsEditorMock, value(QString("Config/AutoPilotConfigCommon"),     QVariant(true))).WillOnce(Return(QVariant(false)));
    // clang-format on

    config.load(&settingsEditorMock);

    // clang-format off
    ASSERT_EQ(config.isAutorun(),                    true);
    ASSERT_EQ(config.getMakeDecisionTimeout(),       30);
    ASSERT_EQ(config.isUseSchedule(),                true);
    ASSERT_EQ(config.getScheduleStartHour(),         7);
    ASSERT_EQ(config.getScheduleStartMinute(),       5);
    ASSERT_EQ(config.getScheduleEndHour(),           21);
    ASSERT_EQ(config.getScheduleEndMinute(),         37);
    ASSERT_EQ(config.isLimitPurchasesPerDay(),       true);
    ASSERT_EQ(config.getAmountOfPurchasesPerDay(),   250);
    ASSERT_EQ(config.isLimitPurchasesPerStock(),     true);
    ASSERT_EQ(config.getAmountOfPurchasesPerStock(), 123);
    ASSERT_EQ(config.getCommission(),                0.8f);
    ASSERT_EQ(config.isLimitStockPurchase(),         true);
    ASSERT_EQ(config.getAmountOfStockPurchase(),     75000);
    ASSERT_EQ(config.getStorageMonthLimit(),         12);
    ASSERT_EQ(config.isSimulatorConfigCommon(),      true);
    ASSERT_EQ(config.isAutoPilotConfigCommon(),      false);
    // clang-format on
}

TEST(Test_Config, Test_getSimulatorConfig)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    ASSERT_EQ(config.getSimulatorConfig(), &simulatorConfigMock);
}

TEST(Test_Config, Test_getAutoPilotConfig)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    ASSERT_EQ(config.getAutoPilotConfig(), &autoPilotConfigMock);
}

TEST(Test_Config, Test_setAutorun_and_isAutorun)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setAutorun(false);
    ASSERT_EQ(config.isAutorun(), false);

    config.setAutorun(true);
    ASSERT_EQ(config.isAutorun(), true);
}

TEST(Test_Config, Test_setMakeDecisionTimeout_and_getMakeDecisionTimeout)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setMakeDecisionTimeout(1);
    ASSERT_EQ(config.getMakeDecisionTimeout(), 1);

    config.setMakeDecisionTimeout(2);
    ASSERT_EQ(config.getMakeDecisionTimeout(), 2);
}

TEST(Test_Config, Test_setUseSchedule_and_isUseSchedule)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setUseSchedule(false);
    ASSERT_EQ(config.isUseSchedule(), false);

    config.setUseSchedule(true);
    ASSERT_EQ(config.isUseSchedule(), true);
}

TEST(Test_Config, Test_setScheduleStartHour_and_getScheduleStartHour)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setScheduleStartHour(1);
    ASSERT_EQ(config.getScheduleStartHour(), 1);

    config.setScheduleStartHour(2);
    ASSERT_EQ(config.getScheduleStartHour(), 2);
}

TEST(Test_Config, Test_setScheduleStartMinute_and_getScheduleStartMinute)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setScheduleStartMinute(1);
    ASSERT_EQ(config.getScheduleStartMinute(), 1);

    config.setScheduleStartMinute(2);
    ASSERT_EQ(config.getScheduleStartMinute(), 2);
}

TEST(Test_Config, Test_setScheduleEndHour_and_getScheduleEndHour)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setScheduleEndHour(1);
    ASSERT_EQ(config.getScheduleEndHour(), 1);

    config.setScheduleEndHour(2);
    ASSERT_EQ(config.getScheduleEndHour(), 2);
}

TEST(Test_Config, Test_setScheduleEndMinute_and_getScheduleEndMinute)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setScheduleEndMinute(1);
    ASSERT_EQ(config.getScheduleEndMinute(), 1);

    config.setScheduleEndMinute(2);
    ASSERT_EQ(config.getScheduleEndMinute(), 2);
}

TEST(Test_Config, Test_setLimitPurchasesPerDay_and_isLimitPurchasesPerDay)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setLimitPurchasesPerDay(false);
    ASSERT_EQ(config.isLimitPurchasesPerDay(), false);

    config.setLimitPurchasesPerDay(true);
    ASSERT_EQ(config.isLimitPurchasesPerDay(), true);
}

TEST(Test_Config, Test_setAmountOfPurchasesPerDay_and_getAmountOfPurchasesPerDay)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setAmountOfPurchasesPerDay(1);
    ASSERT_EQ(config.getAmountOfPurchasesPerDay(), 1);

    config.setAmountOfPurchasesPerDay(2);
    ASSERT_EQ(config.getAmountOfPurchasesPerDay(), 2);
}

TEST(Test_Config, Test_setLimitPurchasesPerStock_and_isLimitPurchasesPerStock)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setLimitPurchasesPerStock(false);
    ASSERT_EQ(config.isLimitPurchasesPerStock(), false);

    config.setLimitPurchasesPerStock(true);
    ASSERT_EQ(config.isLimitPurchasesPerStock(), true);
}

TEST(Test_Config, Test_setAmountOfPurchasesPerStock_and_getAmountOfPurchasesPerStock)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setAmountOfPurchasesPerStock(1);
    ASSERT_EQ(config.getAmountOfPurchasesPerStock(), 1);

    config.setAmountOfPurchasesPerStock(2);
    ASSERT_EQ(config.getAmountOfPurchasesPerStock(), 2);
}

TEST(Test_Config, Test_setCommission_and_getCommission)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setCommission(0.1f);
    ASSERT_EQ(config.getCommission(), 0.1f);

    config.setCommission(0.2f);
    ASSERT_EQ(config.getCommission(), 0.2f);
}

TEST(Test_Config, Test_setLimitStockPurchase_and_isLimitStockPurchase)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setLimitStockPurchase(false);
    ASSERT_EQ(config.isLimitStockPurchase(), false);

    config.setLimitStockPurchase(true);
    ASSERT_EQ(config.isLimitStockPurchase(), true);
}

TEST(Test_Config, Test_setAmountOfStockPurchase_and_getAmountOfStockPurchase)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setAmountOfStockPurchase(false);
    ASSERT_EQ(config.getAmountOfStockPurchase(), false);

    config.setAmountOfStockPurchase(true);
    ASSERT_EQ(config.getAmountOfStockPurchase(), true);
}

TEST(Test_Config, Test_setStorageMonthLimit_and_getStorageMonthLimit)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setStorageMonthLimit(1);
    ASSERT_EQ(config.getStorageMonthLimit(), 1);

    config.setStorageMonthLimit(2);
    ASSERT_EQ(config.getStorageMonthLimit(), 2);
}

TEST(Test_Config, Test_setSimulatorConfigCommon_and_isSimulatorConfigCommon)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setSimulatorConfigCommon(false);
    ASSERT_EQ(config.isSimulatorConfigCommon(), false);

    config.setSimulatorConfigCommon(true);
    ASSERT_EQ(config.isSimulatorConfigCommon(), true);
}

TEST(Test_Config, Test_setAutoPilotConfigCommon_and_isAutoPilotConfigCommon)
{
    StrictMock<DecisionMakerConfigMock> simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> autoPilotConfigMock;

    Config config(&simulatorConfigMock, &autoPilotConfigMock);

    config.setAutoPilotConfigCommon(false);
    ASSERT_EQ(config.isAutoPilotConfigCommon(), false);

    config.setAutoPilotConfigCommon(true);
    ASSERT_EQ(config.isAutoPilotConfigCommon(), true);
}
