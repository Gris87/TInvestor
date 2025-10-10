#pragma once



#include "src/config/decisions/buy/buydecision9config/ibuydecision9config.h"

#include <gmock/gmock.h>



class BuyDecision9ConfigMock : public IBuyDecision9Config
{
public:
    BuyDecision9ConfigMock() :
        IBuyDecision9Config()
    {
    }
    ~BuyDecision9ConfigMock() override = default;

    BuyDecision9ConfigMock(const BuyDecision9ConfigMock& another)            = delete;
    BuyDecision9ConfigMock& operator=(const BuyDecision9ConfigMock& another) = delete;

    MOCK_METHOD(IBuyDecision9Config*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (IBuyDecision9Config * another), (override));
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

    MOCK_METHOD(void, setOrderBookPositions, (int value), (override));
    MOCK_METHOD(int, getOrderBookPositions, (), (override));

    MOCK_METHOD(void, setDuration, (int value), (override));
    MOCK_METHOD(int, getDuration, (), (override));
};
