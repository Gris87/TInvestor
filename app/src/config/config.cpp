#include "config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



Config::Config(QObject *parent) :
    QObject(parent),
    simulatorConfig(),
    autoPilotConfig(),
    mMutex(new QMutex())
{
    qDebug() << "Create Config";

    makeDefault();
}

Config::Config(const Config &config, QObject *parent) :
    QObject(parent),
    simulatorConfig(),
    autoPilotConfig(),
    mMutex(new QMutex())
{
    qDebug() << "Create Config";

    assign(config);
}

Config::~Config()
{
    qDebug() << "Destroy Config";

    delete mMutex;
}

Config& Config::operator=(const Config &config)
{
    assign(config);

    return *this;
}

void Config::makeDefault()
{
    QMutexLocker lock(mMutex);

    qInfo() << "Set Config to default";

    simulatorConfig.makeDefault();
    autoPilotConfig.makeDefault();

    mAutorun                   = true;
    mRefreshTimeout            = 1;
    mUseSchedule               = true;
    mScheduleStartHour         = 10;
    mScheduleStartMinute       = 0;
    mScheduleEndHour           = 18;
    mScheduleEndMinute         = 40;
    mLimitPurchasesPerDay      = true;
    mAmountOfPurchasesPerDay   = 50;
    mLimitPurchasesPerStock    = true;
    mAmountOfPurchasesPerStock = 10;
    mCommission                = 0.3f;
    mLimitStockPurchase        = true;
    mAmountOfStockPurchase     = 10000;
    mStorageMonthLimit         = 24;
    mSimulatorConfigCommon     = true;
    mAutoPilotConfigCommon     = false;
}

void Config::assign(const Config &config)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Assigning Config to Config";

    simulatorConfig = config.simulatorConfig;
    autoPilotConfig = config.autoPilotConfig;

    mAutorun                   = config.mAutorun;
    mRefreshTimeout            = config.mRefreshTimeout;
    mUseSchedule               = config.mUseSchedule;
    mScheduleStartHour         = config.mScheduleStartHour;
    mScheduleStartMinute       = config.mScheduleStartMinute;
    mScheduleEndHour           = config.mScheduleEndHour;
    mScheduleEndMinute         = config.mScheduleEndMinute;
    mLimitPurchasesPerDay      = config.mLimitPurchasesPerDay;
    mAmountOfPurchasesPerDay   = config.mAmountOfPurchasesPerDay;
    mLimitPurchasesPerStock    = config.mLimitPurchasesPerStock;
    mAmountOfPurchasesPerStock = config.mAmountOfPurchasesPerStock;
    mCommission                = config.mCommission;
    mLimitStockPurchase        = config.mLimitStockPurchase;
    mAmountOfStockPurchase     = config.mAmountOfStockPurchase;
    mStorageMonthLimit         = config.mStorageMonthLimit;
    mSimulatorConfigCommon     = config.mSimulatorConfigCommon;
    mAutoPilotConfigCommon     = config.mAutoPilotConfigCommon;
}

void Config::save()
{
    QMutexLocker lock(mMutex);

    qInfo() << "Save Config";

    simulatorConfig.save("Simulator");
    autoPilotConfig.save("AutoPilot");

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue("Config/Autorun",                   mAutorun);
    settings.setValue("Config/RefreshTimeout",            mRefreshTimeout);
    settings.setValue("Config/UseSchedule",               mUseSchedule);
    settings.setValue("Config/ScheduleStartHour",         mScheduleStartHour);
    settings.setValue("Config/ScheduleStartMinute",       mScheduleStartMinute);
    settings.setValue("Config/ScheduleEndHour",           mScheduleEndHour);
    settings.setValue("Config/ScheduleEndMinute",         mScheduleEndMinute);
    settings.setValue("Config/LimitPurchasesPerDay",      mLimitPurchasesPerDay);
    settings.setValue("Config/AmountOfPurchasesPerDay",   mAmountOfPurchasesPerDay);
    settings.setValue("Config/LimitPurchasesPerStock",    mLimitPurchasesPerStock);
    settings.setValue("Config/AmountOfPurchasesPerStock", mAmountOfPurchasesPerStock);
    settings.setValue("Config/Commission",                mCommission);
    settings.setValue("Config/LimitStockPurchase",        mLimitStockPurchase);
    settings.setValue("Config/AmountOfStockPurchase",     mAmountOfStockPurchase);
    settings.setValue("Config/StorageMonthLimit",         mStorageMonthLimit);
    settings.setValue("Config/SimulatorConfigCommon",     mSimulatorConfigCommon);
    settings.setValue("Config/AutoPilotConfigCommon",     mAutoPilotConfigCommon);
}

