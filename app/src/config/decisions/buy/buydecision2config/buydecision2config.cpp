#include "src/config/decisions/buy/buydecision2config/buydecision2config.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/utils/exception/exception.h"



constexpr bool  ENABLED_DEFAULT         = true;
constexpr float PRICE_DIFF_DEFAULT      = 1.0f;
constexpr int   AMOUNT_OF_TIMES_DEFAULT = 2;
constexpr int   DURATION_DEFAULT        = 15;



BuyDecision2Config::BuyDecision2Config() :
    IBuyDecision2Config(),
    mMutex(new QMutex()),
    mEnabled(),
    mPriceDiff(),
    mAmountOfTimes(),
    mDuration()
{
    qDebug() << "Create BuyDecision2Config";
}

BuyDecision2Config::~BuyDecision2Config()
{
    qDebug() << "Destroy BuyDecision2Config";

    delete mMutex;
}

void BuyDecision2Config::assign(IBuyDecision2Config* another)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Assigning BuyDecision2Config to BuyDecision2Config";

    const BuyDecision2Config& config = *dynamic_cast<BuyDecision2Config*>(another);

    mEnabled       = config.mEnabled;
    mPriceDiff     = config.mPriceDiff;
    mAmountOfTimes = config.mAmountOfTimes;
    mDuration      = config.mDuration;
}

void BuyDecision2Config::makeDefault()
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Set BuyDecision2Config to default";

    mEnabled       = ENABLED_DEFAULT;
    mPriceDiff     = PRICE_DIFF_DEFAULT;
    mAmountOfTimes = AMOUNT_OF_TIMES_DEFAULT;
    mDuration      = DURATION_DEFAULT;
}

void BuyDecision2Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Save BuyDecision2Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",       mEnabled);
    settingsEditor->setValue(type + "/PriceDiff",     mPriceDiff);
    settingsEditor->setValue(type + "/AmountOfTimes", mAmountOfTimes);
    settingsEditor->setValue(type + "/Duration",      mDuration);
    // clang-format on
}

void BuyDecision2Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Load BuyDecision2Config";

    // clang-format off
    mEnabled       = settingsEditor->value(type + "/Enabled",       mEnabled).toBool();
    mPriceDiff     = settingsEditor->value(type + "/PriceDiff",     mPriceDiff).toFloat();
    mAmountOfTimes = settingsEditor->value(type + "/AmountOfTimes", mAmountOfTimes).toInt();
    mDuration      = settingsEditor->value(type + "/Duration",      mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision2Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configPriceDiffParse(BuyDecision2Config* config, simdjson::ondemand::value value)
{
    config->setPriceDiff(value.get_double_in_string());
}

static void configAmountOfTimesParse(BuyDecision2Config* config, simdjson::ondemand::value value)
{
    config->setAmountOfTimes(value.get_int64());
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
    {"enabled",       configEnabledParse      },
    {"priceDiff",     configPriceDiffParse    },
    {"amountOfTimes", configAmountOfTimesParse},
    {"duration",      configDurationParse     }
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
    return QString(R"({"enabled":%1,"priceDiff":"%2","amountOfTimes":%3,"duration":%4})")
        .arg(
            mEnabled ? "true" : "false",
            QString::number(mPriceDiff, 'f', 2),
            QString::number(mAmountOfTimes),
            QString::number(mDuration)
        );
}

QStringList BuyDecision2Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList priceDiffVariants     = {"3.00", "4.00", "5.00"};
    const QStringList amountOfTimesVariants = {"1", "2", "3"};
    const QStringList durationVariants      = {"5", "15", "30"};

    for (const QString& priceDiff : priceDiffVariants)
    {
        for (const QString& amountOfTimes : amountOfTimesVariants)
        {
            for (const QString& duration : durationVariants)
            {
                res.append(QString(R"({"enabled":true,"priceDiff":"%1","amountOfTimes":%2,"duration":%3})")
                               .arg(priceDiff, amountOfTimes, duration));
            }
        }
    }

    return res;
}

void BuyDecision2Config::setEnabled(bool value)
{
    const QMutexLocker lock(mMutex);

    mEnabled = value;
}

bool BuyDecision2Config::isEnabled()
{
    const QMutexLocker lock(mMutex);

    return mEnabled;
}

void BuyDecision2Config::setPriceDiff(float value)
{
    const QMutexLocker lock(mMutex);

    mPriceDiff = value;
}

float BuyDecision2Config::getPriceDiff()
{
    const QMutexLocker lock(mMutex);

    return mPriceDiff;
}

void BuyDecision2Config::setAmountOfTimes(int value)
{
    const QMutexLocker lock(mMutex);

    mAmountOfTimes = value;
}

int BuyDecision2Config::getAmountOfTimes()
{
    const QMutexLocker lock(mMutex);

    return mAmountOfTimes;
}

void BuyDecision2Config::setDuration(int value)
{
    const QMutexLocker lock(mMutex);

    mDuration = value;
}

int BuyDecision2Config::getDuration()
{
    const QMutexLocker lock(mMutex);

    return mDuration;
}
