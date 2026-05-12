#include "src/config/config.h"

#include <QDebug>



const char* const SERVER_ADDRESS_DEFAULT = "localhost";

constexpr bool AUTORUN_DEFAULT               = true;
constexpr int  SERVER_PORT_DEFAULT           = 8041;
constexpr bool NOTIFICATIONS_ENABLED_DEFAULT = true;
constexpr bool FILTER_SYSTEM_DEFAULT         = true;
constexpr bool FILTER_PORTFOLIO_DEFAULT      = true;
constexpr bool FILTER_HUGE_SELL_DEFAULT      = true;
constexpr bool FILTER_DIVIDENDS_DEFAULT      = true;
constexpr bool FILTER_PULSE_NEUTRAL_DEFAULT  = true;
constexpr bool FILTER_PULSE_BUY_DEFAULT      = true;
constexpr bool FILTER_PULSE_SELL_DEFAULT     = true;



Config::Config() :
    IConfig(),
    mRwMutex(new QReadWriteLock()),
    mAutorun(),
    mServerAddress(),
    mServerPort(),
    mNotificationsEnabled(),
    mFilterSystem(),
    mFilterPortfolio(),
    mFilterHugeSell(),
    mFilterDividends(),
    mFilterPulseNeutral(),
    mFilterPulseBuy(),
    mFilterPulseSell()
{
    qDebug() << "Create Config";
}

Config::~Config()
{
    qDebug() << "Destroy Config";

    delete mRwMutex;
}

void Config::assign(IConfig* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning Config to Config";

    const Config&     config = *dynamic_cast<Config*>(another);
    const QReadLocker lock2(config.mRwMutex);

    mAutorun              = config.mAutorun;
    mServerAddress        = config.mServerAddress;
    mServerPort           = config.mServerPort;
    mNotificationsEnabled = config.mNotificationsEnabled;
    mFilterSystem         = config.mFilterSystem;
    mFilterPortfolio      = config.mFilterPortfolio;
    mFilterHugeSell       = config.mFilterHugeSell;
    mFilterDividends      = config.mFilterDividends;
    mFilterPulseNeutral   = config.mFilterPulseNeutral;
    mFilterPulseBuy       = config.mFilterPulseBuy;
    mFilterPulseSell      = config.mFilterPulseSell;
}

void Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qInfo() << "Set Config to default";

    mAutorun              = AUTORUN_DEFAULT;
    mServerAddress        = SERVER_ADDRESS_DEFAULT;
    mServerPort           = SERVER_PORT_DEFAULT;
    mNotificationsEnabled = NOTIFICATIONS_ENABLED_DEFAULT;
    mFilterSystem         = FILTER_SYSTEM_DEFAULT;
    mFilterPortfolio      = FILTER_PORTFOLIO_DEFAULT;
    mFilterHugeSell       = FILTER_HUGE_SELL_DEFAULT;
    mFilterDividends      = FILTER_DIVIDENDS_DEFAULT;
    mFilterPulseNeutral   = FILTER_PULSE_NEUTRAL_DEFAULT;
    mFilterPulseBuy       = FILTER_PULSE_BUY_DEFAULT;
    mFilterPulseSell      = FILTER_PULSE_SELL_DEFAULT;
}

void Config::save(ISettingsEditor* settingsEditor)
{
    const QReadLocker lock(mRwMutex);

    qInfo() << "Save Config";

    // clang-format off
    settingsEditor->setValue("Config/Autorun",              mAutorun);
    settingsEditor->setValue("Config/ServerAddress",        mServerAddress);
    settingsEditor->setValue("Config/ServerPort",           mServerPort);
    settingsEditor->setValue("Config/NotificationsEnabled", mNotificationsEnabled);
    settingsEditor->setValue("Config/FilterSystem",         mFilterSystem);
    settingsEditor->setValue("Config/FilterPortfolio",      mFilterPortfolio);
    settingsEditor->setValue("Config/FilterHugeSell",       mFilterHugeSell);
    settingsEditor->setValue("Config/FilterDividends",      mFilterDividends);
    settingsEditor->setValue("Config/FilterPulseNeutral",   mFilterPulseNeutral);
    settingsEditor->setValue("Config/FilterPulseBuy",       mFilterPulseBuy);
    settingsEditor->setValue("Config/FilterPulseSell",      mFilterPulseSell);
    // clang-format on
}

