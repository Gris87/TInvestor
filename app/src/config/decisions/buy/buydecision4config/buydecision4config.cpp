#include "src/config/decisions/buy/buydecision4config/buydecision4config.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/utils/exception/exception.h"



constexpr bool  ENABLED_DEFAULT    = true;
constexpr float PRICE_FALL_DEFAULT = 1.0f;
constexpr float LOSE_YIELD_DEFAULT = 0.1f;
constexpr int   DURATION_DEFAULT   = 5;



BuyDecision4Config::BuyDecision4Config() :
    IBuyDecision4Config(),
    mMutex(new QMutex()),
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

    delete mMutex;
}

void BuyDecision4Config::assign(IBuyDecision4Config* another)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Assigning BuyDecision4Config to BuyDecision4Config";

    const BuyDecision4Config& config = *dynamic_cast<BuyDecision4Config*>(another);

    mEnabled   = config.mEnabled;
    mPriceFall = config.mPriceFall;
    mLoseYield = config.mLoseYield;
    mDuration  = config.mDuration;
}

void BuyDecision4Config::makeDefault()
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Set BuyDecision4Config to default";

    mEnabled   = ENABLED_DEFAULT;
    mPriceFall = PRICE_FALL_DEFAULT;
    mLoseYield = LOSE_YIELD_DEFAULT;
    mDuration  = DURATION_DEFAULT;
}

void BuyDecision4Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

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
    const QMutexLocker lock(mMutex);

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

    const QStringList priceFallVariants = {"3.00", "4.00", "5.00"};
    const QStringList loseYieldVariants = {"0.1", "0.3", "0.7"};
    const QStringList durationVariants  = {"5", "15", "30"};

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
    const QMutexLocker lock(mMutex);

    mEnabled = value;
}

bool BuyDecision4Config::isEnabled()
{
    const QMutexLocker lock(mMutex);

    return mEnabled;
}

void BuyDecision4Config::setPriceFall(float value)
{
    const QMutexLocker lock(mMutex);

    mPriceFall = value;
}

float BuyDecision4Config::getPriceFall()
{
    const QMutexLocker lock(mMutex);

    return mPriceFall;
}

void BuyDecision4Config::setLoseYield(float value)
{
    const QMutexLocker lock(mMutex);

    mLoseYield = value;
}

float BuyDecision4Config::getLoseYield()
{
    const QMutexLocker lock(mMutex);

    return mLoseYield;
}

void BuyDecision4Config::setDuration(int value)
{
    const QMutexLocker lock(mMutex);

    mDuration = value;
}

int BuyDecision4Config::getDuration()
{
    const QMutexLocker lock(mMutex);

    return mDuration;
}
