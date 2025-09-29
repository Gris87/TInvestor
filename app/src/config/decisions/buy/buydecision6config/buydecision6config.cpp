#include "src/config/decisions/buy/buydecision6config/buydecision6config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr bool  ENABLED_DEFAULT              = true;
constexpr float PRICE_RAISE_DEFAULT          = 2.0f;
constexpr float ORDER_BOOK_POSITIONS_DEFAULT = 10;
constexpr int   DURATION_DEFAULT             = 5;



BuyDecision6Config::BuyDecision6Config() :
    IBuyDecision6Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mPriceRaise(),
    mOrderBookPositions(),
    mDuration()
{
    qDebug() << "Create BuyDecision6Config";
}

BuyDecision6Config::~BuyDecision6Config()
{
    qDebug() << "Destroy BuyDecision6Config";

    delete mRwMutex;
}

IBuyDecision6Config* BuyDecision6Config::clone()
{
    BuyDecision6Config* res = new BuyDecision6Config();
    res->assign(this);

    return res;
}

void BuyDecision6Config::deleteRecursively()
{
    delete this;
}

void BuyDecision6Config::assign(IBuyDecision6Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision6Config to BuyDecision6Config";

    const BuyDecision6Config& config = *dynamic_cast<BuyDecision6Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled            = config.mEnabled;
    mPriceRaise         = config.mPriceRaise;
    mOrderBookPositions = config.mOrderBookPositions;
    mDuration           = config.mDuration;
}

void BuyDecision6Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision6Config to default";

    mEnabled            = ENABLED_DEFAULT;
    mPriceRaise         = PRICE_RAISE_DEFAULT;
    mOrderBookPositions = ORDER_BOOK_POSITIONS_DEFAULT;
    mDuration           = DURATION_DEFAULT;
}

void BuyDecision6Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision6Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",            mEnabled);
    settingsEditor->setValue(type + "/PriceRaise",         mPriceRaise);
    settingsEditor->setValue(type + "/OrderBookPositions", mOrderBookPositions);
    settingsEditor->setValue(type + "/Duration",           mDuration);
    // clang-format on
}

void BuyDecision6Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision6Config";

    // clang-format off
    mEnabled            = settingsEditor->value(type + "/Enabled",            mEnabled).toBool();
    mPriceRaise         = settingsEditor->value(type + "/PriceRaise",         mPriceRaise).toFloat();
    mOrderBookPositions = settingsEditor->value(type + "/OrderBookPositions", mOrderBookPositions).toInt();
    mDuration           = settingsEditor->value(type + "/Duration",           mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision6Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configPriceRaiseParse(BuyDecision6Config* config, simdjson::ondemand::value value)
{
    config->setPriceRaise(value.get_double_in_string());
}

static void configOrderBookPositionsParse(BuyDecision6Config* config, simdjson::ondemand::value value)
{
    config->setOrderBookPositions(value.get_int64());
}

static void configDurationParse(BuyDecision6Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    BuyDecision6Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision6Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",            configEnabledParse           },
    {"priceRaise",         configPriceRaiseParse        },
    {"orderBookPositions", configOrderBookPositionsParse},
    {"duration",           configDurationParse          }
};
// clang-format on

void BuyDecision6Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision6Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"priceRaise":"%2","orderBookPositions":%3,"duration":%4})")
        .arg(
            mEnabled ? "true" : "false",
            QString::number(mPriceRaise, 'f', 2),
            QString::number(mOrderBookPositions),
            QString::number(mDuration)
        );
}

QStringList BuyDecision6Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList priceRaiseVariants         = {"1.00", "2.00", "3.00", "4.00", "5.00"};
    const QStringList orderBookPositionsVariants = {"5", "10", "15", "20"};
    const QStringList durationVariants           = {"5", "10", "15"};

    for (const QString& priceRaise : priceRaiseVariants)
    {
        for (const QString& orderBookPositions : orderBookPositionsVariants)
        {
            for (const QString& duration : durationVariants)
            {
                res.append(QString(R"({"enabled":true,"priceRaise":"%1","orderBookPositions":%2,"duration":%3})")
                               .arg(priceRaise, orderBookPositions, duration));
            }
        }
    }

    return res;
}

void BuyDecision6Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision6Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision6Config::setPriceRaise(float value)
{
    const QWriteLocker lock(mRwMutex);

    mPriceRaise = value;
}

float BuyDecision6Config::getPriceRaise()
{
    const QReadLocker lock(mRwMutex);

    return mPriceRaise;
}

void BuyDecision6Config::setOrderBookPositions(int value)
{
    const QWriteLocker lock(mRwMutex);

    mOrderBookPositions = value;
}

int BuyDecision6Config::getOrderBookPositions()
{
    const QReadLocker lock(mRwMutex);

    return mOrderBookPositions;
}

void BuyDecision6Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int BuyDecision6Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
