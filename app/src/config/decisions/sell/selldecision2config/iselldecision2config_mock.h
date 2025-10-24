#pragma once



#include "src/config/decisions/sell/selldecision2config/iselldecision2config.h"

#include <gmock/gmock.h>



class SellDecision2ConfigMock : public ISellDecision2Config
{
public:
    SellDecision2ConfigMock() :
        ISellDecision2Config()
    {
    }
    ~SellDecision2ConfigMock() override = default;

    SellDecision2ConfigMock(const SellDecision2ConfigMock& another)            = delete;
    SellDecision2ConfigMock& operator=(const SellDecision2ConfigMock& another) = delete;

    MOCK_METHOD(ISellDecision2Config*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (ISellDecision2Config * another), (override));
    MOCK_METHOD(void, makeDefault, (int commissionInt), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, fromJsonObject, (simdjson::ondemand::object jsonObject), (override));
    MOCK_METHOD(QString, toJsonString, (), (const, override));
    MOCK_METHOD(QStringList, variantsAsJson, (), (const, override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setYieldAbove, (float value), (override));
    MOCK_METHOD(float, getYieldAbove, (), (override));

    MOCK_METHOD(void, setLoseYield, (float value), (override));
    MOCK_METHOD(float, getLoseYield, (), (override));
};
