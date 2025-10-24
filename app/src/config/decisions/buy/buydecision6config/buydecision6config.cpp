#include "src/config/decisions/buy/buydecision6config/buydecision6config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr int COMMISSION_30 = 30;
constexpr int COMMISSION_5  = 5;

// For commission 0.30%
constexpr bool  ENABLED_DEFAULT_30  = true;
constexpr float RSI_DEFAULT_30      = 25.0f;
constexpr int   DURATION_DEFAULT_30 = 15;

// For commission 0.05%
constexpr bool  ENABLED_DEFAULT_5  = true;
constexpr float RSI_DEFAULT_5      = 25.0f;
constexpr int   DURATION_DEFAULT_5 = 15;

// For commission 0.04%
constexpr bool  ENABLED_DEFAULT_4  = true;
constexpr float RSI_DEFAULT_4      = 25.0f;
constexpr int   DURATION_DEFAULT_4 = 15;



BuyDecision6Config::BuyDecision6Config() :
    IBuyDecision6Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mRsi(),
    mDuration()
{
    qDebug() << "Create BuyDecision6Config";
}

BuyDecision6Config::~BuyDecision6Config()
{
    qDebug() << "Destroy BuyDecision6Config";

    delete mRwMutex;
}

IBuyDecision6Config* BuyDecision6Config::clone()
{
    BuyDecision6Config* res = new BuyDecision6Config();
    res->assign(this);

    return res;
}

void BuyDecision6Config::deleteRecursively()
{
    delete this;
}

void BuyDecision6Config::assign(IBuyDecision6Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision6Config to BuyDecision6Config";

    const BuyDecision6Config& config = *dynamic_cast<BuyDecision6Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled  = config.mEnabled;
    mRsi      = config.mRsi;
    mDuration = config.mDuration;
}

void BuyDecision6Config::makeDefault(int commissionInt)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision6Config to default";

    if (commissionInt == COMMISSION_30)
    {
        mEnabled  = ENABLED_DEFAULT_30;
        mRsi      = RSI_DEFAULT_30;
        mDuration = DURATION_DEFAULT_30;
    }
    else if (commissionInt == COMMISSION_5)
    {
        mEnabled  = ENABLED_DEFAULT_5;
        mRsi      = RSI_DEFAULT_5;
        mDuration = DURATION_DEFAULT_5;
    }
    else
    {
        mEnabled  = ENABLED_DEFAULT_4;
        mRsi      = RSI_DEFAULT_4;
        mDuration = DURATION_DEFAULT_4;
    }
}

bool BuyDecision6Config::isDefault(int commissionInt)
{
    BuyDecision6Config defaultConfig;
    defaultConfig.makeDefault(commissionInt);

    const QReadLocker lock(mRwMutex);

    return mEnabled == defaultConfig.mEnabled && mRsi == defaultConfig.mRsi && mDuration == defaultConfig.mDuration;
}

void BuyDecision6Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision6Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",  mEnabled);
    settingsEditor->setValue(type + "/Rsi",      mRsi);
    settingsEditor->setValue(type + "/Duration", mDuration);
    // clang-format on
}

void BuyDecision6Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision6Config";

    // clang-format off
    mEnabled  = settingsEditor->value(type + "/Enabled",  mEnabled).toBool();
    mRsi      = settingsEditor->value(type + "/Rsi",      mRsi).toFloat();
    mDuration = settingsEditor->value(type + "/Duration", mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision6Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configRsiParse(BuyDecision6Config* config, simdjson::ondemand::value value)
{
    config->setRsi(value.get_double_in_string());
}

static void configDurationParse(BuyDecision6Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    BuyDecision6Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision6Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",  configEnabledParse },
    {"rsi",      configRsiParse     },
    {"duration", configDurationParse}
};
// clang-format on

void BuyDecision6Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision6Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"rsi":"%2","duration":%3})")
        .arg(mEnabled ? "true" : "false", QString::number(mRsi, 'f', 2), QString::number(mDuration));
}

QStringList BuyDecision6Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList rsiVariants      = {"20.00", "25.00", "30.00", "35.00"};
    const QStringList durationVariants = {"15", "20", "30"};

    for (const QString& rsi : rsiVariants)
    {
        for (const QString& duration : durationVariants)
        {
            res.append(QString(R"({"enabled":true,"rsi":"%1","duration":%2})").arg(rsi, duration));
        }
    }

    return res;
}

void BuyDecision6Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision6Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision6Config::setRsi(float value)
{
    const QWriteLocker lock(mRwMutex);

    mRsi = value;
}

float BuyDecision6Config::getRsi()
{
    const QReadLocker lock(mRwMutex);

    return mRsi;
}

void BuyDecision6Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int BuyDecision6Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
