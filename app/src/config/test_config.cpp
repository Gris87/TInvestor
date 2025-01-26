#include <gtest/gtest.h>

#include "config.h"



TEST(Test_Config, TestConstructor) {
    Config cfg;

    EXPECT_EQ(cfg.isAutorun(),                    true);
    EXPECT_EQ(cfg.getRefreshTimeout(),            1);
    EXPECT_EQ(cfg.isUseSchedule(),                true);
    EXPECT_EQ(cfg.getScheduleStartHour(),         10);
    EXPECT_EQ(cfg.getScheduleStartMinute(),       0);
    EXPECT_EQ(cfg.getScheduleEndHour(),           18);
    EXPECT_EQ(cfg.getScheduleEndMinute(),         40);
    EXPECT_EQ(cfg.isLimitPurchasesPerDay(),       true);
    EXPECT_EQ(cfg.getAmountOfPurchasesPerDay(),   50);
    EXPECT_EQ(cfg.isLimitPurchasesPerStock(),     true);
    EXPECT_EQ(cfg.getAmountOfPurchasesPerStock(), 10);
    EXPECT_EQ(cfg.getCommission(),                0.3f);
    EXPECT_EQ(cfg.isLimitStockPurchase(),         true);
    EXPECT_EQ(cfg.getAmountOfStockPurchase(),     10000);
    EXPECT_EQ(cfg.getStorageMonthLimit(),         24);
    EXPECT_EQ(cfg.isSimulatorConfigCommon(),      true);
    EXPECT_EQ(cfg.isAutoPilotConfigCommon(),      false);
}
