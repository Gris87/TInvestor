#pragma once



#include "src/config/decisions/sell/selldecision5config/iselldecision5config.h"

#include <gmock/gmock.h>



class SellDecision5ConfigMock : public ISellDecision5Config
{
public:
    SellDecision5ConfigMock() :
        ISellDecision5Config()
    {
    }
    ~SellDecision5ConfigMock() override = default;

    SellDecision5ConfigMock(const SellDecision5ConfigMock& another)            = delete;
    SellDecision5ConfigMock& operator=(const SellDecision5ConfigMock& another) = delete;

    MOCK_METHOD(ISellDecision5Config*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (ISellDecision5Config * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, fromJsonObject, (simdjson::ondemand::object jsonObject), (override));
    MOCK_METHOD(QString, toJsonString, (), (const, override));
    MOCK_METHOD(QStringList, variantsAsJson, (), (const, override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setLoseYield, (float value), (override));
    MOCK_METHOD(float, getLoseYield, (), (override));

    MOCK_METHOD(void, setDuration, (int value), (override));
    MOCK_METHOD(int, getDuration, (), (override));
};
