#pragma once



#include "src/config/decisions/buy/buydecision1config/ibuydecision1config.h"

#include <gmock/gmock.h>



class BuyDecision1ConfigMock : public IBuyDecision1Config
{
public:
    BuyDecision1ConfigMock() :
        IBuyDecision1Config()
    {
    }
    ~BuyDecision1ConfigMock() override = default;

    BuyDecision1ConfigMock(const BuyDecision1ConfigMock& another)            = delete;
    BuyDecision1ConfigMock& operator=(const BuyDecision1ConfigMock& another) = delete;

    MOCK_METHOD(IBuyDecision1Config*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (IBuyDecision1Config * another), (override));
    MOCK_METHOD(void, makeDefault, (int commissionInt), (override));
    MOCK_METHOD(bool, isDefault, (int commissionInt), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, fromJsonObject, (simdjson::ondemand::object jsonObject), (override));
    MOCK_METHOD(QString, toJsonString, (), (const, override));
    MOCK_METHOD(QStringList, variantsAsJson, (), (const, override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setPriceFall, (float value), (override));
    MOCK_METHOD(float, getPriceFall, (), (override));

    MOCK_METHOD(void, setDuration, (int value), (override));
    MOCK_METHOD(int, getDuration, (), (override));
};
