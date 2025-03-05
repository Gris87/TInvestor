#pragma once



#include "src/config/iconfig.h"

#include <QMutex>



class Config : public IConfig
{
public:
    explicit Config(IDecisionMakerConfig* simulatorConfig, IDecisionMakerConfig* autoPilotConfig);
    ~Config();

    Config(const Config& another)            = delete;
    Config& operator=(const Config& another) = delete;

    void assign(IConfig* another) override;
    void makeDefault() override;

    void save(ISettingsEditor* settingsEditor) override;
    void load(ISettingsEditor* settingsEditor) override;

    IDecisionMakerConfig* getSimulatorConfig() override;
    IDecisionMakerConfig* getAutoPilotConfig() override;

    void setAutorun(bool value) override;
    bool isAutorun() override;

    void setMakeDecisionTimeout(int value) override;
    int  getMakeDecisionTimeout() override;

    void setUseSchedule(bool value) override;
    bool isUseSchedule() override;

    void setScheduleStartHour(int value) override;
    int  getScheduleStartHour() override;

    void setScheduleStartMinute(int value) override;
    int  getScheduleStartMinute() override;

    void setScheduleEndHour(int value) override;
    int  getScheduleEndHour() override;

    void setScheduleEndMinute(int value) override;
    int  getScheduleEndMinute() override;

    void setLimitStockPurchase(bool value) override;
    bool isLimitStockPurchase() override;

    void setAmountOfStockPurchase(int value) override;
    int  getAmountOfStockPurchase() override;

    void setStorageMonthLimit(int value) override;
    int  getStorageMonthLimit() override;

    void setSimulatorConfigCommon(bool value) override;
    bool isSimulatorConfigCommon() override;

    void setAutoPilotConfigCommon(bool value) override;
    bool isAutoPilotConfigCommon() override;

private:
    QMutex* mMutex;

    IDecisionMakerConfig* mSimulatorConfig;
    IDecisionMakerConfig* mAutoPilotConfig;

    bool mAutorun;
    int  mMakeDecisionTimeout;
    bool mUseSchedule;
    int  mScheduleStartHour;
    int  mScheduleStartMinute;
    int  mScheduleEndHour;
    int  mScheduleEndMinute;
    bool mLimitStockPurchase;
    int  mAmountOfStockPurchase;
    int  mStorageMonthLimit;
    bool mSimulatorConfigCommon;
    bool mAutoPilotConfigCommon;
};
