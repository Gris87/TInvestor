#ifndef DECISIONMAKERCONFIG_H
#define DECISIONMAKERCONFIG_H

#include <QMutex>

#include "src/config/decisions/buy/buydecision1config.h"
#include "src/config/decisions/buy/buydecision2config.h"
#include "src/config/decisions/buy/buydecision3config.h"
#include "src/config/decisions/sell/selldecision1config.h"
#include "src/config/decisions/sell/selldecision2config.h"
#include "src/config/decisions/sell/selldecision3config.h"



class DecisionMakerConfig
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

    void makeDefault();

    void save(const QString &type);
    void load(const QString &type);

private:
    void assign(const DecisionMakerConfig &config);
};



#endif // DECISIONMAKERCONFIG_H
