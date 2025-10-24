#include "src/config/decisions/sell/selldecision1config/selldecision1config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr int COMMISSION_30 = 30;
constexpr int COMMISSION_5  = 5;

// For commission 0.30%
constexpr bool  ENABLED_DEFAULT_30     = true;
constexpr float YIELD_ABOVE_DEFAULT_30 = 0.5f;

// For commission 0.05%
constexpr bool  ENABLED_DEFAULT_5     = true;
constexpr float YIELD_ABOVE_DEFAULT_5 = 0.5f;

// For commission 0.04%
constexpr bool  ENABLED_DEFAULT_4     = true;
constexpr float YIELD_ABOVE_DEFAULT_4 = 0.5f;



SellDecision1Config::SellDecision1Config() :
    ISellDecision1Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mYieldAbove()
{
    qDebug() << "Create SellDecision1Config";
}

SellDecision1Config::~SellDecision1Config()
{
    qDebug() << "Destroy SellDecision1Config";

    delete mRwMutex;
}

ISellDecision1Config* SellDecision1Config::clone()
{
    SellDecision1Config* res = new SellDecision1Config();
    res->assign(this);

    return res;
}

void SellDecision1Config::deleteRecursively()
{
    delete this;
}

void SellDecision1Config::assign(ISellDecision1Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning SellDecision1Config to SellDecision1Config";

    const SellDecision1Config& config = *dynamic_cast<SellDecision1Config*>(another);
    const QReadLocker          lock2(config.mRwMutex);

    mEnabled    = config.mEnabled;
    mYieldAbove = config.mYieldAbove;
}

void SellDecision1Config::makeDefault(int commissionInt)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set SellDecision1Config to default";

    if (commissionInt == COMMISSION_30)
    {
        mEnabled    = ENABLED_DEFAULT_30;
        mYieldAbove = YIELD_ABOVE_DEFAULT_30;
    }
    else if (commissionInt == COMMISSION_5)
    {
        mEnabled    = ENABLED_DEFAULT_5;
        mYieldAbove = YIELD_ABOVE_DEFAULT_5;
    }
    else
    {
        mEnabled    = ENABLED_DEFAULT_4;
        mYieldAbove = YIELD_ABOVE_DEFAULT_4;
    }
}

bool SellDecision1Config::isDefault(int commissionInt)
{
    SellDecision1Config defaultConfig;
    defaultConfig.makeDefault(commissionInt);

    const QReadLocker lock(mRwMutex);

    return mEnabled == defaultConfig.mEnabled && mYieldAbove == defaultConfig.mYieldAbove;
}

void SellDecision1Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save SellDecision1Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",    mEnabled);
    settingsEditor->setValue(type + "/YieldAbove", mYieldAbove);
    // clang-format on
}

void SellDecision1Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load SellDecision1Config";

    // clang-format off
    mEnabled    = settingsEditor->value(type + "/Enabled",    mEnabled).toBool();
    mYieldAbove = settingsEditor->value(type + "/YieldAbove", mYieldAbove).toFloat();
    // clang-format on
}

static void configEnabledParse(SellDecision1Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configYieldAboveParse(SellDecision1Config* config, simdjson::ondemand::value value)
{
    config->setYieldAbove(value.get_double_in_string());
}

static void configThrowParseException(
    SellDecision1Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(SellDecision1Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",    configEnabledParse   },
    {"yieldAbove", configYieldAboveParse}
};
// clang-format on

void SellDecision1Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString SellDecision1Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"yieldAbove":"%2"})").arg(mEnabled ? "true" : "false", QString::number(mYieldAbove, 'f', 2));
}

QStringList SellDecision1Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList yieldAboveVariants = {"0.50", "1.00", "2.00"};

    for (const QString& yieldAbove : yieldAboveVariants)
    {
        res.append(QString(R"({"enabled":true,"yieldAbove":"%1"})").arg(yieldAbove));
    }

    return res;
}

void SellDecision1Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool SellDecision1Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void SellDecision1Config::setYieldAbove(float value)
{
    const QWriteLocker lock(mRwMutex);

    mYieldAbove = value;
}

float SellDecision1Config::getYieldAbove()
{
    const QReadLocker lock(mRwMutex);

    return mYieldAbove;
}
