#pragma once



#include "src/config/decisions/sell/iselldecision2config.h"

#include <gmock/gmock.h>



class SellDecision2ConfigMock : public ISellDecision2Config
{
public:
    SellDecision2ConfigMock() {}
    ~SellDecision2ConfigMock() override = default;

    SellDecision2ConfigMock(const SellDecision2ConfigMock &another) = delete;
    SellDecision2ConfigMock& operator=(const SellDecision2ConfigMock &another) = delete;

    MOCK_METHOD1(assign, void(ISellDecision2Config *another));
    MOCK_METHOD0(makeDefault, void());

    MOCK_METHOD1(save, void(const QString &type));
    MOCK_METHOD1(load, void(const QString &type));

    MOCK_METHOD1(setEnabled, void(bool value));
    MOCK_METHOD0(isEnabled, bool());

    MOCK_METHOD1(setIncomeAbove, void(float value));
    MOCK_METHOD0(getIncomeAbove, float());

    MOCK_METHOD1(setLoseIncome, void(float value));
    MOCK_METHOD0(getLoseIncome, float());
};
