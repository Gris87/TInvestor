#include "selldecision3config.h"

#include <QDebug>
#include <QMutexLocker>



SellDecision3Config::SellDecision3Config() :
    ISellDecision3Config(),
    mMutex(new QMutex())
{
    qDebug() << "Create SellDecision3Config";
}

SellDecision3Config::~SellDecision3Config()
{
    qDebug() << "Destroy SellDecision3Config";

    delete mMutex;
}

void SellDecision3Config::assign(ISellDecision3Config* another)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Assigning SellDecision3Config to SellDecision3Config";

    const SellDecision3Config& config = *dynamic_cast<SellDecision3Config*>(another);

    mEnabled    = config.mEnabled;
    mLoseIncome = config.mLoseIncome;
    mDuration   = config.mDuration;
}

void SellDecision3Config::makeDefault()
{
    QMutexLocker lock(mMutex);

    qDebug() << "Set SellDecision3Config to default";

    mEnabled    = true;
    mLoseIncome = 5.0f;
    mDuration   = 3;
}

void SellDecision3Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Save SellDecision3Config";

    settingsEditor->setValue(type + "/Enabled",    mEnabled);
    settingsEditor->setValue(type + "/LoseIncome", mLoseIncome);
    settingsEditor->setValue(type + "/Duration",   mDuration);
}

void SellDecision3Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Load SellDecision3Config";

    mEnabled    = settingsEditor->value(type + "/Enabled",    mEnabled).toBool();
    mLoseIncome = settingsEditor->value(type + "/LoseIncome", mLoseIncome).toFloat();
    mDuration   = settingsEditor->value(type + "/Duration",   mDuration).toInt();
}

void SellDecision3Config::setEnabled(bool value)
{
    QMutexLocker lock(mMutex);

    mEnabled = value;
}

bool SellDecision3Config::isEnabled()
{
    QMutexLocker lock(mMutex);

    return mEnabled;
}

void SellDecision3Config::setLoseIncome(float value)
{
    QMutexLocker lock(mMutex);

    mLoseIncome = value;
}

float SellDecision3Config::getLoseIncome()
{
    QMutexLocker lock(mMutex);

    return mLoseIncome;
}

void SellDecision3Config::setDuration(int value)
{
    QMutexLocker lock(mMutex);

    mDuration = value;
}

int SellDecision3Config::getDuration()
{
    QMutexLocker lock(mMutex);

    return mDuration;
}