void Config::load()
{
    QMutexLocker lock(mMutex);

    qInfo() << "Load Config";

    simulatorConfig.load("Simulator");
    autoPilotConfig.load("AutoPilot");

    QSettings settings("GrisCom", "TInvestor");

    mAutorun                   = settings.value("Config/Autorun",                   mAutorun).toBool();
    mRefreshTimeout            = settings.value("Config/RefreshTimeout",            mRefreshTimeout).toInt();
    mUseSchedule               = settings.value("Config/UseSchedule",               mUseSchedule).toBool();
    mScheduleStartHour         = settings.value("Config/ScheduleStartHour",         mScheduleStartHour).toInt();
    mScheduleStartMinute       = settings.value("Config/ScheduleStartMinute",       mScheduleStartMinute).toInt();
    mScheduleEndHour           = settings.value("Config/ScheduleEndHour",           mScheduleEndHour).toInt();
    mScheduleEndMinute         = settings.value("Config/ScheduleEndMinute",         mScheduleEndMinute).toInt();
    mLimitPurchasesPerDay      = settings.value("Config/LimitPurchasesPerDay",      mLimitPurchasesPerDay).toBool();
    mAmountOfPurchasesPerDay   = settings.value("Config/AmountOfPurchasesPerDay",   mAmountOfPurchasesPerDay).toInt();
    mLimitPurchasesPerStock    = settings.value("Config/LimitPurchasesPerStock",    mLimitPurchasesPerStock).toBool();
    mAmountOfPurchasesPerStock = settings.value("Config/AmountOfPurchasesPerStock", mAmountOfPurchasesPerStock).toInt();
    mCommission                = settings.value("Config/Commission",                mCommission).toFloat();
    mLimitStockPurchase        = settings.value("Config/LimitStockPurchase",        mLimitStockPurchase).toBool();
    mAmountOfStockPurchase     = settings.value("Config/AmountOfStockPurchase",     mAmountOfStockPurchase).toInt();
    mStorageMonthLimit         = settings.value("Config/StorageMonthLimit",         mStorageMonthLimit).toInt();
    mSimulatorConfigCommon     = settings.value("Config/SimulatorConfigCommon",     mSimulatorConfigCommon).toBool();
    mAutoPilotConfigCommon     = settings.value("Config/AutoPilotConfigCommon",     mAutoPilotConfigCommon).toBool();
}

void Config::setAutorun(bool value)
{
    QMutexLocker lock(mMutex);

    mAutorun = value;
}

bool Config::isAutorun()
{
    QMutexLocker lock(mMutex);

    return mAutorun;
}

void Config::setRefreshTimeout(int value)
{
    QMutexLocker lock(mMutex);

    mRefreshTimeout = value;
}

int Config::getRefreshTimeout()
{
    QMutexLocker lock(mMutex);

    return mRefreshTimeout;
}

void Config::setUseSchedule(bool value)
{
    QMutexLocker lock(mMutex);

    mUseSchedule = value;
}

bool Config::isUseSchedule()
{
    QMutexLocker lock(mMutex);

    return mUseSchedule;
}

