#include "selldecision3config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



SellDecision3Config::SellDecision3Config()
{
    qDebug() << "Create SellDecision3Config";
}

SellDecision3Config::~SellDecision3Config()
{
    qDebug() << "Destroy SellDecision3Config";
}

SellDecision3Config& SellDecision3Config::operator=(const SellDecision3Config &config)
{
    assign(config);

    return *this;
}

void SellDecision3Config::makeDefault()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Set SellDecision3Config to default";

    mEnabled     = true;
    mIncomeAbove = 1.0f;
}

void SellDecision3Config::assign(const SellDecision3Config &config)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Assigning SellDecision3Config to SellDecision3Config";

    mEnabled     = config.mEnabled;
    mIncomeAbove = config.mIncomeAbove;
}

void SellDecision3Config::save(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Save SellDecision3Config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Enabled",     mEnabled);
    settings.setValue(type + "/IncomeAbove", mIncomeAbove);
}

void SellDecision3Config::load(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Load SellDecision3Config";

    QSettings settings("GrisCom", "TInvestor");

    mEnabled     = settings.value(type + "/Enabled",     mEnabled).toBool();
    mIncomeAbove = settings.value(type + "/IncomeAbove", mIncomeAbove).toFloat();
}

void SellDecision3Config::setEnabled(bool value)
{
    QMutexLocker lock(&mMutex);

    mEnabled = value;
}

bool SellDecision3Config::isEnabled()
{
    QMutexLocker lock(&mMutex);

    return mEnabled;
}

void SellDecision3Config::setIncomeAbove(float value)
{
    QMutexLocker lock(&mMutex);

    mIncomeAbove = value;
}

float SellDecision3Config::getIncomeAbove()
{
    QMutexLocker lock(&mMutex);

    return mIncomeAbove;
}
