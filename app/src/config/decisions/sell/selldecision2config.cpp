#include "selldecision2config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



SellDecision2Config::SellDecision2Config() :
    ISellDecision2Config(),
    mMutex(new QMutex())
{
    qDebug() << "Create SellDecision2Config";
}

SellDecision2Config::~SellDecision2Config()
{
    qDebug() << "Destroy SellDecision2Config";

    delete mMutex;
}

void SellDecision2Config::assign(ISellDecision2Config *another)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Assigning SellDecision2Config to SellDecision2Config";

    const SellDecision2Config &config = *dynamic_cast<SellDecision2Config *>(another);

    mEnabled     = config.mEnabled;
    mIncomeAbove = config.mIncomeAbove;
    mLoseIncome  = config.mLoseIncome;
}

void SellDecision2Config::makeDefault()
{
    QMutexLocker lock(mMutex);

    qDebug() << "Set SellDecision2Config to default";

    mEnabled     = true;
    mIncomeAbove = 1.0f;
    mLoseIncome  = 0.1f;
}

void SellDecision2Config::save(const QString &type)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Save SellDecision2Config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Enabled",     mEnabled);
    settings.setValue(type + "/IncomeAbove", mIncomeAbove);
    settings.setValue(type + "/LoseIncome",  mLoseIncome);
}

void SellDecision2Config::load(const QString &type)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Load SellDecision2Config";

    QSettings settings("GrisCom", "TInvestor");

    mEnabled     = settings.value(type + "/Enabled",     mEnabled).toBool();
    mIncomeAbove = settings.value(type + "/IncomeAbove", mIncomeAbove).toFloat();
    mLoseIncome  = settings.value(type + "/LoseIncome",  mLoseIncome).toFloat();
}

void SellDecision2Config::setEnabled(bool value)
{
    QMutexLocker lock(mMutex);

    mEnabled = value;
}

bool SellDecision2Config::isEnabled()
{
    QMutexLocker lock(mMutex);

    return mEnabled;
}

void SellDecision2Config::setIncomeAbove(float value)
{
    QMutexLocker lock(mMutex);

    mIncomeAbove = value;
}

float SellDecision2Config::getIncomeAbove()
{
    QMutexLocker lock(mMutex);

    return mIncomeAbove;
}

void SellDecision2Config::setLoseIncome(float value)
{
    QMutexLocker lock(mMutex);

    mLoseIncome = value;
}

float SellDecision2Config::getLoseIncome()
{
    QMutexLocker lock(mMutex);

    return mLoseIncome;
}
