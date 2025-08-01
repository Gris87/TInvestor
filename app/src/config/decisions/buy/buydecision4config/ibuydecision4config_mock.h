#pragma once



#include "src/config/decisions/buy/buydecision4config/ibuydecision4config.h"

#include <gmock/gmock.h>



class BuyDecision4ConfigMock : public IBuyDecision4Config
{
public:
    BuyDecision4ConfigMock() :
        IBuyDecision4Config()
    {
    }
    ~BuyDecision4ConfigMock() override = default;

    BuyDecision4ConfigMock(const BuyDecision4ConfigMock& another)            = delete;
    BuyDecision4ConfigMock& operator=(const BuyDecision4ConfigMock& another) = delete;

    MOCK_METHOD(void, assign, (IBuyDecision4Config * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

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
