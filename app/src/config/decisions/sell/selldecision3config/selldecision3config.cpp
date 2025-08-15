#include "src/config/decisions/sell/selldecision3config/selldecision3config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



// For 2024 year
constexpr bool  ENABLED_DEFAULT    = true;
constexpr float LOSE_YIELD_DEFAULT = 10.0f;
constexpr int   DURATION_DEFAULT   = 5;



SellDecision3Config::SellDecision3Config() :
    ISellDecision3Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mLoseYield(),
    mDuration()
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
    mDuration  = config.mDuration;
}

void SellDecision3Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set SellDecision3Config to default";

    mEnabled   = ENABLED_DEFAULT;
    mLoseYield = LOSE_YIELD_DEFAULT;
    mDuration  = DURATION_DEFAULT;
}

void SellDecision3Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save SellDecision3Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",   mEnabled);
    settingsEditor->setValue(type + "/LoseYield", mLoseYield);
    settingsEditor->setValue(type + "/Duration",  mDuration);
    // clang-format on
}

void SellDecision3Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load SellDecision3Config";

    // clang-format off
    mEnabled   = settingsEditor->value(type + "/Enabled",   mEnabled).toBool();
    mLoseYield = settingsEditor->value(type + "/LoseYield", mLoseYield).toFloat();
    mDuration  = settingsEditor->value(type + "/Duration",  mDuration).toInt();
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

static void configDurationParse(SellDecision3Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
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
    {"loseYield", configLoseYieldParse},
    {"duration",  configDurationParse }
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
    return QString(R"({"enabled":%1,"loseYield":"%2","duration":%3})")
        .arg(mEnabled ? "true" : "false", QString::number(mLoseYield, 'f', 2), QString::number(mDuration));
}

QStringList SellDecision3Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList loseYieldVariants = {"5.00", "7.00", "10.00", "12.00", "15.00"};
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

void SellDecision3Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int SellDecision3Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
