#include "config.h"

#include <QDebug>
#include <QMutexLocker>



Config::Config(IDecisionMakerConfig* simulatorConfig, IDecisionMakerConfig* autoPilotConfig) :
    IConfig(),
    mMutex(new QMutex()),
    mSimulatorConfig(simulatorConfig),
    mAutoPilotConfig(autoPilotConfig)
{
    qDebug() << "Create Config";
}

Config::~Config()
{
    qDebug() << "Destroy Config";

    delete mMutex;
}

void Config::assign(IConfig* another)
{
    QMutexLocker lock(mMutex);

    qDebug() << "Assigning Config to Config";

    const Config& config = *dynamic_cast<Config*>(another);

    mSimulatorConfig->assign(config.mSimulatorConfig);
    mAutoPilotConfig->assign(config.mAutoPilotConfig);

    mAutorun                   = config.mAutorun;
    mMakeDecisionTimeout       = config.mMakeDecisionTimeout;
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

void Config::makeDefault()
{
    QMutexLocker lock(mMutex);

    qInfo() << "Set Config to default";

    mSimulatorConfig->makeDefault();
    mAutoPilotConfig->makeDefault();

    mAutorun                   = true;
    mMakeDecisionTimeout       = 1;
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
    mStorageMonthLimit         = 12;
    mSimulatorConfigCommon     = true;
    mAutoPilotConfigCommon     = false;
}

void Config::save(ISettingsEditor* settingsEditor)
{
    QMutexLocker lock(mMutex);

    qInfo() << "Save Config";

    mSimulatorConfig->save(settingsEditor, "Simulator");
    mAutoPilotConfig->save(settingsEditor, "AutoPilot");

    settingsEditor->setValue("Config/Autorun",                   mAutorun);
    settingsEditor->setValue("Config/MakeDecisionTimeout",       mMakeDecisionTimeout);
    settingsEditor->setValue("Config/UseSchedule",               mUseSchedule);
    settingsEditor->setValue("Config/ScheduleStartHour",         mScheduleStartHour);
    settingsEditor->setValue("Config/ScheduleStartMinute",       mScheduleStartMinute);
    settingsEditor->setValue("Config/ScheduleEndHour",           mScheduleEndHour);
    settingsEditor->setValue("Config/ScheduleEndMinute",         mScheduleEndMinute);
    settingsEditor->setValue("Config/LimitPurchasesPerDay",      mLimitPurchasesPerDay);
    settingsEditor->setValue("Config/AmountOfPurchasesPerDay",   mAmountOfPurchasesPerDay);
    settingsEditor->setValue("Config/LimitPurchasesPerStock",    mLimitPurchasesPerStock);
    settingsEditor->setValue("Config/AmountOfPurchasesPerStock", mAmountOfPurchasesPerStock);
    settingsEditor->setValue("Config/Commission",                mCommission);
    settingsEditor->setValue("Config/LimitStockPurchase",        mLimitStockPurchase);
    settingsEditor->setValue("Config/AmountOfStockPurchase",     mAmountOfStockPurchase);
    settingsEditor->setValue("Config/StorageMonthLimit",         mStorageMonthLimit);
    settingsEditor->setValue("Config/SimulatorConfigCommon",     mSimulatorConfigCommon);
    settingsEditor->setValue("Config/AutoPilotConfigCommon",     mAutoPilotConfigCommon);
}

void Config::load(ISettingsEditor* settingsEditor)
{
    QMutexLocker lock(mMutex);

    qInfo() << "Load Config";

    mSimulatorConfig->load(settingsEditor, "Simulator");
    mAutoPilotConfig->load(settingsEditor, "AutoPilot");

    mAutorun                   = settingsEditor->value("Config/Autorun",                   mAutorun).toBool();
    mMakeDecisionTimeout       = settingsEditor->value("Config/MakeDecisionTimeout",       mMakeDecisionTimeout).toInt();
    mUseSchedule               = settingsEditor->value("Config/UseSchedule",               mUseSchedule).toBool();
    mScheduleStartHour         = settingsEditor->value("Config/ScheduleStartHour",         mScheduleStartHour).toInt();
    mScheduleStartMinute       = settingsEditor->value("Config/ScheduleStartMinute",       mScheduleStartMinute).toInt();
    mScheduleEndHour           = settingsEditor->value("Config/ScheduleEndHour",           mScheduleEndHour).toInt();
    mScheduleEndMinute         = settingsEditor->value("Config/ScheduleEndMinute",         mScheduleEndMinute).toInt();
    mLimitPurchasesPerDay      = settingsEditor->value("Config/LimitPurchasesPerDay",      mLimitPurchasesPerDay).toBool();
    mAmountOfPurchasesPerDay   = settingsEditor->value("Config/AmountOfPurchasesPerDay",   mAmountOfPurchasesPerDay).toInt();
    mLimitPurchasesPerStock    = settingsEditor->value("Config/LimitPurchasesPerStock",    mLimitPurchasesPerStock).toBool();
    mAmountOfPurchasesPerStock = settingsEditor->value("Config/AmountOfPurchasesPerStock", mAmountOfPurchasesPerStock).toInt();
    mCommission                = settingsEditor->value("Config/Commission",                mCommission).toFloat();
    mLimitStockPurchase        = settingsEditor->value("Config/LimitStockPurchase",        mLimitStockPurchase).toBool();
    mAmountOfStockPurchase     = settingsEditor->value("Config/AmountOfStockPurchase",     mAmountOfStockPurchase).toInt();
    mStorageMonthLimit         = settingsEditor->value("Config/StorageMonthLimit",         mStorageMonthLimit).toInt();
    mSimulatorConfigCommon     = settingsEditor->value("Config/SimulatorConfigCommon",     mSimulatorConfigCommon).toBool();
    mAutoPilotConfigCommon     = settingsEditor->value("Config/AutoPilotConfigCommon",     mAutoPilotConfigCommon).toBool();
}

IDecisionMakerConfig* Config::getSimulatorConfig()
{
    return mSimulatorConfig;
}

IDecisionMakerConfig* Config::getAutoPilotConfig()
{
    return mAutoPilotConfig;
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

void Config::setMakeDecisionTimeout(int value)
{
    QMutexLocker lock(mMutex);

    mMakeDecisionTimeout = value;
}

int Config::getMakeDecisionTimeout()
{
    QMutexLocker lock(mMutex);

    return mMakeDecisionTimeout;
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
