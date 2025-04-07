#include "src/config/config.h"

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
    const QMutexLocker lock(mMutex);

    qDebug() << "Assigning Config to Config";

    const Config& config = *dynamic_cast<Config*>(another);

    mSimulatorConfig->assign(config.mSimulatorConfig);
    mAutoPilotConfig->assign(config.mAutoPilotConfig);

    mAutorun               = config.mAutorun;
    mMakeDecisionTimeout   = config.mMakeDecisionTimeout;
    mUseSchedule           = config.mUseSchedule;
    mScheduleStartHour     = config.mScheduleStartHour;
    mScheduleStartMinute   = config.mScheduleStartMinute;
    mScheduleEndHour       = config.mScheduleEndHour;
    mScheduleEndMinute     = config.mScheduleEndMinute;
    mLimitStockPurchase    = config.mLimitStockPurchase;
    mAmountOfStockPurchase = config.mAmountOfStockPurchase;
    mStorageMonthLimit     = config.mStorageMonthLimit;
    mSimulatorConfigCommon = config.mSimulatorConfigCommon;
    mAutoPilotConfigCommon = config.mAutoPilotConfigCommon;
}

void Config::makeDefault()
{
    const QMutexLocker lock(mMutex);

    qInfo() << "Set Config to default";

    mSimulatorConfig->makeDefault();
    mAutoPilotConfig->makeDefault();

    mAutorun               = true;
    mMakeDecisionTimeout   = 1;
    mUseSchedule           = true;
    mScheduleStartHour     = 10;
    mScheduleStartMinute   = 0;
    mScheduleEndHour       = 18;
    mScheduleEndMinute     = 40;
    mLimitStockPurchase    = true;
    mAmountOfStockPurchase = 10000;
    mStorageMonthLimit     = 12;
    mSimulatorConfigCommon = true;
    mAutoPilotConfigCommon = false;
}

void Config::save(ISettingsEditor* settingsEditor)
{
    const QMutexLocker lock(mMutex);

    qInfo() << "Save Config";

    mSimulatorConfig->save(settingsEditor, "Config/Simulator");
    mAutoPilotConfig->save(settingsEditor, "Config/AutoPilot");

    // clang-format off
    settingsEditor->setValue("Config/Autorun",               mAutorun);
    settingsEditor->setValue("Config/MakeDecisionTimeout",   mMakeDecisionTimeout);
    settingsEditor->setValue("Config/UseSchedule",           mUseSchedule);
    settingsEditor->setValue("Config/ScheduleStartHour",     mScheduleStartHour);
    settingsEditor->setValue("Config/ScheduleStartMinute",   mScheduleStartMinute);
    settingsEditor->setValue("Config/ScheduleEndHour",       mScheduleEndHour);
    settingsEditor->setValue("Config/ScheduleEndMinute",     mScheduleEndMinute);
    settingsEditor->setValue("Config/LimitStockPurchase",    mLimitStockPurchase);
    settingsEditor->setValue("Config/AmountOfStockPurchase", mAmountOfStockPurchase);
    settingsEditor->setValue("Config/StorageMonthLimit",     mStorageMonthLimit);
    settingsEditor->setValue("Config/SimulatorConfigCommon", mSimulatorConfigCommon);
    settingsEditor->setValue("Config/AutoPilotConfigCommon", mAutoPilotConfigCommon);
    // clang-format on
}

