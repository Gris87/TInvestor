#include "decisionmakerconfig.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



DecisionMakerConfig::DecisionMakerConfig()
{
    qDebug() << "Create DecisionMakerConfig";
}

DecisionMakerConfig::~DecisionMakerConfig()
{
    qDebug() << "Destroy DecisionMakerConfig";
}

DecisionMakerConfig& DecisionMakerConfig::operator=(const DecisionMakerConfig &config)
{
    assign(config);

    return *this;
}

void DecisionMakerConfig::makeDefault()
{
    qDebug() << "Set DecisionMakerConfig to default";

    buyDecision1Config.makeDefault();
    buyDecision2Config.makeDefault();
    buyDecision3Config.makeDefault();
    sellDecision1Config.makeDefault();
    sellDecision2Config.makeDefault();
    sellDecision3Config.makeDefault();
}

void DecisionMakerConfig::assign(const DecisionMakerConfig &config)
{
    qDebug() << "Assigning DecisionMakerConfig to DecisionMakerConfig";

    buyDecision1Config  = config.buyDecision1Config;
    buyDecision2Config  = config.buyDecision2Config;
    buyDecision3Config  = config.buyDecision3Config;
    sellDecision1Config = config.sellDecision1Config;
    sellDecision2Config = config.sellDecision2Config;
    sellDecision3Config = config.sellDecision3Config;
}

void DecisionMakerConfig::save(const QString &type)
{
    qDebug() << "Save DecisionMakerConfig";

    buyDecision1Config.save(type + "/BuyDecision1Config");
    buyDecision2Config.save(type + "/BuyDecision2Config");
    buyDecision3Config.save(type + "/BuyDecision3Config");
    sellDecision1Config.save(type + "/SellDecision1Config");
    sellDecision2Config.save(type + "/SellDecision2Config");
    sellDecision3Config.save(type + "/SellDecision3Config");
}

void DecisionMakerConfig::load(const QString &type)
{
    qDebug() << "Load DecisionMakerConfig";

    buyDecision1Config.load(type + "/BuyDecision1Config");
    buyDecision2Config.load(type + "/BuyDecision2Config");
    buyDecision3Config.load(type + "/BuyDecision3Config");
    sellDecision1Config.load(type + "/SellDecision1Config");
    sellDecision2Config.load(type + "/SellDecision2Config");
    sellDecision3Config.load(type + "/SellDecision3Config");
}
