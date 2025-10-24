#include "src/config/decisions/sell/selldecision4config/selldecision4config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr int COMMISSION_30 = 30;
constexpr int COMMISSION_5  = 5;

// For commission 0.30%
constexpr bool  ENABLED_DEFAULT_30     = true;
constexpr int   DURATION_DEFAULT_30    = 15;
constexpr float YIELD_ABOVE_DEFAULT_30 = 0.5f;

// For commission 0.05%
constexpr bool  ENABLED_DEFAULT_5     = true;
constexpr int   DURATION_DEFAULT_5    = 15;
constexpr float YIELD_ABOVE_DEFAULT_5 = 0.5f;

// For commission 0.04%
constexpr bool  ENABLED_DEFAULT_4     = true;
constexpr int   DURATION_DEFAULT_4    = 15;
constexpr float YIELD_ABOVE_DEFAULT_4 = 0.5f;



SellDecision4Config::SellDecision4Config() :
    ISellDecision4Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mDuration(),
    mYieldAbove()
{
    qDebug() << "Create SellDecision4Config";
}

SellDecision4Config::~SellDecision4Config()
{
    qDebug() << "Destroy SellDecision4Config";

    delete mRwMutex;
}

ISellDecision4Config* SellDecision4Config::clone()
{
    SellDecision4Config* res = new SellDecision4Config();
    res->assign(this);

    return res;
}

void SellDecision4Config::deleteRecursively()
{
    delete this;
}

void SellDecision4Config::assign(ISellDecision4Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning SellDecision4Config to SellDecision4Config";

    const SellDecision4Config& config = *dynamic_cast<SellDecision4Config*>(another);
    const QReadLocker          lock2(config.mRwMutex);

    mEnabled    = config.mEnabled;
    mDuration   = config.mDuration;
    mYieldAbove = config.mYieldAbove;
}

void SellDecision4Config::makeDefault(int commissionInt)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set SellDecision4Config to default";

    if (commissionInt == COMMISSION_30)
    {
        mEnabled    = ENABLED_DEFAULT_30;
        mDuration   = DURATION_DEFAULT_30;
        mYieldAbove = YIELD_ABOVE_DEFAULT_30;
    }
    else if (commissionInt == COMMISSION_5)
    {
        mEnabled    = ENABLED_DEFAULT_5;
        mDuration   = DURATION_DEFAULT_5;
        mYieldAbove = YIELD_ABOVE_DEFAULT_5;
    }
    else
    {
        mEnabled    = ENABLED_DEFAULT_4;
        mDuration   = DURATION_DEFAULT_4;
        mYieldAbove = YIELD_ABOVE_DEFAULT_4;
    }
}

bool SellDecision4Config::isDefault(int commissionInt)
{
    SellDecision4Config defaultConfig;
    defaultConfig.makeDefault(commissionInt);

    const QReadLocker lock(mRwMutex);

    return mEnabled == defaultConfig.mEnabled && mDuration == defaultConfig.mDuration && mYieldAbove == defaultConfig.mYieldAbove;
}

void SellDecision4Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save SellDecision4Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",    mEnabled);
    settingsEditor->setValue(type + "/Duration",   mDuration);
    settingsEditor->setValue(type + "/YieldAbove", mYieldAbove);
    // clang-format on
}

void SellDecision4Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load SellDecision4Config";

    // clang-format off
    mEnabled    = settingsEditor->value(type + "/Enabled",    mEnabled).toBool();
    mDuration   = settingsEditor->value(type + "/Duration",   mDuration).toInt();
    mYieldAbove = settingsEditor->value(type + "/YieldAbove", mYieldAbove).toFloat();
    // clang-format on
}

static void configEnabledParse(SellDecision4Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configDurationParse(SellDecision4Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configYieldAboveParse(SellDecision4Config* config, simdjson::ondemand::value value)
{
    config->setYieldAbove(value.get_double_in_string());
}

static void configThrowParseException(
    SellDecision4Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(SellDecision4Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",    configEnabledParse   },
    {"duration",   configDurationParse  },
    {"yieldAbove", configYieldAboveParse}
};
// clang-format on

void SellDecision4Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString SellDecision4Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"duration":%2,"yieldAbove":"%3"})")
        .arg(mEnabled ? "true" : "false", QString::number(mDuration), QString::number(mYieldAbove, 'f', 2));
}

QStringList SellDecision4Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList durationVariants   = {"15", "30", "60"};
    const QStringList yieldAboveVariants = {"0.50", "1.00", "1.50"};

    for (const QString& duration : durationVariants)
    {
        for (const QString& yieldAbove : yieldAboveVariants)
        {
            res.append(QString(R"({"enabled":true,"duration":%1,"yieldAbove":"%2"})").arg(duration, yieldAbove));
        }
    }

    return res;
}

void SellDecision4Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool SellDecision4Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void SellDecision4Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int SellDecision4Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}

void SellDecision4Config::setYieldAbove(float value)
{
    const QWriteLocker lock(mRwMutex);

    mYieldAbove = value;
}

float SellDecision4Config::getYieldAbove()
{
    const QReadLocker lock(mRwMutex);

    return mYieldAbove;
}
