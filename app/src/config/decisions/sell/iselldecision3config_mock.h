#pragma once



#include "src/config/decisions/sell/iselldecision3config.h"

#include <gmock/gmock.h>



class SellDecision3ConfigMock : public ISellDecision3Config
{
public:
    SellDecision3ConfigMock() {}
    ~SellDecision3ConfigMock() override = default;

    SellDecision3ConfigMock(const SellDecision3ConfigMock &another) = delete;
    SellDecision3ConfigMock& operator=(const SellDecision3ConfigMock &another) = delete;

    MOCK_METHOD1(assign, void(ISellDecision3Config *another));
    MOCK_METHOD0(makeDefault, void());

    MOCK_METHOD1(save, void(const QString &type));
    MOCK_METHOD1(load, void(const QString &type));

    MOCK_METHOD1(setEnabled, void(bool value));
    MOCK_METHOD0(isEnabled, bool());

    MOCK_METHOD1(setLoseIncome, void(float value));
    MOCK_METHOD0(getLoseIncome, float());

    MOCK_METHOD1(setDuration, void(int value));
    MOCK_METHOD0(getDuration, int());
};
