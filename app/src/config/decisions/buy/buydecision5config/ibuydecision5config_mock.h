#pragma once



#include "src/config/decisions/buy/buydecision5config/ibuydecision5config.h"

#include <gmock/gmock.h>



class BuyDecision5ConfigMock : public IBuyDecision5Config
{
public:
    BuyDecision5ConfigMock() :
        IBuyDecision5Config()
    {
    }
    ~BuyDecision5ConfigMock() override = default;

    BuyDecision5ConfigMock(const BuyDecision5ConfigMock& another)            = delete;
    BuyDecision5ConfigMock& operator=(const BuyDecision5ConfigMock& another) = delete;

    MOCK_METHOD(IBuyDecision5Config*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (IBuyDecision5Config * another), (override));
    MOCK_METHOD(void, makeDefault, (int commissionInt), (override));
    MOCK_METHOD(bool, isDefault, (int commissionInt), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, fromJsonObject, (simdjson::ondemand::object jsonObject), (override));
    MOCK_METHOD(QString, toJsonString, (), (const, override));
    MOCK_METHOD(QStringList, variantsAsJson, (), (const, override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setDuration, (int value), (override));
    MOCK_METHOD(int, getDuration, (), (override));
};
