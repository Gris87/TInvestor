#pragma once



#include "src/config/decisions/buy/buydecision2config/ibuydecision2config.h"

#include <gmock/gmock.h>



class BuyDecision2ConfigMock : public IBuyDecision2Config
{
public:
    BuyDecision2ConfigMock() :
        IBuyDecision2Config()
    {
    }
    ~BuyDecision2ConfigMock() override = default;

    BuyDecision2ConfigMock(const BuyDecision2ConfigMock& another)            = delete;
    BuyDecision2ConfigMock& operator=(const BuyDecision2ConfigMock& another) = delete;

    MOCK_METHOD(IBuyDecision2Config*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (IBuyDecision2Config * another), (override));
    MOCK_METHOD(void, makeDefault, (int commissionInt), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, fromJsonObject, (simdjson::ondemand::object jsonObject), (override));
    MOCK_METHOD(QString, toJsonString, (), (const, override));
    MOCK_METHOD(QStringList, variantsAsJson, (), (const, override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setPriceFall, (float value), (override));
    MOCK_METHOD(float, getPriceFall, (), (override));

    MOCK_METHOD(void, setLoseYield, (float value), (override));
    MOCK_METHOD(float, getLoseYield, (), (override));

    MOCK_METHOD(void, setDuration, (int value), (override));
    MOCK_METHOD(int, getDuration, (), (override));
};
