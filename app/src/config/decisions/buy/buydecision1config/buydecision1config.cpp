#include "src/config/decisions/buy/buydecision1config/buydecision1config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr int COMMISSION_30 = 30;
constexpr int COMMISSION_5  = 5;

// For commission 0.30%
constexpr bool  ENABLED_DEFAULT_30    = true;
constexpr float PRICE_FALL_DEFAULT_30 = 4.5f;
constexpr int   DURATION_DEFAULT_30   = 60;

// For commission 0.05%
constexpr bool  ENABLED_DEFAULT_5    = true;
constexpr float PRICE_FALL_DEFAULT_5 = 4.5f;
constexpr int   DURATION_DEFAULT_5   = 60;

// For commission 0.04%
constexpr bool  ENABLED_DEFAULT_4    = true;
constexpr float PRICE_FALL_DEFAULT_4 = 4.5f;
constexpr int   DURATION_DEFAULT_4   = 60;



BuyDecision1Config::BuyDecision1Config() :
    IBuyDecision1Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mPriceFall(),
    mDuration()
{
    qDebug() << "Create BuyDecision1Config";
}

BuyDecision1Config::~BuyDecision1Config()
{
    qDebug() << "Destroy BuyDecision1Config";

    delete mRwMutex;
}

IBuyDecision1Config* BuyDecision1Config::clone()
{
    BuyDecision1Config* res = new BuyDecision1Config();
    res->assign(this);

    return res;
}

void BuyDecision1Config::deleteRecursively()
{
    delete this;
}

void BuyDecision1Config::assign(IBuyDecision1Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision1Config to BuyDecision1Config";

    const BuyDecision1Config& config = *dynamic_cast<BuyDecision1Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled   = config.mEnabled;
    mPriceFall = config.mPriceFall;
    mDuration  = config.mDuration;
}

void BuyDecision1Config::makeDefault(int commissionInt)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision1Config to default";

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

void BuyDecision1Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision1Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",   mEnabled);
    settingsEditor->setValue(type + "/PriceFall", mPriceFall);
    settingsEditor->setValue(type + "/Duration",  mDuration);
    // clang-format on
}

void BuyDecision1Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision1Config";

    // clang-format off
    mEnabled   = settingsEditor->value(type + "/Enabled",   mEnabled).toBool();
    mPriceFall = settingsEditor->value(type + "/PriceFall", mPriceFall).toFloat();
    mDuration  = settingsEditor->value(type + "/Duration",  mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision1Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configPriceFallParse(BuyDecision1Config* config, simdjson::ondemand::value value)
{
    config->setPriceFall(value.get_double_in_string());
}

static void configDurationParse(BuyDecision1Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    BuyDecision1Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision1Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",   configEnabledParse  },
    {"priceFall", configPriceFallParse},
    {"duration",  configDurationParse }
};
// clang-format on

void BuyDecision1Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision1Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"priceFall":"%2","duration":%3})")
        .arg(mEnabled ? "true" : "false", QString::number(mPriceFall, 'f', 2), QString::number(mDuration));
}

QStringList BuyDecision1Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList priceFallVariants = {"2.00", "2.50", "3.00", "3.50", "4.00"};
    const QStringList durationVariants  = {"5", "15", "30", "60", "120", "180", "240"};

    for (const QString& priceFall : priceFallVariants)
    {
        for (const QString& duration : durationVariants)
        {
            res.append(QString(R"({"enabled":true,"priceFall":"%1","duration":%2})").arg(priceFall, duration));
        }
    }

    return res;
}

void BuyDecision1Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision1Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision1Config::setPriceFall(float value)
{
    const QWriteLocker lock(mRwMutex);

    mPriceFall = value;
}

float BuyDecision1Config::getPriceFall()
{
    const QReadLocker lock(mRwMutex);

    return mPriceFall;
}

void BuyDecision1Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int BuyDecision1Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
