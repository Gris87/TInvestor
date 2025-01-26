#pragma once



#include <gmock/gmock.h>

#include "src/config/decisions/idecisionmakerconfigfactory.h"



class DecisionMakerConfigFactoryMock : public IDecisionMakerConfigFactory
{
public:
    ~DecisionMakerConfigFactoryMock() override = default;

    MOCK_METHOD0(newInstance, IDecisionMakerConfig*());
};
