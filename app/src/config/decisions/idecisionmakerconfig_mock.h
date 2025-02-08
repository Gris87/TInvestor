#pragma once



#include "src/config/decisions/idecisionmakerconfig.h"

#include <gmock/gmock.h>



class DecisionMakerConfigMock : public IDecisionMakerConfig
{
public:
    DecisionMakerConfigMock() :
        IDecisionMakerConfig()
    {
    }
    ~DecisionMakerConfigMock() override = default;

    DecisionMakerConfigMock(const DecisionMakerConfigMock& another)            = delete;
    DecisionMakerConfigMock& operator=(const DecisionMakerConfigMock& another) = delete;

    MOCK_METHOD1(assign, void(IDecisionMakerConfig* another));
    MOCK_METHOD0(makeDefault, void());

    MOCK_METHOD2(save, void(ISettingsEditor* settingsEditor, const QString& type));
    MOCK_METHOD2(load, void(ISettingsEditor* settingsEditor, const QString& type));

    MOCK_METHOD0(getBuyDecision1Config, IBuyDecision1Config*());
    MOCK_METHOD0(getBuyDecision2Config, IBuyDecision2Config*());
    MOCK_METHOD0(getBuyDecision3Config, IBuyDecision3Config*());

    MOCK_METHOD0(getSellDecision1Config, ISellDecision1Config*());
    MOCK_METHOD0(getSellDecision2Config, ISellDecision2Config*());
    MOCK_METHOD0(getSellDecision3Config, ISellDecision3Config*());
};
