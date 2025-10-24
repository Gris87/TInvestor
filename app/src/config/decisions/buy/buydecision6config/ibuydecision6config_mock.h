#pragma once



#include "src/config/decisions/buy/buydecision6config/ibuydecision6config.h"

#include <gmock/gmock.h>



class BuyDecision6ConfigMock : public IBuyDecision6Config
{
public:
    BuyDecision6ConfigMock() :
        IBuyDecision6Config()
    {
    }
    ~BuyDecision6ConfigMock() override = default;

    BuyDecision6ConfigMock(const BuyDecision6ConfigMock& another)            = delete;
    BuyDecision6ConfigMock& operator=(const BuyDecision6ConfigMock& another) = delete;

    MOCK_METHOD(IBuyDecision6Config*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (IBuyDecision6Config * another), (override));
    MOCK_METHOD(void, makeDefault, (int commissionInt), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, fromJsonObject, (simdjson::ondemand::object jsonObject), (override));
    MOCK_METHOD(QString, toJsonString, (), (const, override));
    MOCK_METHOD(QStringList, variantsAsJson, (), (const, override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setRsi, (float value), (override));
    MOCK_METHOD(float, getRsi, (), (override));

    MOCK_METHOD(void, setDuration, (int value), (override));
    MOCK_METHOD(int, getDuration, (), (override));
};
