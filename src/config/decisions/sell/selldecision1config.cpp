#include "selldecision1config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



SellDecision1Config::SellDecision1Config()
{
    qDebug() << "Create SellDecision1Config";
}

SellDecision1Config::~SellDecision1Config()
{
    qDebug() << "Destroy SellDecision1Config";
}

SellDecision1Config& SellDecision1Config::operator=(const SellDecision1Config &config)
{
    assign(config);

    return *this;
}

void SellDecision1Config::makeDefault()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Set SellDecision1Config to default";

    mEnabled = true;
}

void SellDecision1Config::assign(const SellDecision1Config &config)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Assigning SellDecision1Config to SellDecision1Config";

    mEnabled = config.mEnabled;
}

void SellDecision1Config::save(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Save SellDecision1Config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Enabled", mEnabled);
}

void SellDecision1Config::load(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Load SellDecision1Config";

    QSettings settings("GrisCom", "TInvestor");

    mEnabled = settings.value(type + "/Enabled", mEnabled).toBool();
}

void SellDecision1Config::setEnabled(bool value)
{
    QMutexLocker lock(&mMutex);

    mEnabled = value;
}

bool SellDecision1Config::isEnabled()
{
    QMutexLocker lock(&mMutex);

    return mEnabled;
}
