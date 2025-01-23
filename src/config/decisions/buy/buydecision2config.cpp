#include "buydecision2config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



BuyDecision2Config::BuyDecision2Config() :
    mMutex()
{
    qDebug() << "Create BuyDecision2Config";
}

BuyDecision2Config::~BuyDecision2Config()
{
    qDebug() << "Destroy BuyDecision2Config";
}

BuyDecision2Config& BuyDecision2Config::operator=(const BuyDecision2Config &config)
{
    assign(config);

    return *this;
}

void BuyDecision2Config::makeDefault()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Set BuyDecision2Config to default";

    mEnabled       = true;
    mPriceDiff     = 1.0f;
    mAmountOfTimes = 2;
    mDuration      = 15;
}

void BuyDecision2Config::assign(const BuyDecision2Config &config)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Assigning BuyDecision2Config to BuyDecision2Config";

    mEnabled       = config.mEnabled;
    mPriceDiff     = config.mPriceDiff;
    mAmountOfTimes = config.mAmountOfTimes;
    mDuration      = config.mDuration;
}

void BuyDecision2Config::save(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Save BuyDecision2Config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Enabled",       mEnabled);
    settings.setValue(type + "/PriceDiff",     mPriceDiff);
    settings.setValue(type + "/AmountOfTimes", mAmountOfTimes);
    settings.setValue(type + "/Duration",      mDuration);
}

void BuyDecision2Config::load(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Load BuyDecision2Config";

    QSettings settings("GrisCom", "TInvestor");

    mEnabled       = settings.value(type + "/Enabled",       mEnabled).toBool();
    mPriceDiff     = settings.value(type + "/PriceDiff",     mPriceDiff).toFloat();
    mAmountOfTimes = settings.value(type + "/AmountOfTimes", mAmountOfTimes).toInt();
    mDuration      = settings.value(type + "/Duration",      mDuration).toInt();
}

void BuyDecision2Config::setEnabled(bool value)
{
    QMutexLocker lock(&mMutex);

    mEnabled = value;
}

bool BuyDecision2Config::isEnabled()
{
    QMutexLocker lock(&mMutex);

    return mEnabled;
}

void BuyDecision2Config::setPriceDiff(float value)
{
    QMutexLocker lock(&mMutex);

    mPriceDiff = value;
}

float BuyDecision2Config::getPriceDiff()
{
    QMutexLocker lock(&mMutex);

    return mPriceDiff;
}

void BuyDecision2Config::setAmountOfTimes(int value)
{
    QMutexLocker lock(&mMutex);

    mAmountOfTimes = value;
}

int BuyDecision2Config::getAmountOfTimes()
{
    QMutexLocker lock(&mMutex);

    return mAmountOfTimes;
}

void BuyDecision2Config::setDuration(int value)
{
    QMutexLocker lock(&mMutex);

    mDuration = value;
}

int BuyDecision2Config::getDuration()
{
    QMutexLocker lock(&mMutex);

    return mDuration;
}
