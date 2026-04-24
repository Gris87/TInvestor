#pragma once



#include "src/utils/settingseditor/isettingseditor.h"



class IConfig
{
public:
    IConfig()          = default;
    virtual ~IConfig() = default;

    IConfig(const IConfig& another)            = delete;
    IConfig& operator=(const IConfig& another) = delete;

    virtual void assign(IConfig* another) = 0;
    virtual void makeDefault()            = 0;

    virtual void save(ISettingsEditor* settingsEditor) = 0;
    virtual void load(ISettingsEditor* settingsEditor) = 0;

    virtual void setAutorun(bool value) = 0;
    virtual bool isAutorun()            = 0;

    virtual void    setServerAddress(const QString& value) = 0;
    virtual QString getServerAddress()                     = 0;

    virtual void setServerPort(int value) = 0;
    virtual int  getServerPort()          = 0;
};
