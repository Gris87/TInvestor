#include "src/config/decisions/buy/buydecision3config/buydecision3config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr int COMMISSION_30 = 30;
constexpr int COMMISSION_5  = 5;

// For commission 0.30%
constexpr bool  ENABLED_DEFAULT_30    = true;
constexpr float PRICE_FALL_DEFAULT_30 = 5.0f;
constexpr int   DURATION_DEFAULT_30   = 3;

// For commission 0.05%
constexpr bool  ENABLED_DEFAULT_5    = true;
constexpr float PRICE_FALL_DEFAULT_5 = 5.0f;
constexpr int   DURATION_DEFAULT_5   = 3;

// For commission 0.04%
constexpr bool  ENABLED_DEFAULT_4    = true;
constexpr float PRICE_FALL_DEFAULT_4 = 5.0f;
constexpr int   DURATION_DEFAULT_4   = 3;



BuyDecision3Config::BuyDecision3Config() :
    IBuyDecision3Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mPriceFall(),
    mDuration()
{
    qDebug() << "Create BuyDecision3Config";
}

BuyDecision3Config::~BuyDecision3Config()
{
    qDebug() << "Destroy BuyDecision3Config";

    delete mRwMutex;
}

IBuyDecision3Config* BuyDecision3Config::clone()
{
    BuyDecision3Config* res = new BuyDecision3Config();
    res->assign(this);

    return res;
}

void BuyDecision3Config::deleteRecursively()
{
    delete this;
}

void BuyDecision3Config::assign(IBuyDecision3Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision3Config to BuyDecision3Config";

    const BuyDecision3Config& config = *dynamic_cast<BuyDecision3Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled   = config.mEnabled;
    mPriceFall = config.mPriceFall;
    mDuration  = config.mDuration;
}

void BuyDecision3Config::makeDefault(int commissionInt)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision3Config to default";

    if (commissionInt == COMMISSION_30)
    {
        mEnabled   = ENABLED_DEFAULT_30;
        mPriceFall = PRICE_FALL_DEFAULT_30;
        mDuration  = DURATION_DEFAULT_30;
    }
    else if (commissionInt == COMMISSION_5)
    {
        mEnabled   = ENABLED_DEFAULT_5;
        mPriceFall = PRICE_FALL_DEFAULT_5;
        mDuration  = DURATION_DEFAULT_5;
    }
    else
    {
        mEnabled   = ENABLED_DEFAULT_4;
        mPriceFall = PRICE_FALL_DEFAULT_4;
        mDuration  = DURATION_DEFAULT_4;
    }
}

void BuyDecision3Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision3Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",   mEnabled);
    settingsEditor->setValue(type + "/PriceFall", mPriceFall);
    settingsEditor->setValue(type + "/Duration",  mDuration);
    // clang-format on
}

void BuyDecision3Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

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

    const QStringList priceFallVariants = {"5.00", "7.00", "10.00", "12.00", "15.00"};
    const QStringList durationVariants  = {"3", "7", "14", "30", "60", "90"};

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
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision3Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision3Config::setPriceFall(float value)
{
    const QWriteLocker lock(mRwMutex);

    mPriceFall = value;
}

float BuyDecision3Config::getPriceFall()
{
    const QReadLocker lock(mRwMutex);

    return mPriceFall;
}

void BuyDecision3Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int BuyDecision3Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
