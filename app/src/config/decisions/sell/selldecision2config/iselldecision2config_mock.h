#pragma once



#include "src/config/decisions/sell/selldecision2config/iselldecision2config.h"

#include <gmock/gmock.h>



class SellDecision2ConfigMock : public ISellDecision2Config
{
public:
    SellDecision2ConfigMock() : ISellDecision2Config() {}
    ~SellDecision2ConfigMock() override = default;

    SellDecision2ConfigMock(const SellDecision2ConfigMock &another) = delete;
    SellDecision2ConfigMock& operator=(const SellDecision2ConfigMock &another) = delete;

    MOCK_METHOD1(assign, void(ISellDecision2Config *another));
    MOCK_METHOD0(makeDefault, void());

    MOCK_METHOD2(save, void(ISettingsEditor *settingsEditor, const QString &type));
    MOCK_METHOD2(load, void(ISettingsEditor *settingsEditor, const QString &type));

    MOCK_METHOD1(setEnabled, void(bool value));
    MOCK_METHOD0(isEnabled, bool());

    MOCK_METHOD1(setIncomeAbove, void(float value));
    MOCK_METHOD0(getIncomeAbove, float());

    MOCK_METHOD1(setLoseIncome, void(float value));
    MOCK_METHOD0(getLoseIncome, float());
};
