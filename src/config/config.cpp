#include "config.h"

#include <QDebug>
#include <QMutexLocker>
#include <QSettings>



Config::Config(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Create Config";

    makeDefault();
}

Config::Config(const Config &config, QObject *parent) :
    QObject(parent)
{
    qDebug() << "Create Config";

    assign(config);
}

Config::~Config()
{
    qDebug() << "Destroy Config";
}

Config& Config::operator=(const Config &config)
{
    assign(config);

    return *this;
}

void Config::makeDefault()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Set Config to default";

    simulatorConfig.makeDefault();
    autoPilotConfig.makeDefault();

    mAutorun                    = true;
    mRefreshTimeout             = 5;
    mUseSchedule                = true;
    mScheduleStartHour          = 10;
    mScheduleStartMinute        = 0;
    mScheduleEndHour            = 18;
    mScheduleEndMinute          = 0;
    mLimitOperationsPerDay      = true;
    mAmountOfOperationsPerDay   = 50;
    mLimitOperationsPerStock    = true;
    mAmountOfOperationsPerStock = 10;
    mSimulatorConfigCommon      = true;
    mAutoPilotConfigCommon      = false;
}

void Config::assign(const Config &config)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Assigning Config to Config";

    simulatorConfig = config.simulatorConfig;
    autoPilotConfig = config.autoPilotConfig;

    mAutorun                    = config.mAutorun;
    mRefreshTimeout             = config.mRefreshTimeout;
    mUseSchedule                = config.mUseSchedule;
    mScheduleStartHour          = config.mScheduleStartHour;
    mScheduleStartMinute        = config.mScheduleStartMinute;
    mScheduleEndHour            = config.mScheduleEndHour;
    mScheduleEndMinute          = config.mScheduleEndMinute;
    mLimitOperationsPerDay      = config.mLimitOperationsPerDay;
    mAmountOfOperationsPerDay   = config.mAmountOfOperationsPerDay;
    mLimitOperationsPerStock    = config.mLimitOperationsPerStock;
    mAmountOfOperationsPerStock = config.mAmountOfOperationsPerStock;
    mSimulatorConfigCommon      = config.mSimulatorConfigCommon;
    mAutoPilotConfigCommon      = config.mAutoPilotConfigCommon;
}

void Config::save()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Save Config";

    simulatorConfig.save("Simulator");
    autoPilotConfig.save("AutoPilot");

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue("Config/Autorun",                    mAutorun);
    settings.setValue("Config/RefreshTimeout",             mRefreshTimeout);
    settings.setValue("Config/UseSchedule",                mUseSchedule);
    settings.setValue("Config/ScheduleStartHour",          mScheduleStartHour);
    settings.setValue("Config/ScheduleStartMinute",        mScheduleStartMinute);
    settings.setValue("Config/ScheduleEndHour",            mScheduleEndHour);
    settings.setValue("Config/ScheduleEndMinute",          mScheduleEndMinute);
    settings.setValue("Config/LimitOperationsPerDay",      mLimitOperationsPerDay);
    settings.setValue("Config/AmountOfOperationsPerDay",   mAmountOfOperationsPerDay);
    settings.setValue("Config/LimitOperationsPerStock",    mLimitOperationsPerStock);
    settings.setValue("Config/AmountOfOperationsPerStock", mAmountOfOperationsPerStock);
    settings.setValue("Config/SimulatorConfigCommon",      mSimulatorConfigCommon);
    settings.setValue("Config/AutoPilotConfigCommon",      mAutoPilotConfigCommon);
}

void Config::load()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Load Config";

    simulatorConfig.load("Simulator");
    autoPilotConfig.load("AutoPilot");

    QSettings settings("GrisCom", "TInvestor");

    mAutorun                    = settings.value("Config/Autorun",                    mAutorun).toBool();
    mRefreshTimeout             = settings.value("Config/RefreshTimeout",             mRefreshTimeout).toInt();
    mUseSchedule                = settings.value("Config/UseSchedule",                mUseSchedule).toBool();
    mScheduleStartHour          = settings.value("Config/ScheduleStartHour",          mScheduleStartHour).toInt();
    mScheduleStartMinute        = settings.value("Config/ScheduleStartMinute",        mScheduleStartMinute).toInt();
    mScheduleEndHour            = settings.value("Config/ScheduleEndHour",            mScheduleEndHour).toInt();
    mScheduleEndMinute          = settings.value("Config/ScheduleEndMinute",          mScheduleEndMinute).toInt();
    mLimitOperationsPerDay      = settings.value("Config/LimitOperationsPerDay",      mLimitOperationsPerDay).toBool();
    mAmountOfOperationsPerDay   = settings.value("Config/AmountOfOperationsPerDay",   mAmountOfOperationsPerDay).toInt();
    mLimitOperationsPerStock    = settings.value("Config/LimitOperationsPerStock",    mLimitOperationsPerStock).toBool();
    mAmountOfOperationsPerStock = settings.value("Config/AmountOfOperationsPerStock", mAmountOfOperationsPerStock).toInt();
    mSimulatorConfigCommon      = settings.value("Config/SimulatorConfigCommon",      mSimulatorConfigCommon).toBool();
    mAutoPilotConfigCommon      = settings.value("Config/AutoPilotConfigCommon",      mAutoPilotConfigCommon).toBool();
}

