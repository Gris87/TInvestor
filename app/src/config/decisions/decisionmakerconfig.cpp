#include "src/config/decisions/decisionmakerconfig.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/utils/exception/exception.h"



DecisionMakerConfig::DecisionMakerConfig(
    IBuyDecision1Config*  buyDecision1Config,
    IBuyDecision2Config*  buyDecision2Config,
    IBuyDecision3Config*  buyDecision3Config,
    IBuyDecision4Config*  buyDecision4Config,
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

void DecisionMakerConfig::assign(IDecisionMakerConfig* another)
{
    qDebug() << "Assigning DecisionMakerConfig to DecisionMakerConfig";

    const DecisionMakerConfig& config = *dynamic_cast<DecisionMakerConfig*>(another);

    mBuyDecision1Config->assign(config.mBuyDecision1Config);
    mBuyDecision2Config->assign(config.mBuyDecision2Config);
    mBuyDecision3Config->assign(config.mBuyDecision3Config);
    mBuyDecision4Config->assign(config.mBuyDecision4Config);
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
    mSellDecision1Config->load(settingsEditor, type + "/SellDecision1Config");
    mSellDecision2Config->load(settingsEditor, type + "/SellDecision2Config");
    mSellDecision3Config->load(settingsEditor, type + "/SellDecision3Config");
    mSellDecision4Config->load(settingsEditor, type + "/SellDecision4Config");
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

static void configBuy4Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
{
    config->getBuyDecision4Config()->fromJsonObject(value.get_object());
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

static void configSell4Parse(DecisionMakerConfig* config, simdjson::ondemand::value value)
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
    {"b1", configBuy1Parse },
    {"b2", configBuy2Parse },
    {"b3", configBuy3Parse },
    {"b4", configBuy4Parse },
    {"s1", configSell1Parse},
    {"s2", configSell2Parse},
    {"s3", configSell3Parse},
    {"s4", configSell4Parse}
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

    const QString s1 = mSellDecision1Config->toJsonString();
    const QString s2 = mSellDecision2Config->toJsonString();
    const QString s3 = mSellDecision3Config->toJsonString();
    const QString s4 = mSellDecision4Config->toJsonString();

    return QString(R"({"b1":%1,"b2":%2,"b3":%3,"b4":%4,"s1":%5,"s2":%6,"s3":%7,"s4":%8})").arg(b1, b2, b3, b4, s1, s2, s3, s4);
}

struct CombineVariantsInfo
{
    explicit CombineVariantsInfo(const QList<QStringList>& _variants) :
        variants(_variants)
    {
        results.resize(getCpuCount());
    }

    QList<QStringList> variants;
    QList<QStringList> results;
};

// NOLINTBEGIN(readability-magic-numbers)
static void
combineVariantsForParallel(QThread* parentThread, int threadId, QList<int>& /*temp*/, int start, int end, void* additionalArgs)
{
    CombineVariantsInfo* combineVariantsInfo = reinterpret_cast<CombineVariantsInfo*>(additionalArgs);

    QStringList* variantsArray = combineVariantsInfo->variants.data();
    const int    variantsSize  = combineVariantsInfo->variants.size();
    QStringList* resultsArray  = combineVariantsInfo->results.data();

    Q_ASSERT_X(variantsSize == 8, __FUNCTION__, "Unexpected behavior");

    int total = 1;

    for (int i = 0; i < variantsSize; ++i)
    {
        total *= variantsArray[i].size();
    }

    const int part = total / combineVariantsInfo->results.size();

    start = part * threadId;
    end   = qMin(part * (threadId + 1), total);

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        int cur = i;

        const int b1Index  = cur % variantsArray[0].size();
        cur               /= variantsArray[0].size();
        const int b2Index  = cur % variantsArray[1].size();
        cur               /= variantsArray[1].size();
        const int b3Index  = cur % variantsArray[2].size();
        cur               /= variantsArray[2].size();
        const int b4Index  = cur % variantsArray[3].size();
        cur               /= variantsArray[3].size();
        const int s1Index  = cur % variantsArray[4].size();
        cur               /= variantsArray[4].size();
        const int s2Index  = cur % variantsArray[5].size();
        cur               /= variantsArray[5].size();
        const int s3Index  = cur % variantsArray[6].size();
        cur               /= variantsArray[6].size();
        const int s4Index  = cur % variantsArray[7].size();

        if ((b1Index != 0 || b2Index != 0 || b3Index != 0 || b4Index != 0) &&
            ((s1Index != 0 && s2Index == 0) || (s1Index == 0 && s2Index != 0)))
        {
            resultsArray[threadId].append(QString(R"({"b1":%1,"b2":%2,"b3":%3,"b4":%4,"s1":%5,"s2":%6,"s3":%7,"s4":%8})")
                                              .arg(
                                                  variantsArray[0][b1Index],
                                                  variantsArray[1][b2Index],
                                                  variantsArray[2][b3Index],
                                                  variantsArray[3][b4Index],
                                                  variantsArray[4][s1Index],
                                                  variantsArray[5][s2Index],
                                                  variantsArray[6][s3Index],
                                                  variantsArray[7][s4Index]
                                              ));
        }
    }
}
// NOLINTEND(readability-magic-numbers)

QString DecisionMakerConfig::variantsToJsonString() const
{
    QList<QStringList> buyVariants;
    QList<QStringList> sellVariants;

    buyVariants.append(mBuyDecision1Config->variantsAsJson());
    buyVariants.append(mBuyDecision2Config->variantsAsJson());
    buyVariants.append(mBuyDecision3Config->variantsAsJson());
    buyVariants.append(mBuyDecision4Config->variantsAsJson());
    sellVariants.append(mSellDecision1Config->variantsAsJson());
    sellVariants.append(mSellDecision2Config->variantsAsJson());
    // Do not add sell decisions 3 and 4
    // sellVariants.append(mSellDecision3Config->variantsAsJson());
    // sellVariants.append(mSellDecision4Config->variantsAsJson());

    const QString s3 = R"({"enabled":false})";
    const QString s4 = R"({"enabled":false})";

    QStringList results;

    for (int i = 0; i < buyVariants.size(); ++i)
    {
        const QStringList& buyVariant = buyVariants.at(i);
        QStringList        buyConfigs(buyVariants.size(), R"({"enabled":false})");

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

                    results.append(
                        QString(R"({"b1":%1,"b2":%2,"b3":%3,"b4":%4,"s1":%5,"s2":%6,"s3":%7,"s4":%8})")
                            .arg(
                                buyConfigs[0], buyConfigs[1], buyConfigs[2], buyConfigs[3], sellConfigs[0], sellConfigs[1], s3, s4
                            )
                    );
                }
            }
        }
    }

    return "[\n" + results.join(",\n") + "\n]";
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
