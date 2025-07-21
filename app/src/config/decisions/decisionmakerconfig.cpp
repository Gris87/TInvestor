#include "src/config/decisions/decisionmakerconfig.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/utils/exception/exception.h"



DecisionMakerConfig::DecisionMakerConfig(
    IBuyDecision1Config*  buyDecision1Config,
    IBuyDecision2Config*  buyDecision2Config,
    IBuyDecision3Config*  buyDecision3Config,
    ISellDecision1Config* sellDecision1Config,
    ISellDecision2Config* sellDecision2Config,
    ISellDecision3Config* sellDecision3Config
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

void DecisionMakerConfig::assign(IDecisionMakerConfig* another)
{
    qDebug() << "Assigning DecisionMakerConfig to DecisionMakerConfig";

    const DecisionMakerConfig& config = *dynamic_cast<DecisionMakerConfig*>(another);

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

void DecisionMakerConfig::save(ISettingsEditor* settingsEditor, const QString& type)
{
    qDebug() << "Save DecisionMakerConfig";

    mBuyDecision1Config->save(settingsEditor, type + "/BuyDecision1Config");
    mBuyDecision2Config->save(settingsEditor, type + "/BuyDecision2Config");
    mBuyDecision3Config->save(settingsEditor, type + "/BuyDecision3Config");
    mSellDecision1Config->save(settingsEditor, type + "/SellDecision1Config");
    mSellDecision2Config->save(settingsEditor, type + "/SellDecision2Config");
    mSellDecision3Config->save(settingsEditor, type + "/SellDecision3Config");
}

void DecisionMakerConfig::load(ISettingsEditor* settingsEditor, const QString& type)
{
    qDebug() << "Load DecisionMakerConfig";

    mBuyDecision1Config->load(settingsEditor, type + "/BuyDecision1Config");
    mBuyDecision2Config->load(settingsEditor, type + "/BuyDecision2Config");
    mBuyDecision3Config->load(settingsEditor, type + "/BuyDecision3Config");
    mSellDecision1Config->load(settingsEditor, type + "/SellDecision1Config");
    mSellDecision2Config->load(settingsEditor, type + "/SellDecision2Config");
    mSellDecision3Config->load(settingsEditor, type + "/SellDecision3Config");
}

static void configBuy1Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision1Config()->fromJsonObject(value.get_object());
}

static void configBuy2Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision2Config()->fromJsonObject(value.get_object());
}

static void configBuy3Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision3Config()->fromJsonObject(value.get_object());
}

static void configSell1Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getSellDecision1Config()->fromJsonObject(value.get_object());
}

static void configSell2Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getSellDecision2Config()->fromJsonObject(value.get_object());
}

static void configSell3Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getSellDecision3Config()->fromJsonObject(value.get_object());
}

static void configThrowParseException(
    DecisionMakerConfig* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(DecisionMakerConfig* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"b1", configBuy1Parse },
    {"b2", configBuy2Parse },
    {"b3", configBuy3Parse },
    {"s1", configSell1Parse},
    {"s2", configSell2Parse},
    {"s3", configSell3Parse}
};
// clang-format on

void DecisionMakerConfig::fromJsonObject(simdjson::ondemand::object jsonObject)
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString DecisionMakerConfig::toJsonString() const
{
    return QString(R"({"b1":%1,"b2":%2,"b3":%3,"s1":%4,"s2":%5,"s3":%6})")
        .arg(
            mBuyDecision1Config->toJsonString(),
            mBuyDecision2Config->toJsonString(),
            mBuyDecision3Config->toJsonString(),
            mSellDecision1Config->toJsonString(),
            mSellDecision2Config->toJsonString(),
            mSellDecision3Config->toJsonString()
        );
}

QString DecisionMakerConfig::variantsToJsonString() const
{
    const QString res = "[]";

    return res;
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