void Config::setAutorun(bool value)
{
    QMutexLocker lock(&mMutex);

    mAutorun = value;
}

bool Config::isAutorun()
{
    QMutexLocker lock(&mMutex);

    return mAutorun;
}

void Config::setRefreshTimeout(int value)
{
    QMutexLocker lock(&mMutex);

    mRefreshTimeout = value;
}

int Config::getRefreshTimeout()
{
    QMutexLocker lock(&mMutex);

    return mRefreshTimeout;
}

void Config::setUseSchedule(bool value)
{
    QMutexLocker lock(&mMutex);

    mUseSchedule = value;
}

bool Config::isUseSchedule()
{
    QMutexLocker lock(&mMutex);

    return mUseSchedule;
}

void Config::setScheduleStartHour(int value)
{
    QMutexLocker lock(&mMutex);

    mScheduleStartHour = value;
}

int Config::getScheduleStartHour()
{
    QMutexLocker lock(&mMutex);

    return mScheduleStartHour;
}

void Config::setScheduleStartMinute(int value)
{
    QMutexLocker lock(&mMutex);

    mScheduleStartMinute = value;
}

int Config::getScheduleStartMinute()
{
    QMutexLocker lock(&mMutex);

    return mScheduleStartMinute;
}

void Config::setScheduleEndHour(int value)
{
    QMutexLocker lock(&mMutex);

    mScheduleEndHour = value;
}

int Config::getScheduleEndHour()
{
    QMutexLocker lock(&mMutex);

    return mScheduleEndHour;
}

void Config::setScheduleEndMinute(int value)
{
    QMutexLocker lock(&mMutex);

    mScheduleEndMinute = value;
}

int Config::getScheduleEndMinute()
{
    QMutexLocker lock(&mMutex);

    return mScheduleEndMinute;
}

void Config::setLimitOperationsPerDay(bool value)
{
    QMutexLocker lock(&mMutex);

    mLimitOperationsPerDay = value;
}

bool Config::isLimitOperationsPerDay()
{
    QMutexLocker lock(&mMutex);

    return mLimitOperationsPerDay;
}

void Config::setAmountOfOperationsPerDay(int value)
{
    QMutexLocker lock(&mMutex);

    mAmountOfOperationsPerDay = value;
}

int Config::getAmountOfOperationsPerDay()
{
    QMutexLocker lock(&mMutex);

    return mAmountOfOperationsPerDay;
}

void Config::setLimitOperationsPerStock(bool value)
{
    QMutexLocker lock(&mMutex);

    mLimitOperationsPerStock = value;
}

bool Config::isLimitOperationsPerStock()
{
    QMutexLocker lock(&mMutex);

    return mLimitOperationsPerStock;
}

void Config::setAmountOfOperationsPerStock(int value)
{
    QMutexLocker lock(&mMutex);

    mAmountOfOperationsPerStock = value;
}

int Config::getAmountOfOperationsPerStock()
{
    QMutexLocker lock(&mMutex);

    return mAmountOfOperationsPerStock;
}

void Config::setSimulatorConfigCommon(bool value)
{
    QMutexLocker lock(&mMutex);

    mSimulatorConfigCommon = value;
}

bool Config::isSimulatorConfigCommon()
{
    QMutexLocker lock(&mMutex);

    return mSimulatorConfigCommon;
}

void Config::setAutoPilotConfigCommon(bool value)
{
    QMutexLocker lock(&mMutex);

    mAutoPilotConfigCommon = value;
}

bool Config::isAutoPilotConfigCommon()
{
    QMutexLocker lock(&mMutex);

    return mAutoPilotConfigCommon;
}
