#include "src/config/decisions/decisionmakerconfig.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



DecisionMakerConfig::DecisionMakerConfig(
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
    ISellDecision4Config* sellDecision4Config
) :
    IDecisionMakerConfig(),
    mBuyDecision1Config(buyDecision1Config),
    mBuyDecision2Config(buyDecision2Config),
    mBuyDecision3Config(buyDecision3Config),
    mBuyDecision4Config(buyDecision4Config),
    mBuyDecision5Config(buyDecision5Config),
    mBuyDecision6Config(buyDecision6Config),
    mBuyDecision7Config(buyDecision7Config),
    mBuyDecision8Config(buyDecision8Config),
    mSellDecision1Config(sellDecision1Config),
    mSellDecision2Config(sellDecision2Config),
    mSellDecision3Config(sellDecision3Config),
    mSellDecision4Config(sellDecision4Config)
{
    qDebug() << "Create DecisionMakerConfig";
}

DecisionMakerConfig::~DecisionMakerConfig()
{
    qDebug() << "Destroy DecisionMakerConfig";
}

IDecisionMakerConfig* DecisionMakerConfig::clone()
{
    IBuyDecision1Config*  buyDecision1Config  = mBuyDecision1Config->clone();
    IBuyDecision2Config*  buyDecision2Config  = mBuyDecision2Config->clone();
    IBuyDecision3Config*  buyDecision3Config  = mBuyDecision3Config->clone();
    IBuyDecision4Config*  buyDecision4Config  = mBuyDecision4Config->clone();
    IBuyDecision5Config*  buyDecision5Config  = mBuyDecision5Config->clone();
    IBuyDecision6Config*  buyDecision6Config  = mBuyDecision6Config->clone();
    IBuyDecision7Config*  buyDecision7Config  = mBuyDecision7Config->clone();
    IBuyDecision8Config*  buyDecision8Config  = mBuyDecision8Config->clone();
    ISellDecision1Config* sellDecision1Config = mSellDecision1Config->clone();
    ISellDecision2Config* sellDecision2Config = mSellDecision2Config->clone();
    ISellDecision3Config* sellDecision3Config = mSellDecision3Config->clone();
    ISellDecision4Config* sellDecision4Config = mSellDecision4Config->clone();

    DecisionMakerConfig* res = new DecisionMakerConfig(
        buyDecision1Config,
        buyDecision2Config,
        buyDecision3Config,
        buyDecision4Config,
        buyDecision5Config,
        buyDecision6Config,
        buyDecision7Config,
        buyDecision8Config,
        sellDecision1Config,
        sellDecision2Config,
        sellDecision3Config,
        sellDecision4Config
    );
    res->assign(this);

    return res;
}

void DecisionMakerConfig::deleteRecursively()
{
    mBuyDecision1Config->deleteRecursively();
    mBuyDecision2Config->deleteRecursively();
    mBuyDecision3Config->deleteRecursively();
    mBuyDecision4Config->deleteRecursively();
    mBuyDecision5Config->deleteRecursively();
    mBuyDecision6Config->deleteRecursively();
    mBuyDecision7Config->deleteRecursively();
    mBuyDecision8Config->deleteRecursively();
    mSellDecision1Config->deleteRecursively();
    mSellDecision2Config->deleteRecursively();
    mSellDecision3Config->deleteRecursively();
    mSellDecision4Config->deleteRecursively();

    delete this;
}

void DecisionMakerConfig::assign(IDecisionMakerConfig* another)
{
    qDebug() << "Assigning DecisionMakerConfig to DecisionMakerConfig";

    const DecisionMakerConfig& config = *dynamic_cast<DecisionMakerConfig*>(another);

    mBuyDecision1Config->assign(config.mBuyDecision1Config);
    mBuyDecision2Config->assign(config.mBuyDecision2Config);
    mBuyDecision3Config->assign(config.mBuyDecision3Config);
    mBuyDecision4Config->assign(config.mBuyDecision4Config);
    mBuyDecision5Config->assign(config.mBuyDecision5Config);
    mBuyDecision6Config->assign(config.mBuyDecision6Config);
    mBuyDecision7Config->assign(config.mBuyDecision7Config);
    mBuyDecision8Config->assign(config.mBuyDecision8Config);
    mSellDecision1Config->assign(config.mSellDecision1Config);
    mSellDecision2Config->assign(config.mSellDecision2Config);
    mSellDecision3Config->assign(config.mSellDecision3Config);
    mSellDecision4Config->assign(config.mSellDecision4Config);
}

