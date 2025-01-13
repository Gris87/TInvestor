#include "config.h"

#include <QDebug>
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
    qDebug() << "Set config to default";

    mUseForSimulatorAndAutoPilot = true;
}

void Config::assign(const Config &config)
{
    qDebug() << "Assigning config to config";

    mUseForSimulatorAndAutoPilot = config.mUseForSimulatorAndAutoPilot;
}

void Config::save()
{
    qDebug() << "Save config";

    QSettings settings("GrisCom", "TInvestor");
    settings.setValue("Config/UseForSimulatorAndAutoPilot", mUseForSimulatorAndAutoPilot);
}

void Config::load()
{
    qDebug() << "Load config";

    QSettings settings("GrisCom", "TInvestor");
    mUseForSimulatorAndAutoPilot = settings.value("Config/UseForSimulatorAndAutoPilot").toBool();
}
