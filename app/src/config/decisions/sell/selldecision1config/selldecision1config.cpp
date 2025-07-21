#include "src/config/decisions/sell/selldecision1config/selldecision1config.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/utils/exception/exception.h"



constexpr bool  ENABLED_DEFAULT      = true;
constexpr float INCOME_ABOVE_DEFAULT = 1.0f;



SellDecision1Config::SellDecision1Config() :
    ISellDecision1Config(),
    mMutex(new QMutex()),
    mEnabled(),
    mIncomeAbove()
{
    qDebug() << "Create SellDecision1Config";
}

SellDecision1Config::~SellDecision1Config()
{
    qDebug() << "Destroy SellDecision1Config";

    delete mMutex;
}

void SellDecision1Config::assign(ISellDecision1Config* another)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Assigning SellDecision1Config to SellDecision1Config";

    const SellDecision1Config& config = *dynamic_cast<SellDecision1Config*>(another);

    mEnabled     = config.mEnabled;
    mIncomeAbove = config.mIncomeAbove;
}

void SellDecision1Config::makeDefault()
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Set SellDecision1Config to default";

    mEnabled     = ENABLED_DEFAULT;
    mIncomeAbove = INCOME_ABOVE_DEFAULT;
}

void SellDecision1Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Save SellDecision1Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",     mEnabled);
    settingsEditor->setValue(type + "/IncomeAbove", mIncomeAbove);
    // clang-format on
}

void SellDecision1Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Load SellDecision1Config";

    // clang-format off
    mEnabled     = settingsEditor->value(type + "/Enabled",     mEnabled).toBool();
    mIncomeAbove = settingsEditor->value(type + "/IncomeAbove", mIncomeAbove).toFloat();
    // clang-format on
}

static void configEnabledParse(SellDecision1Config* config, simdjson::ondemand::value value)
{
    config->setEnabled(value.get_bool());
}

static void configIncomeAboveParse(SellDecision1Config* config, simdjson::ondemand::value value)
{
    config->setIncomeAbove(value.get_double_in_string());
}

static void configThrowParseException(
    SellDecision1Config* /*config*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(SellDecision1Config* config, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",     configEnabledParse    },
    {"incomeAbove", configIncomeAboveParse}
};
// clang-format on

void SellDecision1Config::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, configThrowParseException);

        parseHandler(this, field.value());
    }
}

QString SellDecision1Config::toJsonString() const
{
    return QString(R"({"enabled":%1,"incomeAbove":"%2"})")
        .arg(mEnabled ? "true" : "false", QString::number(mIncomeAbove, 'f', 2));
}

QStringList SellDecision1Config::variantsAsJson() const
{
    QStringList res;

    res.append(R"({"enabled":false})");

    const QStringList incomeAboveVariants = {"3.00", "4.00", "5.00"};

    for (const QString& incomeAbove : incomeAboveVariants)
    {
        res.append(QString(R"({"enabled":true,"incomeAbove":"%1"})").arg(incomeAbove));
    }

    return res;
}

void SellDecision1Config::setEnabled(bool value)
{
    const QMutexLocker lock(mMutex);

    mEnabled = value;
}

bool SellDecision1Config::isEnabled()
{
    const QMutexLocker lock(mMutex);

    return mEnabled;
}

void SellDecision1Config::setIncomeAbove(float value)
{
    const QMutexLocker lock(mMutex);

    mIncomeAbove = value;
}

float SellDecision1Config::getIncomeAbove()
{
    const QMutexLocker lock(mMutex);

    return mIncomeAbove;
}
