#pragma once



#include "src/config/decisions/buy/ibuydecision1config.h"
#include "src/config/decisions/buy/ibuydecision2config.h"
#include "src/config/decisions/buy/ibuydecision3config.h"
#include "src/config/decisions/sell/iselldecision1config.h"
#include "src/config/decisions/sell/iselldecision2config.h"
#include "src/config/decisions/sell/iselldecision3config.h"



class IDecisionMakerConfig
{
public:
    IDecisionMakerConfig() {}
    virtual ~IDecisionMakerConfig() = default;

    IDecisionMakerConfig(const IDecisionMakerConfig &another) = delete;
    IDecisionMakerConfig& operator=(const IDecisionMakerConfig &another) = delete;

    virtual void assign(IDecisionMakerConfig *another) = 0;
    virtual void makeDefault() = 0;

    virtual void save(const QString &type) = 0;
    virtual void load(const QString &type) = 0;

    virtual IBuyDecision1Config* getBuyDecision1Config() = 0;
    virtual IBuyDecision2Config* getBuyDecision2Config() = 0;
    virtual IBuyDecision3Config* getBuyDecision3Config() = 0;

    virtual ISellDecision1Config* getSellDecision1Config() = 0;
    virtual ISellDecision2Config* getSellDecision2Config() = 0;
    virtual ISellDecision3Config* getSellDecision3Config() = 0;
};
