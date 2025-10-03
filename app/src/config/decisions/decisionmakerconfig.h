#pragma once



#include "src/config/decisions/idecisionmakerconfig.h"



class DecisionMakerConfig : public IDecisionMakerConfig
{
public:
    DecisionMakerConfig(
        IBuyDecision1Config*  buyDecision1Config,
        IBuyDecision2Config*  buyDecision2Config,
        IBuyDecision3Config*  buyDecision3Config,
        IBuyDecision4Config*  buyDecision4Config,
        IBuyDecision5Config*  buyDecision5Config,
        IBuyDecision6Config*  buyDecision6Config,
        IBuyDecision7Config*  buyDecision7Config,
        IBuyDecision8Config*  buyDecision8Config,
        ISellDecision1Config* sellDecision1Config,
        ISellDecision2Config* sellDecision2Config,
        ISellDecision3Config* sellDecision3Config,
        ISellDecision4Config* sellDecision4Config,
        ISellDecision5Config* sellDecision5Config
    );
    ~DecisionMakerConfig() override;

    DecisionMakerConfig(const DecisionMakerConfig& another)            = delete;
    DecisionMakerConfig& operator=(const DecisionMakerConfig& another) = delete;

    IDecisionMakerConfig* clone() override;
    void                  deleteRecursively() override;
    void                  assign(IDecisionMakerConfig* another) override;
    void                  makeDefault() override;

    void save(ISettingsEditor* settingsEditor, const QString& type) override;
    void load(ISettingsEditor* settingsEditor, const QString& type) override;

    void fromJsonObject(simdjson::ondemand::object jsonObject) override; // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QString toJsonString() const override;

    [[nodiscard]]
    QStringList variantsToJsonStringList() const override;

    [[nodiscard]]
    QString variantsToJsonStringListExtendedBySellDecisions(const QStringList& bestConfigs) const override;

    IBuyDecision1Config*  getBuyDecision1Config() override;
    IBuyDecision2Config*  getBuyDecision2Config() override;
    IBuyDecision3Config*  getBuyDecision3Config() override;
    IBuyDecision4Config*  getBuyDecision4Config() override;
    IBuyDecision5Config*  getBuyDecision5Config() override;
    IBuyDecision6Config*  getBuyDecision6Config() override;
    IBuyDecision7Config*  getBuyDecision7Config() override;
    IBuyDecision8Config*  getBuyDecision8Config() override;
    ISellDecision1Config* getSellDecision1Config() override;
    ISellDecision2Config* getSellDecision2Config() override;
    ISellDecision3Config* getSellDecision3Config() override;
    ISellDecision4Config* getSellDecision4Config() override;
    ISellDecision5Config* getSellDecision5Config() override;

private:
    IBuyDecision1Config*  mBuyDecision1Config;
    IBuyDecision2Config*  mBuyDecision2Config;
    IBuyDecision3Config*  mBuyDecision3Config;
    IBuyDecision4Config*  mBuyDecision4Config;
    IBuyDecision5Config*  mBuyDecision5Config;
    IBuyDecision6Config*  mBuyDecision6Config;
    IBuyDecision7Config*  mBuyDecision7Config;
    IBuyDecision8Config*  mBuyDecision8Config;
    ISellDecision1Config* mSellDecision1Config;
    ISellDecision2Config* mSellDecision2Config;
    ISellDecision3Config* mSellDecision3Config;
    ISellDecision4Config* mSellDecision4Config;
    ISellDecision5Config* mSellDecision5Config;
};
