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
    mIncomeAbove = 1.0f;
}

void BuyDecision3Config::assign(const BuyDecision3Config &config)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Assigning BuyDecision3Config to BuyDecision3Config";

    mEnabled     = config.mEnabled;
    mIncomeAbove = config.mIncomeAbove;
}

void BuyDecision3Config::save(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Save BuyDecision3Config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Enabled",     mEnabled);
    settings.setValue(type + "/IncomeAbove", mIncomeAbove);
}

void BuyDecision3Config::load(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Load BuyDecision3Config";

    QSettings settings("GrisCom", "TInvestor");

    mEnabled     = settings.value(type + "/Enabled",     mEnabled).toBool();
    mIncomeAbove = settings.value(type + "/IncomeAbove", mIncomeAbove).toFloat();
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

void BuyDecision3Config::setIncomeAbove(float value)
{
    QMutexLocker lock(&mMutex);

    mIncomeAbove = value;
}

float BuyDecision3Config::getIncomeAbove()
{
    QMutexLocker lock(&mMutex);

    return mIncomeAbove;
}
