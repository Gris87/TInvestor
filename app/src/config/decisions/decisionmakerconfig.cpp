#include "decisionmakerconfig.h"

#include <QDebug>
#include <QMutexLocker>



DecisionMakerConfig::DecisionMakerConfig(
    IBuyDecision1Config *buyDecision1Config,
    IBuyDecision2Config *buyDecision2Config,
    IBuyDecision3Config *buyDecision3Config,
    ISellDecision1Config *sellDecision1Config,
    ISellDecision2Config *sellDecision2Config,
    ISellDecision3Config *sellDecision3Config
) :
    IDecisionMakerConfig(),
    mBuyDecision1Config(buyDecision1Config),
    mBuyDecision2Config(buyDecision2Config),
    mBuyDecision3Config(buyDecision3Config),
    mSellDecision1Config(sellDecision1Config),
    mSellDecision2Config(sellDecision2Config),
    mSellDecision3Config(sellDecision3Config)
{
    qDebug() << "Create DecisionMakerConfig";
}

DecisionMakerConfig::~DecisionMakerConfig()
{
    qDebug() << "Destroy DecisionMakerConfig";
}

void DecisionMakerConfig::assign(IDecisionMakerConfig *another)
{
    qDebug() << "Assigning DecisionMakerConfig to DecisionMakerConfig";

    const DecisionMakerConfig &config = *dynamic_cast<DecisionMakerConfig *>(another);

    mBuyDecision1Config->assign(config.mBuyDecision1Config);
    mBuyDecision2Config->assign(config.mBuyDecision2Config);
    mBuyDecision3Config->assign(config.mBuyDecision3Config);
    mSellDecision1Config->assign(config.mSellDecision1Config);
    mSellDecision2Config->assign(config.mSellDecision2Config);
    mSellDecision3Config->assign(config.mSellDecision3Config);
}

void DecisionMakerConfig::makeDefault()
{
    qDebug() << "Set DecisionMakerConfig to default";

    mBuyDecision1Config->makeDefault();
    mBuyDecision2Config->makeDefault();
    mBuyDecision3Config->makeDefault();
    mSellDecision1Config->makeDefault();
    mSellDecision2Config->makeDefault();
    mSellDecision3Config->makeDefault();
}

void DecisionMakerConfig::save(ISettingsEditor *settingsEditor, const QString &type)
{
    qDebug() << "Save DecisionMakerConfig";

    mBuyDecision1Config->save(settingsEditor, type + "/BuyDecision1Config");
    mBuyDecision2Config->save(settingsEditor, type + "/BuyDecision2Config");
    mBuyDecision3Config->save(settingsEditor, type + "/BuyDecision3Config");
    mSellDecision1Config->save(settingsEditor, type + "/SellDecision1Config");
    mSellDecision2Config->save(settingsEditor, type + "/SellDecision2Config");
    mSellDecision3Config->save(settingsEditor, type + "/SellDecision3Config");
}

void DecisionMakerConfig::load(ISettingsEditor *settingsEditor, const QString &type)
{
    qDebug() << "Load DecisionMakerConfig";

    mBuyDecision1Config->load(settingsEditor, type + "/BuyDecision1Config");
    mBuyDecision2Config->load(settingsEditor, type + "/BuyDecision2Config");
    mBuyDecision3Config->load(settingsEditor, type + "/BuyDecision3Config");
    mSellDecision1Config->load(settingsEditor, type + "/SellDecision1Config");
    mSellDecision2Config->load(settingsEditor, type + "/SellDecision2Config");
    mSellDecision3Config->load(settingsEditor, type + "/SellDecision3Config");
}

IBuyDecision1Config* DecisionMakerConfig::getBuyDecision1Config()
{
    return mBuyDecision1Config;
}

IBuyDecision2Config* DecisionMakerConfig::getBuyDecision2Config()
{
    return mBuyDecision2Config;
}

IBuyDecision3Config* DecisionMakerConfig::getBuyDecision3Config()
{
    return mBuyDecision3Config;
}

ISellDecision1Config* DecisionMakerConfig::getSellDecision1Config()
{
    return mSellDecision1Config;
}

ISellDecision2Config* DecisionMakerConfig::getSellDecision2Config()
{
    return mSellDecision2Config;
}

ISellDecision3Config* DecisionMakerConfig::getSellDecision3Config()
{
    return mSellDecision3Config;
}
