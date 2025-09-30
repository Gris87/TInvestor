#include "src/config/decisions/buy/buydecision6config/buydecision6config.h"

#include <QDebug>

#include "src/utils/exception/exception.h"



constexpr bool ENABLED_DEFAULT  = true;
constexpr int  DURATION_DEFAULT = 30;



BuyDecision6Config::BuyDecision6Config() :
    IBuyDecision6Config(),
    mRwMutex(new QReadWriteLock()),
    mEnabled(),
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

    mEnabled  = config.mEnabled;
    mDuration = config.mDuration;
}

void BuyDecision6Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Set BuyDecision6Config to default";

    mEnabled  = ENABLED_DEFAULT;
    mDuration = DURATION_DEFAULT;
}

void BuyDecision6Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QReadLocker lock(mRwMutex);

    qDebug() << "Save BuyDecision6Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",  mEnabled);
    settingsEditor->setValue(type + "/Duration", mDuration);
    // clang-format on
}

void BuyDecision6Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Load BuyDecision6Config";

    // clang-format off
    mEnabled  = settingsEditor->value(type + "/Enabled",  mEnabled).toBool();
    mDuration = settingsEditor->value(type + "/Duration", mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(BuyDecision6Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
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
    {"enabled",  configEnabledParse },
    {"duration", configDurationParse}
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
    return QString(R"({"enabled":%1,"duration":%2})").arg(mEnabled ? "true" : "false", QString::number(mDuration));
}

QStringList BuyDecision6Config::variantsAsJson() const
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