void DecisionMakerConfig::makeDefault()
{
    qDebug() << "Set DecisionMakerConfig to default";

    mBuyDecision1Config->makeDefault();
    mBuyDecision2Config->makeDefault();
    mBuyDecision3Config->makeDefault();
    mBuyDecision4Config->makeDefault();
    mBuyDecision5Config->makeDefault();
    mBuyDecision6Config->makeDefault();
    mBuyDecision7Config->makeDefault();
    mBuyDecision8Config->makeDefault();
    mSellDecision1Config->makeDefault();
    mSellDecision2Config->makeDefault();
    mSellDecision3Config->makeDefault();
    mSellDecision4Config->makeDefault();
}

void DecisionMakerConfig::save(ISettingsEditor* settingsEditor, const QString& type)
{
    qDebug() << "Save DecisionMakerConfig";

    mBuyDecision1Config->save(settingsEditor, type + "/BuyDecision1Config");
    mBuyDecision2Config->save(settingsEditor, type + "/BuyDecision2Config");
    mBuyDecision3Config->save(settingsEditor, type + "/BuyDecision3Config");
    mBuyDecision4Config->save(settingsEditor, type + "/BuyDecision4Config");
    mBuyDecision5Config->save(settingsEditor, type + "/BuyDecision5Config");
    mBuyDecision6Config->save(settingsEditor, type + "/BuyDecision6Config");
    mBuyDecision7Config->save(settingsEditor, type + "/BuyDecision7Config");
    mBuyDecision8Config->save(settingsEditor, type + "/BuyDecision8Config");
    mSellDecision1Config->save(settingsEditor, type + "/SellDecision1Config");
    mSellDecision2Config->save(settingsEditor, type + "/SellDecision2Config");
    mSellDecision3Config->save(settingsEditor, type + "/SellDecision3Config");
    mSellDecision4Config->save(settingsEditor, type + "/SellDecision4Config");
}

void DecisionMakerConfig::load(ISettingsEditor* settingsEditor, const QString& type)
{
    qDebug() << "Load DecisionMakerConfig";

    mBuyDecision1Config->load(settingsEditor, type + "/BuyDecision1Config");
    mBuyDecision2Config->load(settingsEditor, type + "/BuyDecision2Config");
    mBuyDecision3Config->load(settingsEditor, type + "/BuyDecision3Config");
    mBuyDecision4Config->load(settingsEditor, type + "/BuyDecision4Config");
    mBuyDecision5Config->load(settingsEditor, type + "/BuyDecision5Config");
    mBuyDecision6Config->load(settingsEditor, type + "/BuyDecision6Config");
    mBuyDecision7Config->load(settingsEditor, type + "/BuyDecision7Config");
    mBuyDecision8Config->load(settingsEditor, type + "/BuyDecision8Config");
    mSellDecision1Config->load(settingsEditor, type + "/SellDecision1Config");
    mSellDecision2Config->load(settingsEditor, type + "/SellDecision2Config");
    mSellDecision3Config->load(settingsEditor, type + "/SellDecision3Config");
    mSellDecision4Config->load(settingsEditor, type + "/SellDecision4Config");
}

static void configBuyDecision1Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision1Config()->fromJsonObject(value.get_object());
}

static void configBuyDecision2Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision2Config()->fromJsonObject(value.get_object());
}

static void configBuyDecision3Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision3Config()->fromJsonObject(value.get_object());
}

static void configBuyDecision4Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision4Config()->fromJsonObject(value.get_object());
}

static void configBuyDecision5Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision5Config()->fromJsonObject(value.get_object());
}

static void configBuyDecision6Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision6Config()->fromJsonObject(value.get_object());
}

static void configBuyDecision7Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision7Config()->fromJsonObject(value.get_object());
}

static void configBuyDecision8Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision8Config()->fromJsonObject(value.get_object());
}

static void configSellDecision1Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getSellDecision1Config()->fromJsonObject(value.get_object());
}

static void configSellDecision2Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getSellDecision2Config()->fromJsonObject(value.get_object());
}

static void configSellDecision3Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getSellDecision3Config()->fromJsonObject(value.get_object());
}

