#ifndef DECISIONMAKERCONFIG_H
#define DECISIONMAKERCONFIG_H

#include "src/config/decisions/idecisionmakerconfig.h"

#include <QMutex>

#include "src/config/decisions/buy/buydecision1config.h"
#include "src/config/decisions/buy/buydecision2config.h"
#include "src/config/decisions/buy/buydecision3config.h"
#include "src/config/decisions/sell/selldecision1config.h"
#include "src/config/decisions/sell/selldecision2config.h"
#include "src/config/decisions/sell/selldecision3config.h"



class DecisionMakerConfig : public IDecisionMakerConfig
{
public:
    DecisionMakerConfig();
    ~DecisionMakerConfig();

    BuyDecision1Config  buyDecision1Config;
    BuyDecision2Config  buyDecision2Config;
    BuyDecision3Config  buyDecision3Config;
    SellDecision1Config sellDecision1Config;
    SellDecision2Config sellDecision2Config;
    SellDecision3Config sellDecision3Config;

    DecisionMakerConfig& operator=(const DecisionMakerConfig &config);

    void makeDefault() override;

    void save(const QString &type) override;
    void load(const QString &type) override;

private:
    void assign(const DecisionMakerConfig &config);
};



#endif // DECISIONMAKERCONFIG_H
