#include "src/config/decisions/buy/buydecision9config/buydecision9config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr bool  ENABLED_DEFAULT  = true;
constexpr float RSI_DEFAULT      = 25.0f;
constexpr int   DURATION_DEFAULT = 15;



BuyDecision9Config::BuyDecision9Config() :
    IBuyDecision9Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mRsi(),
    mDuration()
{
    qDebug() << "Create BuyDecision9Config";
}

BuyDecision9Config::~BuyDecision9Config()
{
    qDebug() << "Destroy BuyDecision9Config";

    delete mRwMutex;
}

IBuyDecision9Config* BuyDecision9Config::clone()
{
    BuyDecision9Config* res = new BuyDecision9Config();
    res->assign(this);

    return res;
}

void BuyDecision9Config::deleteRecursively()
{
    delete this;
}

void BuyDecision9Config::assign(IBuyDecision9Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision9Config to BuyDecision9Config";

    const BuyDecision9Config& config = *dynamic_cast<BuyDecision9Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled  = config.mEnabled;
    mRsi      = config.mRsi;
    mDuration = config.mDuration;
}

void BuyDecision9Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision9Config to default";

    mEnabled  = ENABLED_DEFAULT;
    mRsi      = RSI_DEFAULT;
    mDuration = DURATION_DEFAULT;
}

void BuyDecision9Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision9Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",  mEnabled);
    settingsEditor->setValue(type + "/Rsi",      mRsi);
    settingsEditor->setValue(type + "/Duration", mDuration);
    // clang-format on
}

void BuyDecision9Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision9Config";

    // clang-format off
    mEnabled  = settingsEditor->value(type + "/Enabled",  mEnabled).toBool();
    mRsi      = settingsEditor->value(type + "/Rsi",      mRsi).toFloat();
    mDuration = settingsEditor->value(type + "/Duration", mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision9Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configRsiParse(BuyDecision9Config* config, simdjson::ondemand::value value)
{
    config->setRsi(value.get_double_in_string());
}

static void configDurationParse(BuyDecision9Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    BuyDecision9Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision9Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",  configEnabledParse },
    {"rsi",      configRsiParse     },
    {"duration", configDurationParse}
};
// clang-format on

void BuyDecision9Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision9Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"rsi":"%2","duration":%3})")
        .arg(mEnabled ? "true" : "false", QString::number(mRsi, 'f', 2), QString::number(mDuration));
}

QStringList BuyDecision9Config::variantsAsJson() const
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

void BuyDecision9Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision9Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision9Config::setRsi(float value)
{
    const QWriteLocker lock(mRwMutex);

    mRsi = value;
}

float BuyDecision9Config::getRsi()
{
    const QReadLocker lock(mRwMutex);

    return mRsi;
}

void BuyDecision9Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int BuyDecision9Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
