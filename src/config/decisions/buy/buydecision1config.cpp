#include "buydecision1config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



BuyDecision1Config::BuyDecision1Config() :
    mMutex(new QMutex())
{
    qDebug() << "Create BuyDecision1Config";
}

BuyDecision1Config::~BuyDecision1Config()
{
    qDebug() << "Destroy BuyDecision1Config";

    delete mMutex;
}

BuyDecision1Config& BuyDecision1Config::operator=(const BuyDecision1Config &config)
{
    assign(config);

    return *this;
}

void BuyDecision1Config::makeDefault()
{
    QMutexLocker lock(mMutex);

    qDebug() << "Set BuyDecision1Config to default";

    mEnabled   = true;
    mPriceFall = 1.0f;
    mDuration  = 5;
}

void BuyDecision1Config::assign(const BuyDecision1Config &config)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Assigning BuyDecision1Config to BuyDecision1Config";

    mEnabled   = config.mEnabled;
    mPriceFall = config.mPriceFall;
    mDuration  = config.mDuration;
}

void BuyDecision1Config::save(const QString &type)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Save BuyDecision1Config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Enabled",   mEnabled);
    settings.setValue(type + "/PriceFall", mPriceFall);
    settings.setValue(type + "/Duration",  mDuration);
}

void BuyDecision1Config::load(const QString &type)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Load BuyDecision1Config";

    QSettings settings("GrisCom", "TInvestor");

    mEnabled   = settings.value(type + "/Enabled",   mEnabled).toBool();
    mPriceFall = settings.value(type + "/PriceFall", mPriceFall).toFloat();
    mDuration  = settings.value(type + "/Duration",  mDuration).toInt();
}

void BuyDecision1Config::setEnabled(bool value)
{
    QMutexLocker lock(mMutex);

    mEnabled = value;
}

bool BuyDecision1Config::isEnabled()
{
    QMutexLocker lock(mMutex);

    return mEnabled;
}

void BuyDecision1Config::setPriceFall(float value)
{
    QMutexLocker lock(mMutex);

    mPriceFall = value;
}

float BuyDecision1Config::getPriceFall()
{
    QMutexLocker lock(mMutex);

    return mPriceFall;
}

void BuyDecision1Config::setDuration(int value)
{
    QMutexLocker lock(mMutex);

    mDuration = value;
}

int BuyDecision1Config::getDuration()
{
    QMutexLocker lock(mMutex);

    return mDuration;
}
