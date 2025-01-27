#include <gtest/gtest.h>

#include "src/config/config.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



TEST(Test_Config, TestConstructor) {
    DecisionMakerConfigMock simulatorConfigMock;
    DecisionMakerConfigMock autoPilotConfigMock;

    Config cfg(&simulatorConfigMock, &autoPilotConfigMock);

    ASSERT_EQ(cfg.isAutorun(),                    true);
    ASSERT_EQ(cfg.getRefreshTimeout(),            1);
    ASSERT_EQ(cfg.isUseSchedule(),                true);
    ASSERT_EQ(cfg.getScheduleStartHour(),         10);
    ASSERT_EQ(cfg.getScheduleStartMinute(),       0);
    ASSERT_EQ(cfg.getScheduleEndHour(),           18);
    ASSERT_EQ(cfg.getScheduleEndMinute(),         40);
    ASSERT_EQ(cfg.isLimitPurchasesPerDay(),       true);
    ASSERT_EQ(cfg.getAmountOfPurchasesPerDay(),   50);
    ASSERT_EQ(cfg.isLimitPurchasesPerStock(),     true);
    ASSERT_EQ(cfg.getAmountOfPurchasesPerStock(), 10);
    ASSERT_EQ(cfg.getCommission(),                0.3f);
    ASSERT_EQ(cfg.isLimitStockPurchase(),         true);
    ASSERT_EQ(cfg.getAmountOfStockPurchase(),     10000);
    ASSERT_EQ(cfg.getStorageMonthLimit(),         24);
    ASSERT_EQ(cfg.isSimulatorConfigCommon(),      true);
    ASSERT_EQ(cfg.isAutoPilotConfigCommon(),      false);
}
