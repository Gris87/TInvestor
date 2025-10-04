#include "src/config/decisions/sell/selldecision5config/selldecision5config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



// For 2020-2024 years
constexpr bool  ENABLED_DEFAULT     = false;
constexpr int   DURATION_DEFAULT    = 15;
constexpr float YIELD_ABOVE_DEFAULT = 0.5f;



SellDecision5Config::SellDecision5Config() :
    ISellDecision5Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mDuration(),
    mYieldAbove()
{
    qDebug() << "Create SellDecision5Config";
}

SellDecision5Config::~SellDecision5Config()
{
    qDebug() << "Destroy SellDecision5Config";

    delete mRwMutex;
}

ISellDecision5Config* SellDecision5Config::clone()
{
    SellDecision5Config* res = new SellDecision5Config();
    res->assign(this);

    return res;
}

void SellDecision5Config::deleteRecursively()
{
    delete this;
}

void SellDecision5Config::assign(ISellDecision5Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning SellDecision5Config to SellDecision5Config";

    const SellDecision5Config& config = *dynamic_cast<SellDecision5Config*>(another);
    const QReadLocker          lock2(config.mRwMutex);

    mEnabled    = config.mEnabled;
    mDuration   = config.mDuration;
    mYieldAbove = config.mYieldAbove;
}

void SellDecision5Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set SellDecision5Config to default";

    mEnabled    = ENABLED_DEFAULT;
    mDuration   = DURATION_DEFAULT;
    mYieldAbove = YIELD_ABOVE_DEFAULT;
}

void SellDecision5Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save SellDecision5Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",    mEnabled);
    settingsEditor->setValue(type + "/Duration",   mDuration);
    settingsEditor->setValue(type + "/YieldAbove", mYieldAbove);
    // clang-format on
}

void SellDecision5Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load SellDecision5Config";

    // clang-format off
    mEnabled    = settingsEditor->value(type + "/Enabled",    mEnabled).toBool();
    mDuration   = settingsEditor->value(type + "/Duration",   mDuration).toInt();
    mYieldAbove = settingsEditor->value(type + "/YieldAbove", mYieldAbove).toFloat();
    // clang-format on
}

static void configEnabledParse(SellDecision5Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configDurationParse(SellDecision5Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configYieldAboveParse(SellDecision5Config* config, simdjson::ondemand::value value)
{
    config->setYieldAbove(value.get_double_in_string());
}

static void configThrowParseException(
    SellDecision5Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(SellDecision5Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",    configEnabledParse   },
    {"duration",   configDurationParse  },
    {"yieldAbove", configYieldAboveParse}
};
// clang-format on

void SellDecision5Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString SellDecision5Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"duration":%2,"yieldAbove":"%3"})")
        .arg(mEnabled ? "true" : "false", QString::number(mDuration), QString::number(mYieldAbove, 'f', 2));
}

QStringList SellDecision5Config::variantsAsJson() const
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

void SellDecision5Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool SellDecision5Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void SellDecision5Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int SellDecision5Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}

void SellDecision5Config::setYieldAbove(float value)
{
    const QWriteLocker lock(mRwMutex);

    mYieldAbove = value;
}

float SellDecision5Config::getYieldAbove()
{
    const QReadLocker lock(mRwMutex);

    return mYieldAbove;
}
