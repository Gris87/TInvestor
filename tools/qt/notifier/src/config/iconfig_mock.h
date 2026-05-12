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

    MOCK_METHOD(void, setAutorun, (bool value), (override));
    MOCK_METHOD(bool, isAutorun, (), (override));

    MOCK_METHOD(void, setServerAddress, (const QString& value), (override));
    MOCK_METHOD(QString, getServerAddress, (), (override));

    MOCK_METHOD(void, setServerPort, (int value), (override));
    MOCK_METHOD(int, getServerPort, (), (override));

    MOCK_METHOD(void, setNotificationsEnabled, (bool value), (override));
    MOCK_METHOD(bool, isNotificationsEnabled, (), (override));

    MOCK_METHOD(void, setFilterSystem, (bool value), (override));
    MOCK_METHOD(bool, isFilterSystem, (), (override));

    MOCK_METHOD(void, setFilterPortfolio, (bool value), (override));
    MOCK_METHOD(bool, isFilterPortfolio, (), (override));

    MOCK_METHOD(void, setFilterHugeSell, (bool value), (override));
    MOCK_METHOD(bool, isFilterHugeSell, (), (override));

    MOCK_METHOD(void, setFilterDividends, (bool value), (override));
    MOCK_METHOD(bool, isFilterDividends, (), (override));

    MOCK_METHOD(void, setFilterPulseNeutral, (bool value), (override));
    MOCK_METHOD(bool, isFilterPulseNeutral, (), (override));

    MOCK_METHOD(void, setFilterPulseBuy, (bool value), (override));
    MOCK_METHOD(bool, isFilterPulseBuy, (), (override));

    MOCK_METHOD(void, setFilterPulseSell, (bool value), (override));
    MOCK_METHOD(bool, isFilterPulseSell, (), (override));
};
