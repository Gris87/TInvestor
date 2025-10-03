#include "src/config/decisions/sell/selldecision5config/selldecision5config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



// For 2020-2024 years
constexpr bool  ENABLED_DEFAULT    = false;
constexpr float LOSE_YIELD_DEFAULT = 10.0f;
constexpr int   DURATION_DEFAULT   = 5;

// For 2024 year
// constexpr bool  ENABLED_DEFAULT    = false;
// constexpr float LOSE_YIELD_DEFAULT = 10.0f;
// constexpr int   DURATION_DEFAULT   = 5;

// For 2019 year
// constexpr bool  ENABLED_DEFAULT    = false;
// constexpr float LOSE_YIELD_DEFAULT = 15.0f;
// constexpr int   DURATION_DEFAULT   = 30;



SellDecision5Config::SellDecision5Config() :
    ISellDecision5Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mLoseYield(),
    mDuration()
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

    mEnabled   = config.mEnabled;
    mLoseYield = config.mLoseYield;
    mDuration  = config.mDuration;
}

void SellDecision5Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set SellDecision5Config to default";

    mEnabled   = ENABLED_DEFAULT;
    mLoseYield = LOSE_YIELD_DEFAULT;
    mDuration  = DURATION_DEFAULT;
}

void SellDecision5Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save SellDecision5Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",   mEnabled);
    settingsEditor->setValue(type + "/LoseYield", mLoseYield);
    settingsEditor->setValue(type + "/Duration",  mDuration);
    // clang-format on
}

void SellDecision5Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load SellDecision5Config";

    // clang-format off
    mEnabled   = settingsEditor->value(type + "/Enabled",   mEnabled).toBool();
    mLoseYield = settingsEditor->value(type + "/LoseYield", mLoseYield).toFloat();
    mDuration  = settingsEditor->value(type + "/Duration",  mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(SellDecision5Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configLoseYieldParse(SellDecision5Config* config, simdjson::ondemand::value value)
{
    config->setLoseYield(value.get_double_in_string());
}

static void configDurationParse(SellDecision5Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
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
    {"enabled",   configEnabledParse  },
    {"loseYield", configLoseYieldParse},
    {"duration",  configDurationParse }
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
    return QString(R"({"enabled":%1,"loseYield":"%2","duration":%3})")
        .arg(mEnabled ? "true" : "false", QString::number(mLoseYield, 'f', 2), QString::number(mDuration));
}

QStringList SellDecision5Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList loseYieldVariants = {"5.00", "7.00", "10.00"};
    const QStringList durationVariants  = {"5", "15", "30"};

    for (const QString& loseYield : loseYieldVariants)
    {
        for (const QString& duration : durationVariants)
        {
            res.append(QString(R"({"enabled":true,"loseYield":"%1","duration":%2})").arg(loseYield, duration));
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

void SellDecision5Config::setLoseYield(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLoseYield = value;
}

float SellDecision5Config::getLoseYield()
{
    const QReadLocker lock(mRwMutex);

    return mLoseYield;
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
