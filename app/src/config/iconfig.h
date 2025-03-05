#pragma once



#include "src/config/decisions/idecisionmakerconfig.h"
#include "src/utils/settingseditor/isettingseditor.h"



class IConfig
{
public:
    IConfig()
    {
    }
    virtual ~IConfig() = default;

    IConfig(const IConfig& another)            = delete;
    IConfig& operator=(const IConfig& another) = delete;

    virtual void assign(IConfig* another) = 0;
    virtual void makeDefault()            = 0;

    virtual void save(ISettingsEditor* settingsEditor) = 0;
    virtual void load(ISettingsEditor* settingsEditor) = 0;

    virtual IDecisionMakerConfig* getSimulatorConfig() = 0;
    virtual IDecisionMakerConfig* getAutoPilotConfig() = 0;

    virtual void setAutorun(bool value) = 0;
    virtual bool isAutorun()            = 0;

    virtual void setMakeDecisionTimeout(int value) = 0;
    virtual int  getMakeDecisionTimeout()          = 0;

    virtual void setUseSchedule(bool value) = 0;
    virtual bool isUseSchedule()            = 0;

    virtual void setScheduleStartHour(int value) = 0;
    virtual int  getScheduleStartHour()          = 0;

    virtual void setScheduleStartMinute(int value) = 0;
    virtual int  getScheduleStartMinute()          = 0;

    virtual void setScheduleEndHour(int value) = 0;
    virtual int  getScheduleEndHour()          = 0;

    virtual void setScheduleEndMinute(int value) = 0;
    virtual int  getScheduleEndMinute()          = 0;

    virtual void setLimitStockPurchase(bool value) = 0;
    virtual bool isLimitStockPurchase()            = 0;

    virtual void setAmountOfStockPurchase(int value) = 0;
    virtual int  getAmountOfStockPurchase()          = 0;

    virtual void setStorageMonthLimit(int value) = 0;
    virtual int  getStorageMonthLimit()          = 0;

    virtual void setSimulatorConfigCommon(bool value) = 0;
    virtual bool isSimulatorConfigCommon()            = 0;

    virtual void setAutoPilotConfigCommon(bool value) = 0;
    virtual bool isAutoPilotConfigCommon()            = 0;
};
