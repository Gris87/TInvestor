#include "src/config/decisions/buy/buydecision5config/buydecision5config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr int COMMISSION_30 = 30;
constexpr int COMMISSION_5  = 5;

// For commission 0.30%
constexpr bool ENABLED_DEFAULT_30  = false;
constexpr int  DURATION_DEFAULT_30 = 15;

// For commission 0.05%
constexpr bool ENABLED_DEFAULT_5  = false;
constexpr int  DURATION_DEFAULT_5 = 15;

// For commission 0.04%
constexpr bool ENABLED_DEFAULT_4  = false;
constexpr int  DURATION_DEFAULT_4 = 15;



BuyDecision5Config::BuyDecision5Config() :
    IBuyDecision5Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mDuration()
{
    qDebug() << "Create BuyDecision5Config";
}

BuyDecision5Config::~BuyDecision5Config()
{
    qDebug() << "Destroy BuyDecision5Config";

    delete mRwMutex;
}

IBuyDecision5Config* BuyDecision5Config::clone()
{
    BuyDecision5Config* res = new BuyDecision5Config();
    res->assign(this);

    return res;
}

void BuyDecision5Config::deleteRecursively()
{
    delete this;
}

void BuyDecision5Config::assign(IBuyDecision5Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision5Config to BuyDecision5Config";

    const BuyDecision5Config& config = *dynamic_cast<BuyDecision5Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled  = config.mEnabled;
    mDuration = config.mDuration;
}

void BuyDecision5Config::makeDefault(int commissionInt)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision5Config to default";

    if (commissionInt == COMMISSION_30)
    {
        mEnabled  = ENABLED_DEFAULT_30;
        mDuration = DURATION_DEFAULT_30;
    }
    else if (commissionInt == COMMISSION_5)
    {
        mEnabled  = ENABLED_DEFAULT_5;
        mDuration = DURATION_DEFAULT_5;
    }
    else
    {
        mEnabled  = ENABLED_DEFAULT_4;
        mDuration = DURATION_DEFAULT_4;
    }
}

bool BuyDecision5Config::isDefault(int commissionInt)
{
    BuyDecision5Config defaultConfig;
    defaultConfig.makeDefault(commissionInt);

    const QReadLocker lock(mRwMutex);

    return mEnabled == defaultConfig.mEnabled && mDuration == defaultConfig.mDuration;
}

void BuyDecision5Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision5Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",  mEnabled);
    settingsEditor->setValue(type + "/Duration", mDuration);
    // clang-format on
}

void BuyDecision5Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision5Config";

    // clang-format off
    mEnabled  = settingsEditor->value(type + "/Enabled",  mEnabled).toBool();
    mDuration = settingsEditor->value(type + "/Duration", mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision5Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configDurationParse(BuyDecision5Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    BuyDecision5Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision5Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",  configEnabledParse },
    {"duration", configDurationParse}
};
// clang-format on

void BuyDecision5Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision5Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"duration":%2})").arg(mEnabled ? "true" : "false", QString::number(mDuration));
}

QStringList BuyDecision5Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList durationVariants = {"15", "20", "30"};

    for (const QString& duration : durationVariants)
    {
        res.append(QString(R"({"enabled":true,"duration":%1})").arg(duration));
    }

    return res;
}

void BuyDecision5Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision5Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision5Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int BuyDecision5Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
