#pragma once



#include "src/config/iconfigfactory.h"



class ConfigFactory : public IConfigFactory
{
public:
    ConfigFactory();
    ~ConfigFactory();

    IConfig* newInstance(IDecisionMakerConfigFactory *decisionMakerConfigFactory, QObject *parent = nullptr) override;
};
