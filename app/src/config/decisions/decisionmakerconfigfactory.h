#pragma once



#include "src/config/decisions/idecisionmakerconfigfactory.h"

#include "src/config/decisions/idecisionmakerconfig.h"



class DecisionMakerConfigFactory : public IDecisionMakerConfigFactory
{
public:
    DecisionMakerConfigFactory();
    ~DecisionMakerConfigFactory();

    IDecisionMakerConfig* newInstance() override;
};
