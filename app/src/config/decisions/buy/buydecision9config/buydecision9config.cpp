#include "src/config/decisions/buy/buydecision9config/buydecision9config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr bool  ENABLED_DEFAULT   = true;
constexpr float RSI_MONTH_DEFAULT = 70.0f;
constexpr float RSI_WEEK_DEFAULT  = 60.0f;
constexpr float RSI_DAY_DEFAULT   = 60.0f;



BuyDecision9Config::BuyDecision9Config() :
    IBuyDecision9Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mRsiMonth(),
    mRsiWeek(),
    mRsiDay()
{
    qDebug() << "Create BuyDecision9Config";
}

BuyDecision9Config::~BuyDecision9Config()
{
    qDebug() << "Destroy BuyDecision9Config";

    delete mRwMutex;
}

IBuyDecision9Config* BuyDecision9Config::clone()
{
    BuyDecision9Config* res = new BuyDecision9Config();
    res->assign(this);

    return res;
}

void BuyDecision9Config::deleteRecursively()
{
    delete this;
}

void BuyDecision9Config::assign(IBuyDecision9Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision9Config to BuyDecision9Config";

    const BuyDecision9Config& config = *dynamic_cast<BuyDecision9Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled  = config.mEnabled;
    mRsiMonth = config.mRsiMonth;
    mRsiWeek  = config.mRsiWeek;
    mRsiDay   = config.mRsiDay;
}

void BuyDecision9Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision9Config to default";

    mEnabled  = ENABLED_DEFAULT;
    mRsiMonth = RSI_MONTH_DEFAULT;
    mRsiWeek  = RSI_WEEK_DEFAULT;
    mRsiDay   = RSI_DAY_DEFAULT;
}

void BuyDecision9Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision9Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",  mEnabled);
    settingsEditor->setValue(type + "/RsiMonth", mRsiMonth);
    settingsEditor->setValue(type + "/RsiWeek",  mRsiWeek);
    settingsEditor->setValue(type + "/RsiDay",   mRsiDay);
    // clang-format on
}

void BuyDecision9Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision9Config";

    // clang-format off
    mEnabled  = settingsEditor->value(type + "/Enabled",  mEnabled).toBool();
    mRsiMonth = settingsEditor->value(type + "/RsiMonth", mRsiMonth).toFloat();
    mRsiWeek  = settingsEditor->value(type + "/RsiWeek",  mRsiWeek).toFloat();
    mRsiDay   = settingsEditor->value(type + "/RsiDay",   mRsiDay).toFloat();
    // clang-format on
}

static void configEnabledParse(BuyDecision9Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configRsiMonthParse(BuyDecision9Config* config, simdjson::ondemand::value value)
{
    config->setRsiMonth(value.get_double_in_string());
}

static void configRsiWeekParse(BuyDecision9Config* config, simdjson::ondemand::value value)
{
    config->setRsiWeek(value.get_double_in_string());
}

static void configRsiDayParse(BuyDecision9Config* config, simdjson::ondemand::value value)
{
    config->setRsiDay(value.get_double_in_string());
}

static void configThrowParseException(
    BuyDecision9Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision9Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",  configEnabledParse },
    {"rsiMonth", configRsiMonthParse},
    {"rsiWeek",  configRsiWeekParse },
    {"rsiDay",   configRsiDayParse  }
};
// clang-format on

void BuyDecision9Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision9Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"rsiMonth":"%2","rsiWeek":"%3","rsiDay":"%4"})")
        .arg(
            mEnabled ? "true" : "false",
            QString::number(mRsiMonth, 'f', 2),
            QString::number(mRsiWeek, 'f', 2),
            QString::number(mRsiDay, 'f', 2)
        );
}

QStringList BuyDecision9Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList rsiMonthVariants = {"60.00", "70.00", "80.00"};
    const QStringList rsiWeekVariants  = {"60.00", "70.00", "80.00"};
    const QStringList rsiDayVariants   = {"60.00", "70.00", "80.00"};

    for (const QString& rsiMonth : rsiMonthVariants)
    {
        for (const QString& rsiWeek : rsiWeekVariants)
        {
            for (const QString& rsiDay : rsiDayVariants)
            {
                res.append(
                    QString(R"({"enabled":true,"rsiMonth":"%1","rsiWeek":"%2","rsiDay":"%3"})").arg(rsiMonth, rsiWeek, rsiDay)
                );
            }
        }
    }

    return res;
}

void BuyDecision9Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision9Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision9Config::setRsiMonth(float value)
{
    const QWriteLocker lock(mRwMutex);

    mRsiMonth = value;
}

float BuyDecision9Config::getRsiMonth()
{
    const QReadLocker lock(mRwMutex);

    return mRsiMonth;
}

void BuyDecision9Config::setRsiWeek(float value)
{
    const QWriteLocker lock(mRwMutex);

    mRsiWeek = value;
}

float BuyDecision9Config::getRsiWeek()
{
    const QReadLocker lock(mRwMutex);

    return mRsiWeek;
}

void BuyDecision9Config::setRsiDay(float value)
{
    const QWriteLocker lock(mRwMutex);

    mRsiDay = value;
}

float BuyDecision9Config::getRsiDay()
{
    const QReadLocker lock(mRwMutex);

    return mRsiDay;
}
