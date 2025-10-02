#pragma once



#include "src/config/decisions/buy/buydecision7config/ibuydecision7config.h"

#include <gmock/gmock.h>



class BuyDecision7ConfigMock : public IBuyDecision7Config
{
public:
    BuyDecision7ConfigMock() :
        IBuyDecision7Config()
    {
    }
    ~BuyDecision7ConfigMock() override = default;

    BuyDecision7ConfigMock(const BuyDecision7ConfigMock& another)            = delete;
    BuyDecision7ConfigMock& operator=(const BuyDecision7ConfigMock& another) = delete;

    MOCK_METHOD(IBuyDecision7Config*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (IBuyDecision7Config * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, fromJsonObject, (simdjson::ondemand::object jsonObject), (override));
    MOCK_METHOD(QString, toJsonString, (), (const, override));
    MOCK_METHOD(QStringList, variantsAsJson, (), (const, override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setPriceRaise, (float value), (override));
    MOCK_METHOD(float, getPriceRaise, (), (override));

    MOCK_METHOD(void, setDuration, (int value), (override));
    MOCK_METHOD(int, getDuration, (), (override));
};
