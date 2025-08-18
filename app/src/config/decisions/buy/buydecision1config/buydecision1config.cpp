#include "src/config/decisions/buy/buydecision1config/buydecision1config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



// For 2020-2024 years
constexpr bool  ENABLED_DEFAULT    = true;
constexpr float PRICE_FALL_DEFAULT = 3.0f;
constexpr int   DURATION_DEFAULT   = 30;

// For 2024 year
// constexpr bool  ENABLED_DEFAULT    = true;
// constexpr float PRICE_FALL_DEFAULT = 3.0f;
// constexpr int   DURATION_DEFAULT   = 120;

// For 2019 year
// constexpr bool  ENABLED_DEFAULT    = true;
// constexpr float PRICE_FALL_DEFAULT = 2.0f;
// constexpr int   DURATION_DEFAULT   = 240;



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

void BuyDecision1Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision1Config to default";

    mEnabled   = ENABLED_DEFAULT;
    mPriceFall = PRICE_FALL_DEFAULT;
    mDuration  = DURATION_DEFAULT;
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

    const QStringList priceFallVariants = {"2.00", "3.00", "4.00", "5.00"};
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
