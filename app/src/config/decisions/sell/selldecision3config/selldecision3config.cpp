#include "src/config/decisions/sell/selldecision3config/selldecision3config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr int COMMISSION_30 = 30;
constexpr int COMMISSION_5  = 5;

// For commission 0.30%
constexpr bool  ENABLED_DEFAULT_30    = true;
constexpr float LOSE_YIELD_DEFAULT_30 = 3.0f;

// For commission 0.05%
constexpr bool  ENABLED_DEFAULT_5    = true;
constexpr float LOSE_YIELD_DEFAULT_5 = 3.0f;

// For commission 0.04%
constexpr bool  ENABLED_DEFAULT_4    = true;
constexpr float LOSE_YIELD_DEFAULT_4 = 3.0f;



SellDecision3Config::SellDecision3Config() :
    ISellDecision3Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mLoseYield()
{
    qDebug() << "Create SellDecision3Config";
}

SellDecision3Config::~SellDecision3Config()
{
    qDebug() << "Destroy SellDecision3Config";

    delete mRwMutex;
}

ISellDecision3Config* SellDecision3Config::clone()
{
    SellDecision3Config* res = new SellDecision3Config();
    res->assign(this);

    return res;
}

void SellDecision3Config::deleteRecursively()
{
    delete this;
}

void SellDecision3Config::assign(ISellDecision3Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning SellDecision3Config to SellDecision3Config";

    const SellDecision3Config& config = *dynamic_cast<SellDecision3Config*>(another);
    const QReadLocker          lock2(config.mRwMutex);

    mEnabled   = config.mEnabled;
    mLoseYield = config.mLoseYield;
}

void SellDecision3Config::makeDefault(int commissionInt)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set SellDecision3Config to default";

    if (commissionInt == COMMISSION_30)
    {
        mEnabled   = ENABLED_DEFAULT_30;
        mLoseYield = LOSE_YIELD_DEFAULT_30;
    }
    else if (commissionInt == COMMISSION_5)
    {
        mEnabled   = ENABLED_DEFAULT_5;
        mLoseYield = LOSE_YIELD_DEFAULT_5;
    }
    else
    {
        mEnabled   = ENABLED_DEFAULT_4;
        mLoseYield = LOSE_YIELD_DEFAULT_4;
    }
}

bool SellDecision3Config::isDefault(int commissionInt)
{
    SellDecision3Config defaultConfig;
    defaultConfig.makeDefault(commissionInt);

    const QReadLocker lock(mRwMutex);

    return mEnabled == defaultConfig.mEnabled && mLoseYield == defaultConfig.mLoseYield;
}

void SellDecision3Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save SellDecision3Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",    mEnabled);
    settingsEditor->setValue(type + "/LoseYield",  mLoseYield);
    // clang-format on
}

void SellDecision3Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load SellDecision3Config";

    // clang-format off
    mEnabled    = settingsEditor->value(type + "/Enabled",    mEnabled).toBool();
    mLoseYield  = settingsEditor->value(type + "/LoseYield",  mLoseYield).toFloat();
    // clang-format on
}

static void configEnabledParse(SellDecision3Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configLoseYieldParse(SellDecision3Config* config, simdjson::ondemand::value value)
{
    config->setLoseYield(value.get_double_in_string());
}

static void configThrowParseException(
    SellDecision3Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(SellDecision3Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",   configEnabledParse  },
    {"loseYield", configLoseYieldParse}
};
// clang-format on

void SellDecision3Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString SellDecision3Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"loseYield":"%2"})").arg(mEnabled ? "true" : "false", QString::number(mLoseYield, 'f', 2));
}

QStringList SellDecision3Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList loseYieldVariants = {"3.00", "5.00", "7.00"};

    for (const QString& loseYield : loseYieldVariants)
    {
        res.append(QString(R"({"enabled":true,"loseYield":"%1"})").arg(loseYield));
    }

    return res;
}

void SellDecision3Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool SellDecision3Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void SellDecision3Config::setLoseYield(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLoseYield = value;
}

float SellDecision3Config::getLoseYield()
{
    const QReadLocker lock(mRwMutex);

    return mLoseYield;
}
