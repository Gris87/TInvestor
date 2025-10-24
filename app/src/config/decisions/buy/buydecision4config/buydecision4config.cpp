#include "src/config/decisions/buy/buydecision4config/buydecision4config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr int COMMISSION_30 = 30;
constexpr int COMMISSION_5  = 5;

// For commission 0.30%
constexpr bool  ENABLED_DEFAULT_30    = true;
constexpr float PRICE_FALL_DEFAULT_30 = 10.0f;
constexpr float LOSE_YIELD_DEFAULT_30 = 1.0f;
constexpr int   DURATION_DEFAULT_30   = 7;

// For commission 0.05%
constexpr bool  ENABLED_DEFAULT_5    = true;
constexpr float PRICE_FALL_DEFAULT_5 = 10.0f;
constexpr float LOSE_YIELD_DEFAULT_5 = 1.0f;
constexpr int   DURATION_DEFAULT_5   = 7;

// For commission 0.04%
constexpr bool  ENABLED_DEFAULT_4    = true;
constexpr float PRICE_FALL_DEFAULT_4 = 10.0f;
constexpr float LOSE_YIELD_DEFAULT_4 = 1.0f;
constexpr int   DURATION_DEFAULT_4   = 7;



BuyDecision4Config::BuyDecision4Config() :
    IBuyDecision4Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mPriceFall(),
    mLoseYield(),
    mDuration()
{
    qDebug() << "Create BuyDecision4Config";
}

BuyDecision4Config::~BuyDecision4Config()
{
    qDebug() << "Destroy BuyDecision4Config";

    delete mRwMutex;
}

IBuyDecision4Config* BuyDecision4Config::clone()
{
    BuyDecision4Config* res = new BuyDecision4Config();
    res->assign(this);

    return res;
}

void BuyDecision4Config::deleteRecursively()
{
    delete this;
}

void BuyDecision4Config::assign(IBuyDecision4Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision4Config to BuyDecision4Config";

    const BuyDecision4Config& config = *dynamic_cast<BuyDecision4Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled   = config.mEnabled;
    mPriceFall = config.mPriceFall;
    mLoseYield = config.mLoseYield;
    mDuration  = config.mDuration;
}

void BuyDecision4Config::makeDefault(int commissionInt)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision4Config to default";

    if (commissionInt == COMMISSION_30)
    {
        mEnabled   = ENABLED_DEFAULT_30;
        mPriceFall = PRICE_FALL_DEFAULT_30;
        mLoseYield = LOSE_YIELD_DEFAULT_30;
        mDuration  = DURATION_DEFAULT_30;
    }
    else if (commissionInt == COMMISSION_5)
    {
        mEnabled   = ENABLED_DEFAULT_5;
        mPriceFall = PRICE_FALL_DEFAULT_5;
        mLoseYield = LOSE_YIELD_DEFAULT_5;
        mDuration  = DURATION_DEFAULT_5;
    }
    else
    {
        mEnabled   = ENABLED_DEFAULT_4;
        mPriceFall = PRICE_FALL_DEFAULT_4;
        mLoseYield = LOSE_YIELD_DEFAULT_4;
        mDuration  = DURATION_DEFAULT_4;
    }
}

void BuyDecision4Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision4Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",   mEnabled);
    settingsEditor->setValue(type + "/PriceFall", mPriceFall);
    settingsEditor->setValue(type + "/LoseYield", mLoseYield);
    settingsEditor->setValue(type + "/Duration",  mDuration);
    // clang-format on
}

void BuyDecision4Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision4Config";

    // clang-format off
    mEnabled   = settingsEditor->value(type + "/Enabled",   mEnabled).toBool();
    mPriceFall = settingsEditor->value(type + "/PriceFall", mPriceFall).toFloat();
    mLoseYield = settingsEditor->value(type + "/LoseYield", mLoseYield).toFloat();
    mDuration  = settingsEditor->value(type + "/Duration",  mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision4Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configPriceFallParse(BuyDecision4Config* config, simdjson::ondemand::value value)
{
    config->setPriceFall(value.get_double_in_string());
}

static void configLoseYieldParse(BuyDecision4Config* config, simdjson::ondemand::value value)
{
    config->setLoseYield(value.get_double_in_string());
}

static void configDurationParse(BuyDecision4Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    BuyDecision4Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision4Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",   configEnabledParse  },
    {"priceFall", configPriceFallParse},
    {"loseYield", configLoseYieldParse},
    {"duration",  configDurationParse }
};
// clang-format on

void BuyDecision4Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision4Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"priceFall":"%2","loseYield":"%3","duration":%4})")
        .arg(
            mEnabled ? "true" : "false",
            QString::number(mPriceFall, 'f', 2),
            QString::number(mLoseYield, 'f', 2),
            QString::number(mDuration)
        );
}

QStringList BuyDecision4Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList priceFallVariants = {"5.00", "7.00", "10.00", "12.00", "15.00"};
    const QStringList loseYieldVariants = {"1.0", "2.0", "3.0"};
    const QStringList durationVariants  = {"3", "7", "14", "30", "60", "90"};

    for (const QString& priceFall : priceFallVariants)
    {
        for (const QString& loseYield : loseYieldVariants)
        {
            for (const QString& duration : durationVariants)
            {
                res.append(QString(R"({"enabled":true,"priceFall":"%1","loseYield":"%2","duration":%3})")
                               .arg(priceFall, loseYield, duration));
            }
        }
    }

    return res;
}

void BuyDecision4Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision4Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision4Config::setPriceFall(float value)
{
    const QWriteLocker lock(mRwMutex);

    mPriceFall = value;
}

float BuyDecision4Config::getPriceFall()
{
    const QReadLocker lock(mRwMutex);

    return mPriceFall;
}

void BuyDecision4Config::setLoseYield(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLoseYield = value;
}

float BuyDecision4Config::getLoseYield()
{
    const QReadLocker lock(mRwMutex);

    return mLoseYield;
}

void BuyDecision4Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int BuyDecision4Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
