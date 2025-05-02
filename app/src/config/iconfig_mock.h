#pragma once



#include "src/config/iconfig.h"

#include <gmock/gmock.h>



class ConfigMock : public IConfig
{
public:
    ConfigMock() :
        IConfig()
    {
    }
    ~ConfigMock() override = default;

    ConfigMock(const ConfigMock& another)            = delete;
    ConfigMock& operator=(const ConfigMock& another) = delete;

    MOCK_METHOD(void, assign, (IConfig * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor), (override));

    MOCK_METHOD(IDecisionMakerConfig*, getSimulatorConfig, (), (override));
    MOCK_METHOD(IDecisionMakerConfig*, getAutoPilotConfig, (), (override));

    MOCK_METHOD(void, setAutorun, (bool value), (override));
    MOCK_METHOD(bool, isAutorun, (), (override));

    MOCK_METHOD(void, setMakeDecisionTimeout, (int value), (override));
    MOCK_METHOD(int, getMakeDecisionTimeout, (), (override));

    MOCK_METHOD(void, setUseSchedule, (bool value), (override));
    MOCK_METHOD(bool, isUseSchedule, (), (override));

    MOCK_METHOD(void, setScheduleStartHour, (int value), (override));
    MOCK_METHOD(int, getScheduleStartHour, (), (override));

    MOCK_METHOD(void, setScheduleStartMinute, (int value), (override));
    MOCK_METHOD(int, getScheduleStartMinute, (), (override));

    MOCK_METHOD(void, setScheduleEndHour, (int value), (override));
    MOCK_METHOD(int, getScheduleEndHour, (), (override));

    MOCK_METHOD(void, setScheduleEndMinute, (int value), (override));
    MOCK_METHOD(int, getScheduleEndMinute, (), (override));

    MOCK_METHOD(void, setLimitStockPurchase, (bool value), (override));
    MOCK_METHOD(bool, isLimitStockPurchase, (), (override));

    MOCK_METHOD(void, setAmountOfStockPurchase, (int value), (override));
    MOCK_METHOD(int, getAmountOfStockPurchase, (), (override));

    MOCK_METHOD(void, setLimitByTurnover, (bool value), (override));
    MOCK_METHOD(bool, isLimitByTurnover, (), (override));

    MOCK_METHOD(void, setLimitByTurnoverPercent, (float value), (override));
    MOCK_METHOD(float, getLimitByTurnoverPercent, (), (override));

    MOCK_METHOD(void, setStorageMonthLimit, (int value), (override));
    MOCK_METHOD(int, getStorageMonthLimit, (), (override));

    MOCK_METHOD(void, setSimulatorConfigCommon, (bool value), (override));
    MOCK_METHOD(bool, isSimulatorConfigCommon, (), (override));

    MOCK_METHOD(void, setAutoPilotConfigCommon, (bool value), (override));
    MOCK_METHOD(bool, isAutoPilotConfigCommon, (), (override));
};
