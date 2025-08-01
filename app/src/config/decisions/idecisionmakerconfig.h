#pragma once



#include <Qt>

#include "src/config/decisions/buy/buydecision1config/ibuydecision1config.h"
#include "src/config/decisions/buy/buydecision2config/ibuydecision2config.h"
#include "src/config/decisions/buy/buydecision3config/ibuydecision3config.h"
#include "src/config/decisions/buy/buydecision4config/ibuydecision4config.h"
#include "src/config/decisions/sell/selldecision1config/iselldecision1config.h"
#include "src/config/decisions/sell/selldecision2config/iselldecision2config.h"
#include "src/config/decisions/sell/selldecision3config/iselldecision3config.h"
#include "src/config/decisions/sell/selldecision4config/iselldecision4config.h"
#include "src/simdjson/simdjson_wrapped.h"



class IDecisionMakerConfig
{
public:
    IDecisionMakerConfig()          = default;
    virtual ~IDecisionMakerConfig() = default;

    IDecisionMakerConfig(const IDecisionMakerConfig& another)            = delete;
    IDecisionMakerConfig& operator=(const IDecisionMakerConfig& another) = delete;

    virtual void assign(IDecisionMakerConfig* another) = 0;
    virtual void makeDefault()                         = 0;

    virtual void save(ISettingsEditor* settingsEditor, const QString& type) = 0;
    virtual void load(ISettingsEditor* settingsEditor, const QString& type) = 0;

    virtual void fromJsonObject(simdjson::ondemand::object jsonObject) = 0; // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    virtual QString toJsonString() const = 0;

    [[nodiscard]]
    virtual QString variantsToJsonString() const = 0;

    virtual IBuyDecision1Config* getBuyDecision1Config() = 0;
    virtual IBuyDecision2Config* getBuyDecision2Config() = 0;
    virtual IBuyDecision3Config* getBuyDecision3Config() = 0;
    virtual IBuyDecision4Config* getBuyDecision4Config() = 0;

    virtual ISellDecision1Config* getSellDecision1Config() = 0;
    virtual ISellDecision2Config* getSellDecision2Config() = 0;
    virtual ISellDecision3Config* getSellDecision3Config() = 0;
    virtual ISellDecision4Config* getSellDecision4Config() = 0;
};
