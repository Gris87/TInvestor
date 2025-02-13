#pragma once



#include "src/config/decisions/sell/selldecision2config/iselldecision2config.h"

#include <gmock/gmock.h>



class SellDecision2ConfigMock : public ISellDecision2Config
{
public:
    SellDecision2ConfigMock() :
        ISellDecision2Config()
    {
    }
    ~SellDecision2ConfigMock() override = default;

    SellDecision2ConfigMock(const SellDecision2ConfigMock& another)            = delete;
    SellDecision2ConfigMock& operator=(const SellDecision2ConfigMock& another) = delete;

    MOCK_METHOD(void, assign, (ISellDecision2Config * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setIncomeAbove, (float value), (override));
    MOCK_METHOD(float, getIncomeAbove, (), (override));

    MOCK_METHOD(void, setLoseIncome, (float value), (override));
    MOCK_METHOD(float, getLoseIncome, (), (override));
};
