#include "src/config/decisions/buy/buydecision3config/buydecision3config.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/utils/exception/exception.h"



constexpr bool  ENABLED_DEFAULT    = true;
constexpr float PRICE_FALL_DEFAULT = 1.0f;
constexpr int   DURATION_DEFAULT   = 5;



BuyDecision3Config::BuyDecision3Config() :
    IBuyDecision3Config(),
    mMutex(new QMutex()),
    mEnabled(),
    mPriceFall(),
    mDuration()
{
    qDebug() << "Create BuyDecision3Config";
}

BuyDecision3Config::~BuyDecision3Config()
{
    qDebug() << "Destroy BuyDecision3Config";

    delete mMutex;
}

void BuyDecision3Config::assign(IBuyDecision3Config* another)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Assigning BuyDecision3Config to BuyDecision3Config";

    const BuyDecision3Config& config = *dynamic_cast<BuyDecision3Config*>(another);

    mEnabled   = config.mEnabled;
    mPriceFall = config.mPriceFall;
    mDuration  = config.mDuration;
}

void BuyDecision3Config::makeDefault()
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Set BuyDecision3Config to default";

    mEnabled   = ENABLED_DEFAULT;
    mPriceFall = PRICE_FALL_DEFAULT;
    mDuration  = DURATION_DEFAULT;
}

void BuyDecision3Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Save BuyDecision3Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",   mEnabled);
    settingsEditor->setValue(type + "/PriceFall", mPriceFall);
    settingsEditor->setValue(type + "/Duration",  mDuration);
    // clang-format on
}

void BuyDecision3Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Load BuyDecision3Config";

    // clang-format off
    mEnabled   = settingsEditor->value(type + "/Enabled",   mEnabled).toBool();
    mPriceFall = settingsEditor->value(type + "/PriceFall", mPriceFall).toFloat();
    mDuration  = settingsEditor->value(type + "/Duration",  mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision3Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configPriceFallParse(BuyDecision3Config* config, simdjson::ondemand::value value)
{
    config->setPriceFall(value.get_double_in_string());
}

static void configDurationParse(BuyDecision3Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    BuyDecision3Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision3Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",   configEnabledParse  },
    {"priceFall", configPriceFallParse},
    {"duration",  configDurationParse }
};
// clang-format on

void BuyDecision3Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision3Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"priceFall":"%2","duration":%3})")
        .arg(mEnabled ? "true" : "false", QString::number(mPriceFall, 'f', 2), QString::number(mDuration));
}

QStringList BuyDecision3Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList priceFallVariants = {"10.00", "15.00", "20.00"};
    const QStringList durationVariants  = {"3", "7", "14"};

    for (const QString& priceFall : priceFallVariants)
    {
        for (const QString& duration : durationVariants)
        {
            res.append(QString(R"({"enabled":true,"priceFall":"%1","duration":%2})").arg(priceFall, duration));
        }
    }

    return res;
}

void BuyDecision3Config::setEnabled(bool value)
{
    const QMutexLocker lock(mMutex);

    mEnabled = value;
}

bool BuyDecision3Config::isEnabled()
{
    const QMutexLocker lock(mMutex);

    return mEnabled;
}

void BuyDecision3Config::setPriceFall(float value)
{
    const QMutexLocker lock(mMutex);

    mPriceFall = value;
}

float BuyDecision3Config::getPriceFall()
{
    const QMutexLocker lock(mMutex);

    return mPriceFall;
}

void BuyDecision3Config::setDuration(int value)
{
    const QMutexLocker lock(mMutex);

    mDuration = value;
}

int BuyDecision3Config::getDuration()
{
    const QMutexLocker lock(mMutex);

    return mDuration;
}