static void configSellDecision4Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getSellDecision4Config()->fromJsonObject(value.get_object());
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
    {"b1", configBuyDecision1Parse },
    {"b2", configBuyDecision2Parse },
    {"b3", configBuyDecision3Parse },
    {"b4", configBuyDecision4Parse },
    {"b5", configBuyDecision5Parse },
    {"b6", configBuyDecision6Parse },
    {"b7", configBuyDecision7Parse },
    {"b8", configBuyDecision8Parse },
    {"s1", configSellDecision1Parse},
    {"s2", configSellDecision2Parse},
    {"s3", configSellDecision3Parse},
    {"s4", configSellDecision4Parse}
};
// clang-format on

void DecisionMakerConfig::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
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
    const QString b1 = mBuyDecision1Config->toJsonString();
    const QString b2 = mBuyDecision2Config->toJsonString();
    const QString b3 = mBuyDecision3Config->toJsonString();
    const QString b4 = mBuyDecision4Config->toJsonString();
    const QString b5 = mBuyDecision5Config->toJsonString();
    const QString b6 = mBuyDecision6Config->toJsonString();
    const QString b7 = mBuyDecision7Config->toJsonString();
    const QString b8 = mBuyDecision8Config->toJsonString();

    const QString s1 = mSellDecision1Config->toJsonString();
    const QString s2 = mSellDecision2Config->toJsonString();
    const QString s3 = mSellDecision3Config->toJsonString();
    const QString s4 = mSellDecision4Config->toJsonString();

    return QString("{%1,%2}").arg(
        QString(R"("b1":%1,"b2":%2,"b3":%3,"b4":%4,"b5":%5,"b6":%6,"b7":%7,"b8":%8)").arg(b1, b2, b3, b4, b5, b6, b7, b8),
        QString(R"("s1":%1,"s2":%2,"s3":%3,"s4":%4)").arg(s1, s2, s3, s4)
    );
}

QStringList DecisionMakerConfig::variantsToJsonStringList() const
{
    QStringList res;

    const QList<QStringList> buyVariants = {
        mBuyDecision1Config->variantsAsJson(),
        mBuyDecision2Config->variantsAsJson(),
        mBuyDecision3Config->variantsAsJson(),
        mBuyDecision4Config->variantsAsJson(),
        mBuyDecision5Config->variantsAsJson(),
        mBuyDecision6Config->variantsAsJson(),
        mBuyDecision7Config->variantsAsJson(),
        mBuyDecision8Config->variantsAsJson(),
    };
    const QList<QStringList> sellVariants = {
        mSellDecision1Config->variantsAsJson(),
        mSellDecision2Config->variantsAsJson(),
        // Do not add sell decisions 3 and 4
        // mSellDecision3Config->variantsAsJson(),
        // mSellDecision4Config->variantsAsJson(),
    };

    const QString s3 = R"({"enabled":false})";
    const QString s4 = R"({"enabled":false})";

    QStringList results;

    for (int i = 0; i < buyVariants.size(); ++i)
    {
        const QStringList& buyVariant = buyVariants.at(i);
        QStringList        buyConfigs(buyVariants.size(), R"({"enabled":false})");

        results.clear();

        for (int j = 1; j < buyVariant.size(); ++j)
        {
            buyConfigs[i] = buyVariant.at(j);

            for (int k = 0; k < sellVariants.size(); ++k)
            {
                const QStringList& sellVariant = sellVariants.at(k);
                QStringList        sellConfigs(sellVariants.size(), R"({"enabled":false})");

                for (int g = 1; g < sellVariant.size(); ++g)
                {
                    sellConfigs[k] = sellVariant.at(g);

                    // NOLINTBEGIN(readability-magic-numbers)
                    results.append(QString("{%1,%2}").arg(
                        QString(R"("b1":%1,"b2":%2,"b3":%3,"b4":%4,"b5":%5,"b6":%6,"b7":%7,"b8":%8)")
                            .arg(
                                buyConfigs[0],
                                buyConfigs[1],
                                buyConfigs[2],
                                buyConfigs[3],
                                buyConfigs[4],
                                buyConfigs[5],
                                buyConfigs[6],
                                buyConfigs[7]
                            ),
                        QString(R"("s1":%1,"s2":%2,"s3":%3,"s4":%4)").arg(sellConfigs[0], sellConfigs[1], s3, s4)
                    ));
                    // NOLINTEND(readability-magic-numbers)
                }
            }
        }

        res.append("[\n" + results.join(",\n") + "\n]");
    }

    return res;
}

