#include "decisionmakerconfig.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



DecisionMakerConfig::DecisionMakerConfig()
{
    qDebug() << "Create DecisionMakerConfig";

    makeDefault();
}

DecisionMakerConfig::~DecisionMakerConfig()
{
    qDebug() << "Destroy DecisionMakerConfig";
}

DecisionMakerConfig& DecisionMakerConfig::operator=(const DecisionMakerConfig &config)
{
    assign(config);

    return *this;
}

void DecisionMakerConfig::makeDefault()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Set DecisionMakerConfig to default";

    mAutorun = true;
}

void DecisionMakerConfig::assign(const DecisionMakerConfig &config)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Assigning DecisionMakerConfig to DecisionMakerConfig";

    mAutorun = config.mAutorun;
}

void DecisionMakerConfig::save(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Save DecisionMakerConfig";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue(type + "/Autorun", mAutorun);
}

void DecisionMakerConfig::load(const QString &type)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Load DecisionMakerConfig";

    QSettings settings("GrisCom", "TInvestor");

    mAutorun = settings.value(type + "/Autorun", mAutorun).toBool();
}

void DecisionMakerConfig::setAutorun(bool value)
{
    QMutexLocker lock(&mMutex);

    mAutorun = value;
}

bool DecisionMakerConfig::isAutorun()
{
    QMutexLocker lock(&mMutex);

    return mAutorun;
}
