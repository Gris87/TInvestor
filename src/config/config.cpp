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

    qDebug() << "Set config to default";

    mRefreshTimeout              = 5;
    mUseSchedule                 = true;
    mScheduleStartHour           = 10;
    mScheduleStartMinute         = 0;
    mScheduleEndHour             = 18;
    mScheduleEndMinute           = 0;
    mUseForSimulatorAndAutoPilot = true;
}

void Config::assign(const Config &config)
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Assigning config to config";

    mRefreshTimeout              = config.mRefreshTimeout;
    mUseSchedule                 = config.mUseSchedule;
    mScheduleStartHour           = config.mScheduleStartHour;
    mScheduleStartMinute         = config.mScheduleStartMinute;
    mScheduleEndHour             = config.mScheduleEndHour;
    mScheduleEndMinute           = config.mScheduleEndMinute;
    mUseForSimulatorAndAutoPilot = config.mUseForSimulatorAndAutoPilot;
}

void Config::save()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Save config";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue("Config/RefreshTimeout",              mRefreshTimeout);
    settings.setValue("Config/UseSchedule",                 mUseSchedule);
    settings.setValue("Config/ScheduleStartHour",           mScheduleStartHour);
    settings.setValue("Config/ScheduleStartMinute",         mScheduleStartMinute);
    settings.setValue("Config/ScheduleEndHour",             mScheduleEndHour);
    settings.setValue("Config/ScheduleEndMinute",           mScheduleEndMinute);
    settings.setValue("Config/UseForSimulatorAndAutoPilot", mUseForSimulatorAndAutoPilot);
}

void Config::load()
{
    QMutexLocker lock(&mMutex);

    qDebug() << "Load config";

    QSettings settings("GrisCom", "TInvestor");

    mRefreshTimeout              = settings.value("Config/RefreshTimeout",              mRefreshTimeout).toInt();
    mUseSchedule                 = settings.value("Config/UseSchedule",                 mUseSchedule).toBool();
    mScheduleStartHour           = settings.value("Config/ScheduleStartHour",           mScheduleStartHour).toInt();
    mScheduleStartMinute         = settings.value("Config/ScheduleStartMinute",         mScheduleStartMinute).toInt();
    mScheduleEndHour             = settings.value("Config/ScheduleEndHour",             mScheduleEndHour).toInt();
    mScheduleEndMinute           = settings.value("Config/ScheduleEndMinute",           mScheduleEndMinute).toInt();
    mUseForSimulatorAndAutoPilot = settings.value("Config/UseForSimulatorAndAutoPilot", mUseForSimulatorAndAutoPilot).toBool();
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

void Config::setUseForSimulatorAndAutoPilot(bool value)
{
    QMutexLocker lock(&mMutex);

    mUseForSimulatorAndAutoPilot = value;
}

bool Config::isUseForSimulatorAndAutoPilot()
{
    QMutexLocker lock(&mMutex);

    return mUseForSimulatorAndAutoPilot;
}
