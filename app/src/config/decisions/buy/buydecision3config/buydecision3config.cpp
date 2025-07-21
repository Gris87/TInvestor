#include "src/config/decisions/buy/buydecision3config/buydecision3config.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/utils/exception/exception.h"



constexpr bool  ENABLED_DEFAULT    = true;
constexpr float PRICE_RISE_DEFAULT = 2.0f;
constexpr int   DURATION_DEFAULT   = 1;



BuyDecision3Config::BuyDecision3Config() :
    IBuyDecision3Config(),
    mMutex(new QMutex()),
    mEnabled(),
    mPriceRise(),
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
    mPriceRise = config.mPriceRise;
    mDuration  = config.mDuration;
}

void BuyDecision3Config::makeDefault()
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Set BuyDecision3Config to default";

    mEnabled   = ENABLED_DEFAULT;
    mPriceRise = PRICE_RISE_DEFAULT;
    mDuration  = DURATION_DEFAULT;
}

void BuyDecision3Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Save BuyDecision3Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",   mEnabled);
    settingsEditor->setValue(type + "/PriceRise", mPriceRise);
    settingsEditor->setValue(type + "/Duration",  mDuration);
    // clang-format on
}

void BuyDecision3Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Load BuyDecision3Config";

    // clang-format off
    mEnabled   = settingsEditor->value(type + "/Enabled",   mEnabled).toBool();
    mPriceRise = settingsEditor->value(type + "/PriceRise", mPriceRise).toFloat();
    mDuration  = settingsEditor->value(type + "/Duration",  mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision3Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configPriceRiseParse(BuyDecision3Config* config, simdjson::ondemand::value value)
{
    config->setPriceRise(value.get_double_in_string());
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
    {"priceRise", configPriceRiseParse},
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
    return QString(R"({"enabled":%1,"priceRise":"%2","duration":%3})")
        .arg(mEnabled ? "true" : "false", QString::number(mPriceRise, 'f', 2), QString::number(mDuration));
}

QStringList BuyDecision3Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList priceRiseVariants = {"3.00", "4.00", "5.00"};
    const QStringList durationVariants  = {"5", "15", "30"};

    for (const QString& priceRise : priceRiseVariants)
    {
        for (const QString& duration : durationVariants)
        {
            res.append(QString(R"({"enabled":true,"priceRise":"%1","duration":%2})").arg(priceRise, duration));
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

void BuyDecision3Config::setPriceRise(float value)
{
    const QMutexLocker lock(mMutex);

    mPriceRise = value;
}

float BuyDecision3Config::getPriceRise()
{
    const QMutexLocker lock(mMutex);

    return mPriceRise;
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
