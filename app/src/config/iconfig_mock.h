#pragma once



#include "src/config/iconfig.h"

#include <gmock/gmock.h>



class ConfigMock : public IConfig
{
public:
    ConfigMock() {}
    ~ConfigMock() override = default;

    ConfigMock(const ConfigMock &another) = delete;
    ConfigMock& operator=(const ConfigMock &another) = delete;

    MOCK_METHOD1(assign, void(IConfig *another));
    MOCK_METHOD0(makeDefault, void());

    MOCK_METHOD0(save, void());
    MOCK_METHOD0(load, void());

    MOCK_METHOD0(getSimulatorConfig, IDecisionMakerConfig*());
    MOCK_METHOD0(getAutoPilotConfig, IDecisionMakerConfig*());

    MOCK_METHOD1(setAutorun, void(bool value));
    MOCK_METHOD0(isAutorun, bool());

    MOCK_METHOD1(setRefreshTimeout, void(int value));
    MOCK_METHOD0(getRefreshTimeout, int());

    MOCK_METHOD1(setUseSchedule, void(bool value));
    MOCK_METHOD0(isUseSchedule, bool());

    MOCK_METHOD1(setScheduleStartHour, void(int value));
    MOCK_METHOD0(getScheduleStartHour, int());

    MOCK_METHOD1(setScheduleStartMinute, void(int value));
    MOCK_METHOD0(getScheduleStartMinute, int());

    MOCK_METHOD1(setScheduleEndHour, void(int value));
    MOCK_METHOD0(getScheduleEndHour, int());

    MOCK_METHOD1(setScheduleEndMinute, void(int value));
    MOCK_METHOD0(getScheduleEndMinute, int());

    MOCK_METHOD1(setLimitPurchasesPerDay, void(bool value));
    MOCK_METHOD0(isLimitPurchasesPerDay, bool());

    MOCK_METHOD1(setAmountOfPurchasesPerDay, void(int value));
    MOCK_METHOD0(getAmountOfPurchasesPerDay, int());

    MOCK_METHOD1(setLimitPurchasesPerStock, void(bool value));
    MOCK_METHOD0(isLimitPurchasesPerStock, bool());

    MOCK_METHOD1(setAmountOfPurchasesPerStock, void(int value));
    MOCK_METHOD0(getAmountOfPurchasesPerStock, int());

    MOCK_METHOD1(setCommission, void(float value));
    MOCK_METHOD0(getCommission, float());

    MOCK_METHOD1(setLimitStockPurchase, void(bool value));
    MOCK_METHOD0(isLimitStockPurchase, bool());

    MOCK_METHOD1(setAmountOfStockPurchase, void(int value));
    MOCK_METHOD0(getAmountOfStockPurchase, int());

    MOCK_METHOD1(setStorageMonthLimit, void(int value));
    MOCK_METHOD0(getStorageMonthLimit, int());

    MOCK_METHOD1(setSimulatorConfigCommon, void(bool value));
    MOCK_METHOD0(isSimulatorConfigCommon, bool());

    MOCK_METHOD1(setAutoPilotConfigCommon, void(bool value));
    MOCK_METHOD0(isAutoPilotConfigCommon, bool());
};
