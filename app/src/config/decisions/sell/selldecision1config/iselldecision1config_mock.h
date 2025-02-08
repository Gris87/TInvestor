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

    MOCK_METHOD1(assign, void(ISellDecision1Config* another));
    MOCK_METHOD0(makeDefault, void());

    MOCK_METHOD2(save, void(ISettingsEditor* settingsEditor, const QString& type));
    MOCK_METHOD2(load, void(ISettingsEditor* settingsEditor, const QString& type));

    MOCK_METHOD1(setEnabled, void(bool value));
    MOCK_METHOD0(isEnabled, bool());

    MOCK_METHOD1(setIncomeAbove, void(float value));
    MOCK_METHOD0(getIncomeAbove, float());
};
