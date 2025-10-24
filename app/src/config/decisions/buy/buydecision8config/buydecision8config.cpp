#include "src/config/decisions/buy/buydecision8config/buydecision8config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr int COMMISSION_30 = 30;
constexpr int COMMISSION_5  = 5;

// For commission 0.30%
constexpr bool ENABLED_DEFAULT_30  = true;
constexpr int  DURATION_DEFAULT_30 = 30;

// For commission 0.05%
constexpr bool ENABLED_DEFAULT_5  = true;
constexpr int  DURATION_DEFAULT_5 = 30;

// For commission 0.04%
constexpr bool ENABLED_DEFAULT_4  = true;
constexpr int  DURATION_DEFAULT_4 = 30;



BuyDecision8Config::BuyDecision8Config() :
    IBuyDecision8Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
    mDuration()
{
    qDebug() << "Create BuyDecision8Config";
}

BuyDecision8Config::~BuyDecision8Config()
{
    qDebug() << "Destroy BuyDecision8Config";

    delete mRwMutex;
}

IBuyDecision8Config* BuyDecision8Config::clone()
{
    BuyDecision8Config* res = new BuyDecision8Config();
    res->assign(this);

    return res;
}

void BuyDecision8Config::deleteRecursively()
{
    delete this;
}

void BuyDecision8Config::assign(IBuyDecision8Config* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning BuyDecision8Config to BuyDecision8Config";

    const BuyDecision8Config& config = *dynamic_cast<BuyDecision8Config*>(another);
    const QReadLocker         lock2(config.mRwMutex);

    mEnabled  = config.mEnabled;
    mDuration = config.mDuration;
}

void BuyDecision8Config::makeDefault(int commissionInt)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision8Config to default";

    if (commissionInt == COMMISSION_30)
    {
        mEnabled  = ENABLED_DEFAULT_30;
        mDuration = DURATION_DEFAULT_30;
    }
    else if (commissionInt == COMMISSION_5)
    {
        mEnabled  = ENABLED_DEFAULT_5;
        mDuration = DURATION_DEFAULT_5;
    }
    else
    {
        mEnabled  = ENABLED_DEFAULT_4;
        mDuration = DURATION_DEFAULT_4;
    }
}

void BuyDecision8Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision8Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",  mEnabled);
    settingsEditor->setValue(type + "/Duration", mDuration);
    // clang-format on
}

void BuyDecision8Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision8Config";

    // clang-format off
    mEnabled  = settingsEditor->value(type + "/Enabled",  mEnabled).toBool();
    mDuration = settingsEditor->value(type + "/Duration", mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision8Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configDurationParse(BuyDecision8Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    BuyDecision8Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BuyDecision8Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",  configEnabledParse },
    {"duration", configDurationParse}
};
// clang-format on

void BuyDecision8Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString BuyDecision8Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"duration":%2})").arg(mEnabled ? "true" : "false", QString::number(mDuration));
}

QStringList BuyDecision8Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList durationVariants = {"15", "30", "60", "120", "180"};

    for (const QString& duration : durationVariants)
    {
        res.append(QString(R"({"enabled":true,"duration":%1})").arg(duration));
    }

    return res;
}

void BuyDecision8Config::setEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mEnabled = value;
}

bool BuyDecision8Config::isEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mEnabled;
}

void BuyDecision8Config::setDuration(int value)
{
    const QWriteLocker lock(mRwMutex);

    mDuration = value;
}

int BuyDecision8Config::getDuration()
{
    const QReadLocker lock(mRwMutex);

    return mDuration;
}
