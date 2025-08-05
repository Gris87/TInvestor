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

    MOCK_METHOD(IDecisionMakerConfig*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (IDecisionMakerConfig * another), (override));
    MOCK_METHOD(void, makeDefault, (), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor, const QString& type), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor, const QString& type), (override));

    MOCK_METHOD(void, fromJsonObject, (simdjson::ondemand::object jsonObject), (override));
    MOCK_METHOD(QString, toJsonString, (), (const, override));
    MOCK_METHOD(QStringList, variantsToJsonStringList, (), (const, override));
    MOCK_METHOD(QString, variantsToJsonStringListExtendedBySellDecisions, (const QStringList& bestConfigs), (const, override));

    MOCK_METHOD(IBuyDecision1Config*, getBuyDecision1Config, (), (override));
    MOCK_METHOD(IBuyDecision2Config*, getBuyDecision2Config, (), (override));
    MOCK_METHOD(IBuyDecision3Config*, getBuyDecision3Config, (), (override));
    MOCK_METHOD(IBuyDecision4Config*, getBuyDecision4Config, (), (override));

    MOCK_METHOD(ISellDecision1Config*, getSellDecision1Config, (), (override));
    MOCK_METHOD(ISellDecision2Config*, getSellDecision2Config, (), (override));
    MOCK_METHOD(ISellDecision3Config*, getSellDecision3Config, (), (override));
    MOCK_METHOD(ISellDecision4Config*, getSellDecision4Config, (), (override));
};
