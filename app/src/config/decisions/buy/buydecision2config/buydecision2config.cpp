#include "src/config/decisions/buy/buydecision2config/buydecision2config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



// For 2024 year
constexpr bool  ENABLED_DEFAULT    = true;
constexpr float PRICE_FALL_DEFAULT = 4.0f;
constexpr float LOSE_YIELD_DEFAULT = 0.5f;
constexpr int   DURATION_DEFAULT   = 120;



BuyDecision2Config::BuyDecision2Config() :
    IBuyDecision2Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mPriceFall(),
    mLoseYield(),
    mDuration()
{
    qDebug() << "Create BuyDecision2Config";
}

BuyDecision2Config::~BuyDecision2Config()
{
    qDebug() << "Destroy BuyDecision2Config";

    delete mRwMutex;
}

IBuyDecision2Config* BuyDecision2Config::clone()
{
    BuyDecision2Config* res = new BuyDecision2Config();
    res->assign(this);

    return res;
}

void BuyDecision2Config::deleteRecursively()
{
    delete this;
}

void BuyDecision2Config::assign(IBuyDecision2Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision2Config to BuyDecision2Config";

    const BuyDecision2Config& config = *dynamic_cast<BuyDecision2Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled   = config.mEnabled;
    mPriceFall = config.mPriceFall;
    mLoseYield = config.mLoseYield;
    mDuration  = config.mDuration;
}

void BuyDecision2Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision2Config to default";

    mEnabled   = ENABLED_DEFAULT;
    mPriceFall = PRICE_FALL_DEFAULT;
    mLoseYield = LOSE_YIELD_DEFAULT;
    mDuration  = DURATION_DEFAULT;
}

void BuyDecision2Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision2Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",   mEnabled);
    settingsEditor->setValue(type + "/PriceFall", mPriceFall);
    settingsEditor->setValue(type + "/LoseYield", mLoseYield);
    settingsEditor->setValue(type + "/Duration",  mDuration);
    // clang-format on
}

void BuyDecision2Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision2Config";

    // clang-format off
    mEnabled   = settingsEditor->value(type + "/Enabled",   mEnabled).toBool();
    mPriceFall = settingsEditor->value(type + "/PriceFall", mPriceFall).toFloat();
    mLoseYield = settingsEditor->value(type + "/LoseYield", mLoseYield).toFloat();
    mDuration  = settingsEditor->value(type + "/Duration",  mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision2Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configPriceFallParse(BuyDecision2Config* config, simdjson::ondemand::value value)
{
    config->setPriceFall(value.get_double_in_string());
}

static void configLoseYieldParse(BuyDecision2Config* config, simdjson::ondemand::value value)
{
    config->setLoseYield(value.get_double_in_string());
}

static void configDurationParse(BuyDecision2Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    BuyDecision2Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision2Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",   configEnabledParse  },
    {"priceFall", configPriceFallParse},
    {"loseYield", configLoseYieldParse},
    {"duration",  configDurationParse }
};
// clang-format on

void BuyDecision2Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision2Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"priceFall":"%2","loseYield":"%3","duration":%4})")
        .arg(
            mEnabled ? "true" : "false",
            QString::number(mPriceFall, 'f', 2),
            QString::number(mLoseYield, 'f', 2),
            QString::number(mDuration)
        );
}

QStringList BuyDecision2Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList priceFallVariants = {"2.00", "3.00", "4.00", "5.00"};
    const QStringList loseYieldVariants = {"0.3", "0.5", "0.7"};
    const QStringList durationVariants  = {"5", "15", "30", "60", "120", "180", "240"};

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

void BuyDecision2Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision2Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision2Config::setPriceFall(float value)
{
    const QWriteLocker lock(mRwMutex);

    mPriceFall = value;
}

float BuyDecision2Config::getPriceFall()
{
    const QReadLocker lock(mRwMutex);

    return mPriceFall;
}

void BuyDecision2Config::setLoseYield(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLoseYield = value;
}

float BuyDecision2Config::getLoseYield()
{
    const QReadLocker lock(mRwMutex);

    return mLoseYield;
}

void BuyDecision2Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int BuyDecision2Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
