#include "src/config/decisions/sell/selldecision4config/selldecision4config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



// For 2020-2024 years
constexpr bool  ENABLED_DEFAULT    = true;
constexpr float LOSE_YIELD_DEFAULT = 3.0f;

// For 2024 year
// constexpr bool  ENABLED_DEFAULT    = true;
// constexpr float LOSE_YIELD_DEFAULT = 5.0f;

// For 2019 year
// constexpr bool  ENABLED_DEFAULT    = true;
// constexpr float LOSE_YIELD_DEFAULT = 7.0f;



SellDecision4Config::SellDecision4Config() :
    ISellDecision4Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mLoseYield()
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

    mEnabled   = config.mEnabled;
    mLoseYield = config.mLoseYield;
}

void SellDecision4Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set SellDecision4Config to default";

    mEnabled   = ENABLED_DEFAULT;
    mLoseYield = LOSE_YIELD_DEFAULT;
}

void SellDecision4Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save SellDecision4Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",    mEnabled);
    settingsEditor->setValue(type + "/LoseYield",  mLoseYield);
    // clang-format on
}

void SellDecision4Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load SellDecision4Config";

    // clang-format off
    mEnabled    = settingsEditor->value(type + "/Enabled",    mEnabled).toBool();
    mLoseYield  = settingsEditor->value(type + "/LoseYield",  mLoseYield).toFloat();
    // clang-format on
}

static void configEnabledParse(SellDecision4Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configLoseYieldParse(SellDecision4Config* config, simdjson::ondemand::value value)
{
    config->setLoseYield(value.get_double_in_string());
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
    {"enabled",   configEnabledParse  },
    {"loseYield", configLoseYieldParse}
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
    return QString(R"({"enabled":%1,"loseYield":"%2"})").arg(mEnabled ? "true" : "false", QString::number(mLoseYield, 'f', 2));
}

QStringList SellDecision4Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList loseYieldVariants = {"5.00", "7.00", "10.00"};

    for (const QString& loseYield : loseYieldVariants)
    {
        res.append(QString(R"({"enabled":true,"loseYield":"%1"})").arg(loseYield));
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

void SellDecision4Config::setLoseYield(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLoseYield = value;
}

float SellDecision4Config::getLoseYield()
{
    const QReadLocker lock(mRwMutex);

    return mLoseYield;
}
