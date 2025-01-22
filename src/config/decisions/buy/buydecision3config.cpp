#include "buydecision3config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



BuyDecision3Config::BuyDecision3Config()
{
    qDebug() << "Create BuyDecision3Config";
}

BuyDecision3Config::~BuyDecision3Config()
{
    qDebug() << "Destroy BuyDecision3Config";
}

BuyDecision3Config& BuyDecision3Config::operator=(const BuyDecision3Config &config)
{
    assign(config);

    return *this;
}

void BuyDecision3Config::makeDefault()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Set BuyDecision3Config to default";

    mEnabled     = true;
    mPriceRise   = 2.0f;
    mDuration = 1.0f;
}

void BuyDecision3Config::assign(const BuyDecision3Config &config)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Assigning BuyDecision3Config to BuyDecision3Config";

    mEnabled   = config.mEnabled;
    mPriceRise = config.mPriceRise;
    mDuration  = config.mDuration;
}

void BuyDecision3Config::save(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Save BuyDecision3Config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Enabled",   mEnabled);
    settings.setValue(type + "/PriceRise", mPriceRise);
    settings.setValue(type + "/Duration",  mDuration);
}

void BuyDecision3Config::load(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Load BuyDecision3Config";

    QSettings settings("GrisCom", "TInvestor");

    mEnabled   = settings.value(type + "/Enabled",   mEnabled).toBool();
    mPriceRise = settings.value(type + "/PriceRise", mPriceRise).toFloat();
    mDuration  = settings.value(type + "/Duration",  mDuration).toFloat();
}

void BuyDecision3Config::setEnabled(bool value)
{
    QMutexLocker lock(&mMutex);

    mEnabled = value;
}

bool BuyDecision3Config::isEnabled()
{
    QMutexLocker lock(&mMutex);

    return mEnabled;
}

void BuyDecision3Config::setPriceRise(float value)
{
    QMutexLocker lock(&mMutex);

    mPriceRise = value;
}

float BuyDecision3Config::getPriceRise()
{
    QMutexLocker lock(&mMutex);

    return mPriceRise;
}

void BuyDecision3Config::setDuration(int value)
{
    QMutexLocker lock(&mMutex);

    mDuration = value;
}

int BuyDecision3Config::getDuration()
{
    QMutexLocker lock(&mMutex);

    return mDuration;
}
