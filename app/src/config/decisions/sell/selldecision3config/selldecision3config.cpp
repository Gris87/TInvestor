#include "src/config/decisions/sell/selldecision3config/selldecision3config.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/utils/exception/exception.h"



constexpr bool  ENABLED_DEFAULT     = true;
constexpr float LOSE_INCOME_DEFAULT = 5.0f;
constexpr int   DURATION_DEFAULT    = 3;



SellDecision3Config::SellDecision3Config() :
    ISellDecision3Config(),
    mMutex(new QMutex()),
    mEnabled(),
    mLoseIncome(),
    mDuration()
{
    qDebug() << "Create SellDecision3Config";
}

SellDecision3Config::~SellDecision3Config()
{
    qDebug() << "Destroy SellDecision3Config";

    delete mMutex;
}

void SellDecision3Config::assign(ISellDecision3Config* another)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Assigning SellDecision3Config to SellDecision3Config";

    const SellDecision3Config& config = *dynamic_cast<SellDecision3Config*>(another);

    mEnabled    = config.mEnabled;
    mLoseIncome = config.mLoseIncome;
    mDuration   = config.mDuration;
}

void SellDecision3Config::makeDefault()
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Set SellDecision3Config to default";

    mEnabled    = ENABLED_DEFAULT;
    mLoseIncome = LOSE_INCOME_DEFAULT;
    mDuration   = DURATION_DEFAULT;
}

void SellDecision3Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Save SellDecision3Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",    mEnabled);
    settingsEditor->setValue(type + "/LoseIncome", mLoseIncome);
    settingsEditor->setValue(type + "/Duration",   mDuration);
    // clang-format on
}

void SellDecision3Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Load SellDecision3Config";

    // clang-format off
    mEnabled    = settingsEditor->value(type + "/Enabled",    mEnabled).toBool();
    mLoseIncome = settingsEditor->value(type + "/LoseIncome", mLoseIncome).toFloat();
    mDuration   = settingsEditor->value(type + "/Duration",   mDuration).toInt();
    // clang-format on
}

static void configEnabledParse(SellDecision3Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configLoseIncomeParse(SellDecision3Config* config, simdjson::ondemand::value value)
{
    config->setLoseIncome(value.get_double_in_string());
}

static void configDurationParse(SellDecision3Config* config, simdjson::ondemand::value value)
{
    config->setDuration(value.get_int64());
}

static void configThrowParseException(
    SellDecision3Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(SellDecision3Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",    configEnabledParse   },
    {"loseIncome", configLoseIncomeParse},
    {"duration",   configDurationParse  }
};
// clang-format on

void SellDecision3Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString SellDecision3Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"loseIncome":"%2","duration":%3})")
        .arg(mEnabled ? "true" : "false", QString::number(mLoseIncome, 'f', 2), QString::number(mDuration));
}

QStringList SellDecision3Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList loseIncomeVariants = {"3.00", "4.00", "5.00"};
    const QStringList durationVariants   = {"5", "15", "30"};

    for (const QString& loseIncome : loseIncomeVariants)
    {
        for (const QString& duration : durationVariants)
        {
            res.append(QString(R"({"enabled":true,"loseIncome":"%1","duration":%2})").arg(loseIncome, duration));
        }
    }

    return res;
}

void SellDecision3Config::setEnabled(bool value)
{
    const QMutexLocker lock(mMutex);

    mEnabled = value;
}

bool SellDecision3Config::isEnabled()
{
    const QMutexLocker lock(mMutex);

    return mEnabled;
}

void SellDecision3Config::setLoseIncome(float value)
{
    const QMutexLocker lock(mMutex);

    mLoseIncome = value;
}

float SellDecision3Config::getLoseIncome()
{
    const QMutexLocker lock(mMutex);

    return mLoseIncome;
}

void SellDecision3Config::setDuration(int value)
{
    const QMutexLocker lock(mMutex);

    mDuration = value;
}

int SellDecision3Config::getDuration()
{
    const QMutexLocker lock(mMutex);

    return mDuration;
}
