#pragma once



#include "src/config/decisions/sell/selldecision4config/iselldecision4config.h"

#include <gmock/gmock.h>



class SellDecision4ConfigMock : public ISellDecision4Config
{
public:
    SellDecision4ConfigMock() :
        ISellDecision4Config()
    {
    }
    ~SellDecision4ConfigMock() override = default;

    SellDecision4ConfigMock(const SellDecision4ConfigMock& another)            = delete;
    SellDecision4ConfigMock& operator=(const SellDecision4ConfigMock& another) = delete;

    MOCK_METHOD(ISellDecision4Config*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (ISellDecision4Config * another), (override));
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
};