void Config::load(ISettingsEditor* settingsEditor)
{
    const QWriteLocker lock(mRwMutex);

    qInfo() << "Load Config";

    // clang-format off
    mAutorun              = settingsEditor->value("Config/Autorun",              mAutorun).toBool();
    mServerAddress        = settingsEditor->value("Config/ServerAddress",        mServerAddress).toString();
    mServerPort           = settingsEditor->value("Config/ServerPort",           mServerPort).toInt();
    mNotificationsEnabled = settingsEditor->value("Config/NotificationsEnabled", mNotificationsEnabled).toBool();
    mFilterSystem         = settingsEditor->value("Config/FilterSystem",         mFilterSystem).toBool();
    mFilterPortfolio      = settingsEditor->value("Config/FilterPortfolio",      mFilterPortfolio).toBool();
    mFilterHugeSell       = settingsEditor->value("Config/FilterHugeSell",       mFilterHugeSell).toBool();
    mFilterDividends      = settingsEditor->value("Config/FilterDividends",      mFilterDividends).toBool();
    mFilterPulseNeutral   = settingsEditor->value("Config/FilterPulseNeutral",   mFilterPulseNeutral).toBool();
    mFilterPulseBuy       = settingsEditor->value("Config/FilterPulseBuy",       mFilterPulseBuy).toBool();
    mFilterPulseSell      = settingsEditor->value("Config/FilterPulseSell",      mFilterPulseSell).toBool();
    // clang-format on
}

void Config::setAutorun(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mAutorun = value;
}

bool Config::isAutorun()
{
    const QReadLocker lock(mRwMutex);

    return mAutorun;
}

void Config::setServerAddress(const QString& value)
{
    const QWriteLocker lock(mRwMutex);

    mServerAddress = value;
}

QString Config::getServerAddress()
{
    const QReadLocker lock(mRwMutex);

    return mServerAddress;
}

void Config::setServerPort(int value)
{
    const QWriteLocker lock(mRwMutex);

    mServerPort = value;
}

int Config::getServerPort()
{
    const QReadLocker lock(mRwMutex);

    return mServerPort;
}

void Config::setNotificationsEnabled(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mNotificationsEnabled = value;
}

bool Config::isNotificationsEnabled()
{
    const QReadLocker lock(mRwMutex);

    return mNotificationsEnabled;
}

void Config::setFilterSystem(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mFilterSystem = value;
}

bool Config::isFilterSystem()
{
    const QReadLocker lock(mRwMutex);

    return mFilterSystem;
}

void Config::setFilterPortfolio(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mFilterPortfolio = value;
}

bool Config::isFilterPortfolio()
{
    const QReadLocker lock(mRwMutex);

    return mFilterPortfolio;
}

void Config::setFilterHugeSell(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mFilterHugeSell = value;
}

bool Config::isFilterHugeSell()
{
    const QReadLocker lock(mRwMutex);

    return mFilterHugeSell;
}

void Config::setFilterDividends(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mFilterDividends = value;
}

bool Config::isFilterDividends()
{
    const QReadLocker lock(mRwMutex);

    return mFilterDividends;
}

void Config::setFilterPulseNeutral(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mFilterPulseNeutral = value;
}

bool Config::isFilterPulseNeutral()
{
    const QReadLocker lock(mRwMutex);

    return mFilterPulseNeutral;
}

void Config::setFilterPulseBuy(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mFilterPulseBuy = value;
}

bool Config::isFilterPulseBuy()
{
    const QReadLocker lock(mRwMutex);

    return mFilterPulseBuy;
}

void Config::setFilterPulseSell(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mFilterPulseSell = value;
}

bool Config::isFilterPulseSell()
{
    const QReadLocker lock(mRwMutex);

    return mFilterPulseSell;
}
