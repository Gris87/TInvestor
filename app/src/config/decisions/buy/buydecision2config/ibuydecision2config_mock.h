#pragma once



#include "src/config/decisions/buy/ibuydecision2config.h"

#include <gmock/gmock.h>



class BuyDecision2ConfigMock : public IBuyDecision2Config
{
public:
    BuyDecision2ConfigMock() : IBuyDecision2Config() {}
    ~BuyDecision2ConfigMock() override = default;

    BuyDecision2ConfigMock(const BuyDecision2ConfigMock &another) = delete;
    BuyDecision2ConfigMock& operator=(const BuyDecision2ConfigMock &another) = delete;

    MOCK_METHOD1(assign, void(IBuyDecision2Config *another));
    MOCK_METHOD0(makeDefault, void());

    MOCK_METHOD2(save, void(ISettingsEditor *settingsEditor, const QString &type));
    MOCK_METHOD2(load, void(ISettingsEditor *settingsEditor, const QString &type));

    MOCK_METHOD1(setEnabled, void(bool value));
    MOCK_METHOD0(isEnabled, bool());

    MOCK_METHOD1(setPriceDiff, void(float value));
    MOCK_METHOD0(getPriceDiff, float());

    MOCK_METHOD1(setAmountOfTimes, void(int value));
    MOCK_METHOD0(getAmountOfTimes, int());

    MOCK_METHOD1(setDuration, void(int value));
    MOCK_METHOD0(getDuration, int());
};
