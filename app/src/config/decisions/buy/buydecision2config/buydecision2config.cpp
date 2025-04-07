#include "src/config/decisions/buy/buydecision2config/buydecision2config.h"

#include <QDebug>
#include <QMutexLocker>



BuyDecision2Config::BuyDecision2Config() :
    IBuyDecision2Config(),
    mMutex(new QMutex())
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

    mEnabled       = true;
    mPriceDiff     = 1.0f;
    mAmountOfTimes = 2;
    mDuration      = 15;
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
