#pragma once



#include "src/config/decisions/buy/ibuydecision3config.h"

#include <gmock/gmock.h>



class BuyDecision3ConfigMock : public IBuyDecision3Config
{
public:
    BuyDecision3ConfigMock() {}
    ~BuyDecision3ConfigMock() override = default;

    BuyDecision3ConfigMock(const BuyDecision3ConfigMock &another) = delete;
    BuyDecision3ConfigMock& operator=(const BuyDecision3ConfigMock &another) = delete;

    MOCK_METHOD1(assign, void(IBuyDecision3Config *another));
    MOCK_METHOD0(makeDefault, void());

    MOCK_METHOD1(save, void(const QString &type));
    MOCK_METHOD1(load, void(const QString &type));

    MOCK_METHOD1(setEnabled, void(bool value));
    MOCK_METHOD0(isEnabled, bool());

    MOCK_METHOD1(setPriceRise, void(float value));
    MOCK_METHOD0(getPriceRise, float());

    MOCK_METHOD1(setDuration, void(int value));
    MOCK_METHOD0(getDuration, int());
};
