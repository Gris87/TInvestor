#include "selldecision1config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



SellDecision1Config::SellDecision1Config() :
    ISellDecision1Config(),
    mMutex(new QMutex())
{
    qDebug() << "Create SellDecision1Config";
}

SellDecision1Config::~SellDecision1Config()
{
    qDebug() << "Destroy SellDecision1Config";

    delete mMutex;
}

void SellDecision1Config::assign(ISellDecision1Config *another)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Assigning SellDecision1Config to SellDecision1Config";

    const SellDecision1Config &config = *dynamic_cast<SellDecision1Config *>(another);

    mEnabled     = config.mEnabled;
    mIncomeAbove = config.mIncomeAbove;
}

void SellDecision1Config::makeDefault()
{
    QMutexLocker lock(mMutex);

    qDebug() << "Set SellDecision1Config to default";

    mEnabled     = true;
    mIncomeAbove = 1.0f;
}

void SellDecision1Config::save(const QString &type)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Save SellDecision1Config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Enabled",     mEnabled);
    settings.setValue(type + "/IncomeAbove", mIncomeAbove);
}

void SellDecision1Config::load(const QString &type)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Load SellDecision1Config";

    QSettings settings("GrisCom", "TInvestor");

    mEnabled     = settings.value(type + "/Enabled",     mEnabled).toBool();
    mIncomeAbove = settings.value(type + "/IncomeAbove", mIncomeAbove).toFloat();
}

void SellDecision1Config::setEnabled(bool value)
{
    QMutexLocker lock(mMutex);

    mEnabled = value;
}

bool SellDecision1Config::isEnabled()
{
    QMutexLocker lock(mMutex);

    return mEnabled;
}

void SellDecision1Config::setIncomeAbove(float value)
{
    QMutexLocker lock(mMutex);

    mIncomeAbove = value;
}

float SellDecision1Config::getIncomeAbove()
{
    QMutexLocker lock(mMutex);

    return mIncomeAbove;
}
