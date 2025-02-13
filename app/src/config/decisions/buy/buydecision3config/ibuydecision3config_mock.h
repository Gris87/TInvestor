#pragma once



#include "src/config/decisions/buy/buydecision3config/ibuydecision3config.h"

#include <gmock/gmock.h>



class BuyDecision3ConfigMock : public IBuyDecision3Config
{
public:
    BuyDecision3ConfigMock() :
        IBuyDecision3Config()
    {
    }
    ~BuyDecision3ConfigMock() override = default;

    BuyDecision3ConfigMock(const BuyDecision3ConfigMock& another)            = delete;
    BuyDecision3ConfigMock& operator=(const BuyDecision3ConfigMock& another) = delete;

    MOCK_METHOD(void, assign, (IBuyDecision3Config * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setPriceRise, (float value), (override));
    MOCK_METHOD(float, getPriceRise, (), (override));

    MOCK_METHOD(void, setDuration, (int value), (override));
    MOCK_METHOD(int, getDuration, (), (override));
};
