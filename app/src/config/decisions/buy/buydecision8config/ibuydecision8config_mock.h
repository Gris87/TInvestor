#pragma once



#include "src/config/decisions/buy/buydecision8config/ibuydecision8config.h"

#include <gmock/gmock.h>



class BuyDecision8ConfigMock : public IBuyDecision8Config
{
public:
    BuyDecision8ConfigMock() :
        IBuyDecision8Config()
    {
    }
    ~BuyDecision8ConfigMock() override = default;

    BuyDecision8ConfigMock(const BuyDecision8ConfigMock& another)            = delete;
    BuyDecision8ConfigMock& operator=(const BuyDecision8ConfigMock& another) = delete;

    MOCK_METHOD(IBuyDecision8Config*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (IBuyDecision8Config * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

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