void Config::setScheduleStartHour(int value)
{
    QMutexLocker lock(mMutex);

    mScheduleStartHour = value;
}

int Config::getScheduleStartHour()
{
    QMutexLocker lock(mMutex);

    return mScheduleStartHour;
}

void Config::setScheduleStartMinute(int value)
{
    QMutexLocker lock(mMutex);

    mScheduleStartMinute = value;
}

int Config::getScheduleStartMinute()
{
    QMutexLocker lock(mMutex);

    return mScheduleStartMinute;
}

void Config::setScheduleEndHour(int value)
{
    QMutexLocker lock(mMutex);

    mScheduleEndHour = value;
}

int Config::getScheduleEndHour()
{
    QMutexLocker lock(mMutex);

    return mScheduleEndHour;
}

void Config::setScheduleEndMinute(int value)
{
    QMutexLocker lock(mMutex);

    mScheduleEndMinute = value;
}

int Config::getScheduleEndMinute()
{
    QMutexLocker lock(mMutex);

    return mScheduleEndMinute;
}

void Config::setLimitPurchasesPerDay(bool value)
{
    QMutexLocker lock(mMutex);

    mLimitPurchasesPerDay = value;
}

bool Config::isLimitPurchasesPerDay()
{
    QMutexLocker lock(mMutex);

    return mLimitPurchasesPerDay;
}

void Config::setAmountOfPurchasesPerDay(int value)
{
    QMutexLocker lock(mMutex);

    mAmountOfPurchasesPerDay = value;
}

int Config::getAmountOfPurchasesPerDay()
{
    QMutexLocker lock(mMutex);

    return mAmountOfPurchasesPerDay;
}

void Config::setLimitPurchasesPerStock(bool value)
{
    QMutexLocker lock(mMutex);

    mLimitPurchasesPerStock = value;
}

bool Config::isLimitPurchasesPerStock()
{
    QMutexLocker lock(mMutex);

    return mLimitPurchasesPerStock;
}

void Config::setAmountOfPurchasesPerStock(int value)
{
    QMutexLocker lock(mMutex);

    mAmountOfPurchasesPerStock = value;
}

int Config::getAmountOfPurchasesPerStock()
{
    QMutexLocker lock(mMutex);

    return mAmountOfPurchasesPerStock;
}

void Config::setCommission(float value)
{
    QMutexLocker lock(mMutex);

    mCommission = value;
}

float Config::getCommission()
{
    QMutexLocker lock(mMutex);

    return mCommission;
}

void Config::setLimitStockPurchase(bool value)
{
    QMutexLocker lock(mMutex);

    mLimitStockPurchase = value;
}

bool Config::isLimitStockPurchase()
{
    QMutexLocker lock(mMutex);

    return mLimitStockPurchase;
}

void Config::setAmountOfStockPurchase(int value)
{
    QMutexLocker lock(mMutex);

    mAmountOfStockPurchase = value;
}

int Config::getAmountOfStockPurchase()
{
    QMutexLocker lock(mMutex);

    return mAmountOfStockPurchase;
}

void Config::setStorageMonthLimit(int value)
{
    QMutexLocker lock(mMutex);

    mStorageMonthLimit = value;
}

int Config::getStorageMonthLimit()
{
    QMutexLocker lock(mMutex);

    return mStorageMonthLimit;
}

void Config::setSimulatorConfigCommon(bool value)
{
    QMutexLocker lock(mMutex);

    mSimulatorConfigCommon = value;
}

bool Config::isSimulatorConfigCommon()
{
    QMutexLocker lock(mMutex);

    return mSimulatorConfigCommon;
}

void Config::setAutoPilotConfigCommon(bool value)
{
    QMutexLocker lock(mMutex);

    mAutoPilotConfigCommon = value;
}

bool Config::isAutoPilotConfigCommon()
{
    QMutexLocker lock(mMutex);

    return mAutoPilotConfigCommon;
}
