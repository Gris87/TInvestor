#include "src/config/decisions/buy/buydecision1config/buydecision1config.h"

#include <QDebug>
#include <QMutexLocker>



BuyDecision1Config::BuyDecision1Config() :
    IBuyDecision1Config(),
    mMutex(new QMutex()),
    mEnabled(),
    mPriceFall(),
    mDuration()
{
    qDebug() << "Create BuyDecision1Config";
}

BuyDecision1Config::~BuyDecision1Config()
{
    qDebug() << "Destroy BuyDecision1Config";

    delete mMutex;
}

void BuyDecision1Config::assign(IBuyDecision1Config* another)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Assigning BuyDecision1Config to BuyDecision1Config";

    const BuyDecision1Config& config = *dynamic_cast<BuyDecision1Config*>(another);

    mEnabled   = config.mEnabled;
    mPriceFall = config.mPriceFall;
    mDuration  = config.mDuration;
}

void BuyDecision1Config::makeDefault()
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Set BuyDecision1Config to default";

    mEnabled   = true;
    mPriceFall = 1.0f;
    mDuration  = 5;
}

void BuyDecision1Config::save(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Save BuyDecision1Config";

    // clang-format off
    settingsEditor->setValue(type + "/Enabled",   mEnabled);
    settingsEditor->setValue(type + "/PriceFall", mPriceFall);
    settingsEditor->setValue(type + "/Duration",  mDuration);
    // clang-format on
}

void BuyDecision1Config::load(ISettingsEditor* settingsEditor, const QString& type)
{
    const QMutexLocker lock(mMutex);

    qDebug() << "Load BuyDecision1Config";

    // clang-format off
    mEnabled   = settingsEditor->value(type + "/Enabled",   mEnabled).toBool();
    mPriceFall = settingsEditor->value(type + "/PriceFall", mPriceFall).toFloat();
    mDuration  = settingsEditor->value(type + "/Duration",  mDuration).toInt();
    // clang-format on
}

void BuyDecision1Config::setEnabled(bool value)
{
    const QMutexLocker lock(mMutex);

    mEnabled = value;
}

bool BuyDecision1Config::isEnabled()
{
    const QMutexLocker lock(mMutex);

    return mEnabled;
}

void BuyDecision1Config::setPriceFall(float value)
{
    const QMutexLocker lock(mMutex);

    mPriceFall = value;
}

float BuyDecision1Config::getPriceFall()
{
    const QMutexLocker lock(mMutex);

    return mPriceFall;
}

void BuyDecision1Config::setDuration(int value)
{
    const QMutexLocker lock(mMutex);

    mDuration = value;
}

int BuyDecision1Config::getDuration()
{
    const QMutexLocker lock(mMutex);

    return mDuration;
}
