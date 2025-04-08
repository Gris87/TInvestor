#include "src/config/decisions/buy/buydecision3config/buydecision3config.h"

#include <QDebug>
#include <QMutexLocker>



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
