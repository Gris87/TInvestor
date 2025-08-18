#include "src/config/decisions/sell/selldecision2config/selldecision2config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



// For 2024 year
constexpr bool  ENABLED_DEFAULT     = false;
constexpr float YIELD_ABOVE_DEFAULT = 1.0f;
constexpr float LOSE_YIELD_DEFAULT  = 0.1f;

// For 2024 year
// constexpr bool  ENABLED_DEFAULT     = false;
// constexpr float YIELD_ABOVE_DEFAULT = 1.0f;
// constexpr float LOSE_YIELD_DEFAULT  = 0.1f;



SellDecision2Config::SellDecision2Config() :
    ISellDecision2Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mYieldAbove(),
    mLoseYield()
{
    qDebug() << "Create SellDecision2Config";
}

SellDecision2Config::~SellDecision2Config()
{
    qDebug() << "Destroy SellDecision2Config";

    delete mRwMutex;
}

ISellDecision2Config* SellDecision2Config::clone()
{
    SellDecision2Config* res = new SellDecision2Config();
    res->assign(this);

    return res;
}

void SellDecision2Config::deleteRecursively()
{
    delete this;
}

void SellDecision2Config::assign(ISellDecision2Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning SellDecision2Config to SellDecision2Config";

    const SellDecision2Config& config = *dynamic_cast<SellDecision2Config*>(another);
    const QReadLocker          lock2(config.mRwMutex);

    mEnabled    = config.mEnabled;
    mYieldAbove = config.mYieldAbove;
    mLoseYield  = config.mLoseYield;
}

void SellDecision2Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set SellDecision2Config to default";

    mEnabled    = ENABLED_DEFAULT;
    mYieldAbove = YIELD_ABOVE_DEFAULT;
    mLoseYield  = LOSE_YIELD_DEFAULT;
}

void SellDecision2Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save SellDecision2Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",    mEnabled);
    settingsEditor->setValue(type + "/YieldAbove", mYieldAbove);
    settingsEditor->setValue(type + "/LoseYield",  mLoseYield);
    // clang-format on
}

void SellDecision2Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load SellDecision2Config";

    // clang-format off
    mEnabled    = settingsEditor->value(type + "/Enabled",    mEnabled).toBool();
    mYieldAbove = settingsEditor->value(type + "/YieldAbove", mYieldAbove).toFloat();
    mLoseYield  = settingsEditor->value(type + "/LoseYield",  mLoseYield).toFloat();
    // clang-format on
}

static void configEnabledParse(SellDecision2Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configYieldAboveParse(SellDecision2Config* config, simdjson::ondemand::value value)
{
    config->setYieldAbove(value.get_double_in_string());
}

static void configLoseYieldParse(SellDecision2Config* config, simdjson::ondemand::value value)
{
    config->setLoseYield(value.get_double_in_string());
}

static void configThrowParseException(
    SellDecision2Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(SellDecision2Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",    configEnabledParse    },
    {"yieldAbove", configYieldAboveParse},
    {"loseYield",  configLoseYieldParse }
};
// clang-format on

void SellDecision2Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString SellDecision2Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"yieldAbove":"%2","loseYield":"%3"})")
        .arg(mEnabled ? "true" : "false", QString::number(mYieldAbove, 'f', 2), QString::number(mLoseYield, 'f', 2));
}

QStringList SellDecision2Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList yieldAboveVariants = {"0.50", "1.00", "2.00"};
    const QStringList loseYieldVariants  = {"0.1", "0.3", "0.5", "0.7"};

    for (const QString& yieldAbove : yieldAboveVariants)
    {
        for (const QString& loseYield : loseYieldVariants)
        {
            res.append(QString(R"({"enabled":true,"yieldAbove":"%1","loseYield":"%2"})").arg(yieldAbove, loseYield));
        }
    }

    return res;
}

void SellDecision2Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool SellDecision2Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void SellDecision2Config::setYieldAbove(float value)
{
    const QWriteLocker lock(mRwMutex);

    mYieldAbove = value;
}

float SellDecision2Config::getYieldAbove()
{
    const QReadLocker lock(mRwMutex);

    return mYieldAbove;
}

void SellDecision2Config::setLoseYield(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLoseYield = value;
}

float SellDecision2Config::getLoseYield()
{
    const QReadLocker lock(mRwMutex);

    return mLoseYield;
}
