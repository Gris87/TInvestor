#include "buydecision1config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



BuyDecision1Config::BuyDecision1Config()
{
    qDebug() << "Create BuyDecision1Config";
}

BuyDecision1Config::~BuyDecision1Config()
{
    qDebug() << "Destroy BuyDecision1Config";
}

BuyDecision1Config& BuyDecision1Config::operator=(const BuyDecision1Config &config)
{
    assign(config);

    return *this;
}

void BuyDecision1Config::makeDefault()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Set BuyDecision1Config to default";

    mEnabled     = true;
    mIncomeAbove = 1.0f;
}

void BuyDecision1Config::assign(const BuyDecision1Config &config)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Assigning BuyDecision1Config to BuyDecision1Config";

    mEnabled     = config.mEnabled;
    mIncomeAbove = config.mIncomeAbove;
}

void BuyDecision1Config::save(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Save BuyDecision1Config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Enabled",     mEnabled);
    settings.setValue(type + "/IncomeAbove", mIncomeAbove);
}

void BuyDecision1Config::load(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Load BuyDecision1Config";

    QSettings settings("GrisCom", "TInvestor");

    mEnabled     = settings.value(type + "/Enabled",     mEnabled).toBool();
    mIncomeAbove = settings.value(type + "/IncomeAbove", mIncomeAbove).toFloat();
}

void BuyDecision1Config::setEnabled(bool value)
{
    QMutexLocker lock(&mMutex);

    mEnabled = value;
}

bool BuyDecision1Config::isEnabled()
{
    QMutexLocker lock(&mMutex);

    return mEnabled;
}

void BuyDecision1Config::setIncomeAbove(float value)
{
    QMutexLocker lock(&mMutex);

    mIncomeAbove = value;
}

float BuyDecision1Config::getIncomeAbove()
{
    QMutexLocker lock(&mMutex);

    return mIncomeAbove;
}
