#pragma once



#include "src/config/decisions/sell/selldecision1config/iselldecision1config.h"

#include <gmock/gmock.h>



class SellDecision1ConfigMock : public ISellDecision1Config
{
public:
    SellDecision1ConfigMock() :
        ISellDecision1Config()
    {
    }
    ~SellDecision1ConfigMock() override = default;

    SellDecision1ConfigMock(const SellDecision1ConfigMock& another)            = delete;
    SellDecision1ConfigMock& operator=(const SellDecision1ConfigMock& another) = delete;

    MOCK_METHOD(void, assign, (ISellDecision1Config * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, setEnabled, (bool value), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));

    MOCK_METHOD(void, setIncomeAbove, (float value), (override));
    MOCK_METHOD(float, getIncomeAbove, (), (override));
};
