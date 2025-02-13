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

    MOCK_METHOD(void, assign, (IDecisionMakerConfig * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(IBuyDecision1Config*, getBuyDecision1Config, (), (override));
    MOCK_METHOD(IBuyDecision2Config*, getBuyDecision2Config, (), (override));
    MOCK_METHOD(IBuyDecision3Config*, getBuyDecision3Config, (), (override));

    MOCK_METHOD(ISellDecision1Config*, getSellDecision1Config, (), (override));
    MOCK_METHOD(ISellDecision2Config*, getSellDecision2Config, (), (override));
    MOCK_METHOD(ISellDecision3Config*, getSellDecision3Config, (), (override));
};
