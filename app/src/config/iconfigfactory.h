#pragma once



#include "src/config/iconfig.h"
#include "src/config/decisions/idecisionmakerconfigfactory.h"



class IConfigFactory
{
public:
    virtual ~IConfigFactory() = default;

    virtual IConfig* newInstance(IDecisionMakerConfigFactory *decisionMakerConfigFactory, QObject *parent = nullptr) = 0;
};
