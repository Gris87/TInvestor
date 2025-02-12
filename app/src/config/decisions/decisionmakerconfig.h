#pragma once



#include "src/config/decisions/idecisionmakerconfig.h"

#include <QMutex>



class DecisionMakerConfig : public IDecisionMakerConfig
{
public:
    DecisionMakerConfig(
        IBuyDecision1Config*  buyDecision1Config,
        IBuyDecision2Config*  buyDecision2Config,
        IBuyDecision3Config*  buyDecision3Config,
        ISellDecision1Config* sellDecision1Config,
        ISellDecision2Config* sellDecision2Config,
        ISellDecision3Config* sellDecision3Config
    );
    ~DecisionMakerConfig();

    DecisionMakerConfig(const DecisionMakerConfig& another)            = delete;
    DecisionMakerConfig& operator=(const DecisionMakerConfig& another) = delete;

    void assign(IDecisionMakerConfig* another) override;
    void makeDefault() override;

    void save(ISettingsEditor* settingsEditor, const QString& type) override;
    void load(ISettingsEditor* settingsEditor, const QString& type) override;

    IBuyDecision1Config*  getBuyDecision1Config() override;
    IBuyDecision2Config*  getBuyDecision2Config() override;
    IBuyDecision3Config*  getBuyDecision3Config() override;
    ISellDecision1Config* getSellDecision1Config() override;
    ISellDecision2Config* getSellDecision2Config() override;
    ISellDecision3Config* getSellDecision3Config() override;

private:
    IBuyDecision1Config*  mBuyDecision1Config;
    IBuyDecision2Config*  mBuyDecision2Config;
    IBuyDecision3Config*  mBuyDecision3Config;
    ISellDecision1Config* mSellDecision1Config;
    ISellDecision2Config* mSellDecision2Config;
    ISellDecision3Config* mSellDecision3Config;
};
