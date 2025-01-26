#pragma once



#include "src/config/decisions/idecisionmakerconfig.h"



class IDecisionMakerConfigFactory
{
public:
    virtual ~IDecisionMakerConfigFactory() = default;

    virtual IDecisionMakerConfig* newInstance() = 0;
};
