#pragma once



#include <gmock/gmock.h>

#include "src/config/decisions/idecisionmakerconfig.h"



class DecisionMakerConfigMock : public IDecisionMakerConfig
{
public:
    ~DecisionMakerConfigMock() override = default;

    MOCK_METHOD1(assign, void(IDecisionMakerConfig *another));
    MOCK_METHOD0(makeDefault, void());
    MOCK_METHOD1(save, void(const QString &type));
    MOCK_METHOD1(load, void(const QString &type));
};