QString DecisionMakerConfig::variantsToJsonStringListExtendedBySellDecisions(const QStringList& bestConfigs) const
{
    QStringList res;

    QStringList unitedBestConfigs;

    // NOLINTBEGIN(readability-magic-numbers)
    Q_ASSERT_X(bestConfigs.size() == 8, __FUNCTION__, "Unexpected behavior");
    const QString& config0 = bestConfigs.at(0);
    const QString& config1 = bestConfigs.at(1);
    const QString& config2 = bestConfigs.at(2);
    const QString& config3 = bestConfigs.at(3);
    const QString& config4 = bestConfigs.at(4);
    const QString& config5 = bestConfigs.at(5);
    const QString& config6 = bestConfigs.at(6);
    const QString& config7 = bestConfigs.at(7);
    // NOLINTEND(readability-magic-numbers)

    const QString b1 =
        config0.mid(config0.indexOf(R"("b1":)") + 5, config0.indexOf(R"("b2":)") - config0.indexOf(R"("b1":)") - 6);
    const QString b2 =
        config1.mid(config1.indexOf(R"("b2":)") + 5, config1.indexOf(R"("b3":)") - config1.indexOf(R"("b2":)") - 6);
    const QString b3 =
        config2.mid(config2.indexOf(R"("b3":)") + 5, config2.indexOf(R"("b4":)") - config2.indexOf(R"("b3":)") - 6);
    const QString b4 =
        config3.mid(config3.indexOf(R"("b4":)") + 5, config3.indexOf(R"("b5":)") - config3.indexOf(R"("b4":)") - 6);
    const QString b5 =
        config4.mid(config4.indexOf(R"("b5":)") + 5, config4.indexOf(R"("b6":)") - config4.indexOf(R"("b5":)") - 6);
    const QString b6 =
        config5.mid(config5.indexOf(R"("b6":)") + 5, config5.indexOf(R"("b7":)") - config5.indexOf(R"("b6":)") - 6);
    const QString b7 =
        config6.mid(config6.indexOf(R"("b7":)") + 5, config6.indexOf(R"("b8":)") - config6.indexOf(R"("b7":)") - 6);
    const QString b8 =
        config7.mid(config7.indexOf(R"("b8":)") + 5, config7.indexOf(R"("s1":)") - config7.indexOf(R"("b8":)") - 6);

    for (const QString& bestConfig : bestConfigs)
    {
        unitedBestConfigs.append(QString("{%1,%2}").arg(
            QString(R"("b1":%1,"b2":%2,"b3":%3,"b4":%4,"b5":%5,"b6":%6,"b7":%7,"b8":%8)").arg(b1, b2, b3, b4, b5, b6, b7, b8),
            bestConfig.mid(bestConfig.indexOf(R"("s1":)"))
        ));
    }

    unitedBestConfigs.removeDuplicates();

    const QStringList sellVariants3 = mSellDecision3Config->variantsAsJson();
    const QStringList sellVariants4 = mSellDecision4Config->variantsAsJson();

    for (const QString& bestConfig : unitedBestConfigs)
    {
        for (const QString& s3 : sellVariants3)
        {
            for (const QString& s4 : sellVariants4)
            {
                QString temp = bestConfig;

                res.append(temp.replace(R"("s3":{"enabled":false})", QString(R"("s3":%1)").arg(s3))
                               .replace(R"("s4":{"enabled":false})", QString(R"("s4":%1)").arg(s4)));
            }
        }
    }

    return "[\n" + res.join(",\n") + "\n]";
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

IBuyDecision4Config* DecisionMakerConfig::getBuyDecision4Config()
{
    return mBuyDecision4Config;
}

IBuyDecision5Config* DecisionMakerConfig::getBuyDecision5Config()
{
    return mBuyDecision5Config;
}

IBuyDecision6Config* DecisionMakerConfig::getBuyDecision6Config()
{
    return mBuyDecision6Config;
}

IBuyDecision7Config* DecisionMakerConfig::getBuyDecision7Config()
{
    return mBuyDecision7Config;
}

IBuyDecision8Config* DecisionMakerConfig::getBuyDecision8Config()
{
    return mBuyDecision8Config;
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

ISellDecision4Config* DecisionMakerConfig::getSellDecision4Config()
{
    return mSellDecision4Config;
}
