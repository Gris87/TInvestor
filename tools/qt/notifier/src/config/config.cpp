#include "src/config/config.h"

#include <QDebug>



const char* const SERVER_ADDRESS_DEFAULT = "localhost";

constexpr bool AUTORUN_DEFAULT     = true;
constexpr int  SERVER_PORT_DEFAULT = 8041;



Config::Config() :
    IConfig(),
    mRwMutex(new QReadWriteLock()),
    mAutorun(),
    mServerAddress(),
    mServerPort()
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

    mAutorun       = config.mAutorun;
    mServerAddress = config.mServerAddress;
    mServerPort    = config.mServerPort;
}

void Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qInfo() << "Set Config to default";

    mAutorun       = AUTORUN_DEFAULT;
    mServerAddress = SERVER_ADDRESS_DEFAULT;
    mServerPort    = SERVER_PORT_DEFAULT;
}

void Config::save(ISettingsEditor* settingsEditor)
{
    const QReadLocker lock(mRwMutex);

    qInfo() << "Save Config";

    // clang-format off
    settingsEditor->setValue("Config/Autorun",                               mAutorun);
    settingsEditor->setValue("Config/ServerAddress",                              mServerAddress);
    settingsEditor->setValue("Config/ServerPort",                     mServerPort);
    // clang-format on
}

void Config::load(ISettingsEditor* settingsEditor)
{
    const QWriteLocker lock(mRwMutex);

    qInfo() << "Load Config";

    // clang-format off
    mAutorun       = settingsEditor->value("Config/Autorun",       mAutorun).toBool();
    mServerAddress = settingsEditor->value("Config/ServerAddress", mServerAddress).toString();
    mServerPort    = settingsEditor->value("Config/ServerPort",    mServerPort).toInt();
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
