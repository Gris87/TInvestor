#include "src/config/decisions/buy/buydecision7config/buydecision7config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr int COMMISSION_30 = 30;
constexpr int COMMISSION_5  = 5;

// For commission 0.30%
constexpr bool  ENABLED_DEFAULT_30     = true;
constexpr float PRICE_RAISE_DEFAULT_30 = 2.0f;
constexpr int   DURATION_DEFAULT_30    = 3;

// For commission 0.05%
constexpr bool  ENABLED_DEFAULT_5     = true;
constexpr float PRICE_RAISE_DEFAULT_5 = 2.0f;
constexpr int   DURATION_DEFAULT_5    = 3;

// For commission 0.04%
constexpr bool  ENABLED_DEFAULT_4     = true;
constexpr float PRICE_RAISE_DEFAULT_4 = 2.0f;
constexpr int   DURATION_DEFAULT_4    = 3;



BuyDecision7Config::BuyDecision7Config() :
    IBuyDecision7Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mPriceRaise(),
    mDuration()
{
    qDebug() << "Create BuyDecision7Config";
}

BuyDecision7Config::~BuyDecision7Config()
{
    qDebug() << "Destroy BuyDecision7Config";

    delete mRwMutex;
}

IBuyDecision7Config* BuyDecision7Config::clone()
{
    BuyDecision7Config* res = new BuyDecision7Config();
    res->assign(this);

    return res;
}

void BuyDecision7Config::deleteRecursively()
{
    delete this;
}

void BuyDecision7Config::assign(IBuyDecision7Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision7Config to BuyDecision7Config";

    const BuyDecision7Config& config = *dynamic_cast<BuyDecision7Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled    = config.mEnabled;
    mPriceRaise = config.mPriceRaise;
    mDuration   = config.mDuration;
}

void BuyDecision7Config::makeDefault(int commissionInt)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision7Config to default";

    if (commissionInt == COMMISSION_30)
    {
        mEnabled    = ENABLED_DEFAULT_30;
        mPriceRaise = PRICE_RAISE_DEFAULT_30;
        mDuration   = DURATION_DEFAULT_30;
    }
    else if (commissionInt == COMMISSION_5)
    {
        mEnabled    = ENABLED_DEFAULT_5;
        mPriceRaise = PRICE_RAISE_DEFAULT_5;
        mDuration   = DURATION_DEFAULT_5;
    }
    else
    {
        mEnabled    = ENABLED_DEFAULT_4;
        mPriceRaise = PRICE_RAISE_DEFAULT_4;
        mDuration   = DURATION_DEFAULT_4;
    }
}

bool BuyDecision7Config::isDefault(int commissionInt)
{
    BuyDecision7Config defaultConfig;
    defaultConfig.makeDefault(commissionInt);

    const QReadLocker lock(mRwMutex);

    return mEnabled == defaultConfig.mEnabled && mPriceRaise == defaultConfig.mPriceRaise && mDuration == defaultConfig.mDuration;
}

void BuyDecision7Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision7Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",    mEnabled);
    settingsEditor->setValue(type + "/PriceRaise", mPriceRaise);
    settingsEditor->setValue(type + "/Duration",   mDuration);
    // clang-format on
}

void BuyDecision7Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision7Config";

    // clang-format off
    mEnabled    = settingsEditor->value(type + "/Enabled",    mEnabled).toBool();
    mPriceRaise = settingsEditor->value(type + "/PriceRaise", mPriceRaise).toFloat();
    mDuration   = settingsEditor->value(type + "/Duration",   mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision7Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configPriceRaiseParse(BuyDecision7Config* config, simdjson::ondemand::value value)
{
    config->setPriceRaise(value.get_double_in_string());
}

static void configDurationParse(BuyDecision7Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    BuyDecision7Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision7Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",    configEnabledParse   },
    {"priceRaise", configPriceRaiseParse},
    {"duration",   configDurationParse  }
};
// clang-format on

void BuyDecision7Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision7Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"priceRaise":"%2","duration":%3})")
        .arg(mEnabled ? "true" : "false", QString::number(mPriceRaise, 'f', 2), QString::number(mDuration));
}

QStringList BuyDecision7Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList priceRaiseVariants = {"2.00", "3.00", "4.00"};
    const QStringList durationVariants   = {"2", "3", "4"};

    for (const QString& priceRaise : priceRaiseVariants)
    {
        for (const QString& duration : durationVariants)
        {
            res.append(QString(R"({"enabled":true,"priceRaise":"%1","duration":%2})").arg(priceRaise, duration));
        }
    }

    return res;
}

void BuyDecision7Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision7Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision7Config::setPriceRaise(float value)
{
    const QWriteLocker lock(mRwMutex);

    mPriceRaise = value;
}

float BuyDecision7Config::getPriceRaise()
{
    const QReadLocker lock(mRwMutex);

    return mPriceRaise;
}

void BuyDecision7Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int BuyDecision7Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
