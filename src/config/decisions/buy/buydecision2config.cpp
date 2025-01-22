#include "buydecision2config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



BuyDecision2Config::BuyDecision2Config()
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

    mEnabled     = true;
    mIncomeAbove = 1.0f;
}

void BuyDecision2Config::assign(const BuyDecision2Config &config)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Assigning BuyDecision2Config to BuyDecision2Config";

    mEnabled     = config.mEnabled;
    mIncomeAbove = config.mIncomeAbove;
}

void BuyDecision2Config::save(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Save BuyDecision2Config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Enabled",     mEnabled);
    settings.setValue(type + "/IncomeAbove", mIncomeAbove);
}

void BuyDecision2Config::load(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Load BuyDecision2Config";

    QSettings settings("GrisCom", "TInvestor");

    mEnabled     = settings.value(type + "/Enabled",     mEnabled).toBool();
    mIncomeAbove = settings.value(type + "/IncomeAbove", mIncomeAbove).toFloat();
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

void BuyDecision2Config::setIncomeAbove(float value)
{
    QMutexLocker lock(&mMutex);

    mIncomeAbove = value;
}

float BuyDecision2Config::getIncomeAbove()
{
    QMutexLocker lock(&mMutex);

    return mIncomeAbove;
}
