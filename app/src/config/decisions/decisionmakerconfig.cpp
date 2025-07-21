#include "src/config/decisions/decisionmakerconfig.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/threads/parallelhelper/parallelhelperthread.h"
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

struct CombineVariantsInfo
{
    explicit CombineVariantsInfo(const QList<QStringList> _variants) :
        variants(_variants)
    {
#ifndef TESTING_MODE
        const int cpuCount = QThread::idealThreadCount();
#else
        const int cpuCount = 1;
#endif

        results.resize(cpuCount);
    }

    QList<QStringList> variants;
    QList<QStringList> results;
};

static void
combineVariantsForParallel(QThread* parentThread, int threadId, QList<int>& /*temp*/, int start, int end, void* additionalArgs)
{
    CombineVariantsInfo* combineVariantsInfo = reinterpret_cast<CombineVariantsInfo*>(additionalArgs);

    QStringList* variantsArray = combineVariantsInfo->variants.data();
    const int    variantsSize  = combineVariantsInfo->variants.size();
    QStringList* resultsArray  = combineVariantsInfo->results.data();

    Q_ASSERT_X(variantsSize == 6, __FUNCTION__, "Unexpected behavior");

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
        const int s1Index  = cur % variantsArray[3].size();
        cur               /= variantsArray[3].size();
        const int s2Index  = cur % variantsArray[4].size();
        cur               /= variantsArray[4].size();
        const int s3Index  = cur % variantsArray[5].size();

        if ((b1Index != 0 || b2Index != 0 || b3Index != 0) && ((s1Index != 0 && s2Index == 0) || (s1Index == 0 && s2Index != 0)))
        {
            resultsArray[threadId].append(QString(R"({"b1":%1,"b2":%2,"b3":%3,"s1":%4,"s2":%5,"s3":%6})")
                                              .arg(
                                                  variantsArray[0][b1Index],
                                                  variantsArray[1][b2Index],
                                                  variantsArray[2][b3Index],
                                                  variantsArray[3][s1Index],
                                                  variantsArray[4][s2Index],
                                                  variantsArray[5][s3Index]
                                              ));
        }
    }
}

QString DecisionMakerConfig::variantsToJsonString() const
{
    QString res;

    QList<QStringList> variants;
    QList<int>         temp;

    variants.append(mBuyDecision1Config->variantsAsJson());
    variants.append(mBuyDecision2Config->variantsAsJson());
    variants.append(mBuyDecision3Config->variantsAsJson());
    variants.append(mSellDecision1Config->variantsAsJson());
    variants.append(mSellDecision2Config->variantsAsJson());
    variants.append(mSellDecision3Config->variantsAsJson());

    CombineVariantsInfo combineVariantsInfo(variants);
    processInParallel(temp, combineVariantsForParallel, &combineVariantsInfo);

    res        = "[\n";
    bool first = true;

    for (const QStringList& results : combineVariantsInfo.results)
    {
        if (!results.isEmpty())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                res += ",\n";
            }

            res += results.join(",\n");
        }
    }

    res += "\n]";

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
