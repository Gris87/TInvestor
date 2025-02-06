#pragma once



#include "src/config/decisions/buy/buydecision1config/ibuydecision1config.h"

#include <gmock/gmock.h>



class BuyDecision1ConfigMock : public IBuyDecision1Config
{
public:
    BuyDecision1ConfigMock() : IBuyDecision1Config() {}
    ~BuyDecision1ConfigMock() override = default;

    BuyDecision1ConfigMock(const BuyDecision1ConfigMock &another) = delete;
    BuyDecision1ConfigMock& operator=(const BuyDecision1ConfigMock &another) = delete;

    MOCK_METHOD1(assign, void(IBuyDecision1Config *another));
    MOCK_METHOD0(makeDefault, void());

    MOCK_METHOD2(save, void(ISettingsEditor *settingsEditor, const QString &type));
    MOCK_METHOD2(load, void(ISettingsEditor *settingsEditor, const QString &type));

    MOCK_METHOD1(setEnabled, void(bool value));
    MOCK_METHOD0(isEnabled, bool());

    MOCK_METHOD1(setPriceFall, void(float value));
    MOCK_METHOD0(getPriceFall, float());

    MOCK_METHOD1(setDuration, void(int value));
    MOCK_METHOD0(getDuration, int());
};
