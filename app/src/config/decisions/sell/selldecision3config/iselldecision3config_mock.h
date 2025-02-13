#pragma once



#include "src/config/decisions/sell/selldecision3config/iselldecision3config.h"

#include <gmock/gmock.h>



class SellDecision3ConfigMock : public ISellDecision3Config
{
public:
    SellDecision3ConfigMock() :
        ISellDecision3Config()
    {
    }
    ~SellDecision3ConfigMock() override = default;

    SellDecision3ConfigMock(const SellDecision3ConfigMock& another)            = delete;
    SellDecision3ConfigMock& operator=(const SellDecision3ConfigMock& another) = delete;

    MOCK_METHOD(void, assign, (ISellDecision3Config * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setLoseIncome, (float value), (override));
    MOCK_METHOD(float, getLoseIncome, (), (override));

    MOCK_METHOD(void, setDuration, (int value), (override));
    MOCK_METHOD(int, getDuration, (), (override));
};