void Config::load(ISettingsEditor* settingsEditor)
{
    const QMutexLocker lock(mMutex);

    qInfo() << "Load Config";

    mSimulatorConfig->load(settingsEditor, "Config/Simulator");
    mAutoPilotConfig->load(settingsEditor, "Config/AutoPilot");

    // clang-format off
    mAutorun                   = settingsEditor->value("Config/Autorun",               mAutorun).toBool();
    mMakeDecisionTimeout       = settingsEditor->value("Config/MakeDecisionTimeout",   mMakeDecisionTimeout).toInt();
    mUseSchedule               = settingsEditor->value("Config/UseSchedule",           mUseSchedule).toBool();
    mScheduleStartHour         = settingsEditor->value("Config/ScheduleStartHour",     mScheduleStartHour).toInt();
    mScheduleStartMinute       = settingsEditor->value("Config/ScheduleStartMinute",   mScheduleStartMinute).toInt();
    mScheduleEndHour           = settingsEditor->value("Config/ScheduleEndHour",       mScheduleEndHour).toInt();
    mScheduleEndMinute         = settingsEditor->value("Config/ScheduleEndMinute",     mScheduleEndMinute).toInt();
    mLimitStockPurchase        = settingsEditor->value("Config/LimitStockPurchase",    mLimitStockPurchase).toBool();
    mAmountOfStockPurchase     = settingsEditor->value("Config/AmountOfStockPurchase", mAmountOfStockPurchase).toInt();
    mStorageMonthLimit         = settingsEditor->value("Config/StorageMonthLimit",     mStorageMonthLimit).toInt();
    mSimulatorConfigCommon     = settingsEditor->value("Config/SimulatorConfigCommon", mSimulatorConfigCommon).toBool();
    mAutoPilotConfigCommon     = settingsEditor->value("Config/AutoPilotConfigCommon", mAutoPilotConfigCommon).toBool();
    // clang-format on
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
    const QMutexLocker lock(mMutex);

    mAutorun = value;
}

bool Config::isAutorun()
{
    const QMutexLocker lock(mMutex);

    return mAutorun;
}

void Config::setMakeDecisionTimeout(int value)
{
    const QMutexLocker lock(mMutex);

    mMakeDecisionTimeout = value;
}

int Config::getMakeDecisionTimeout()
{
    const QMutexLocker lock(mMutex);

    return mMakeDecisionTimeout;
}

void Config::setUseSchedule(bool value)
{
    const QMutexLocker lock(mMutex);

    mUseSchedule = value;
}

bool Config::isUseSchedule()
{
    const QMutexLocker lock(mMutex);

    return mUseSchedule;
}

void Config::setScheduleStartHour(int value)
{
    const QMutexLocker lock(mMutex);

    mScheduleStartHour = value;
}

int Config::getScheduleStartHour()
{
    const QMutexLocker lock(mMutex);

    return mScheduleStartHour;
}

void Config::setScheduleStartMinute(int value)
{
    const QMutexLocker lock(mMutex);

    mScheduleStartMinute = value;
}

int Config::getScheduleStartMinute()
{
    const QMutexLocker lock(mMutex);

    return mScheduleStartMinute;
}

void Config::setScheduleEndHour(int value)
{
    const QMutexLocker lock(mMutex);

    mScheduleEndHour = value;
}

int Config::getScheduleEndHour()
{
    const QMutexLocker lock(mMutex);

    return mScheduleEndHour;
}

void Config::setScheduleEndMinute(int value)
{
    const QMutexLocker lock(mMutex);

    mScheduleEndMinute = value;
}

int Config::getScheduleEndMinute()
{
    const QMutexLocker lock(mMutex);

    return mScheduleEndMinute;
}

void Config::setLimitStockPurchase(bool value)
{
    const QMutexLocker lock(mMutex);

    mLimitStockPurchase = value;
}

bool Config::isLimitStockPurchase()
{
    const QMutexLocker lock(mMutex);

    return mLimitStockPurchase;
}

void Config::setAmountOfStockPurchase(int value)
{
    const QMutexLocker lock(mMutex);

    mAmountOfStockPurchase = value;
}

int Config::getAmountOfStockPurchase()
{
    const QMutexLocker lock(mMutex);

    return mAmountOfStockPurchase;
}

void Config::setStorageMonthLimit(int value)
{
    const QMutexLocker lock(mMutex);

    mStorageMonthLimit = value;
}

int Config::getStorageMonthLimit()
{
    const QMutexLocker lock(mMutex);

    return mStorageMonthLimit;
}

void Config::setSimulatorConfigCommon(bool value)
{
    const QMutexLocker lock(mMutex);

    mSimulatorConfigCommon = value;
}

bool Config::isSimulatorConfigCommon()
{
    const QMutexLocker lock(mMutex);

    return mSimulatorConfigCommon;
}

void Config::setAutoPilotConfigCommon(bool value)
{
    const QMutexLocker lock(mMutex);

    mAutoPilotConfigCommon = value;
}

bool Config::isAutoPilotConfigCommon()
{
    const QMutexLocker lock(mMutex);

    return mAutoPilotConfigCommon;
}
