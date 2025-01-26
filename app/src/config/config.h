#pragma once



#include "src/config/iconfig.h"

#include <QMutex>

#include "src/config/decisions/idecisionmakerconfig.h"
#include "src/config/decisions/idecisionmakerconfigfactory.h"



class Config : public IConfig
{
    Q_OBJECT

public:
    explicit Config(IDecisionMakerConfigFactory *decisionMakerConfigFactory, QObject *parent = nullptr);
    ~Config();

    void assign(IConfig *another) override;
    void makeDefault() override;

    void save() override;
    void load() override;

    IDecisionMakerConfig* getSimulatorConfig() override;
    IDecisionMakerConfig* getAutoPilotConfig() override;

    void setAutorun(bool value) override;
    bool isAutorun() override;

    void setRefreshTimeout(int value) override;
    int getRefreshTimeout() override;

    void setUseSchedule(bool value) override;
    bool isUseSchedule() override;

    void setScheduleStartHour(int value) override;
    int getScheduleStartHour() override;

    void setScheduleStartMinute(int value) override;
    int getScheduleStartMinute() override;

    void setScheduleEndHour(int value) override;
    int getScheduleEndHour() override;

    void setScheduleEndMinute(int value) override;
    int getScheduleEndMinute() override;

    void setLimitPurchasesPerDay(bool value) override;
    bool isLimitPurchasesPerDay() override;

    void setAmountOfPurchasesPerDay(int value) override;
    int getAmountOfPurchasesPerDay() override;

    void setLimitPurchasesPerStock(bool value) override;
    bool isLimitPurchasesPerStock() override;

    void setAmountOfPurchasesPerStock(int value) override;
    int getAmountOfPurchasesPerStock() override;

    void setCommission(float value) override;
    float getCommission() override;

    void setLimitStockPurchase(bool value) override;
    bool isLimitStockPurchase() override;

    void setAmountOfStockPurchase(int value) override;
    int getAmountOfStockPurchase() override;

    void setStorageMonthLimit(int value) override;
    int getStorageMonthLimit() override;

    void setSimulatorConfigCommon(bool value) override;
    bool isSimulatorConfigCommon() override;

    void setAutoPilotConfigCommon(bool value) override;
    bool isAutoPilotConfigCommon() override;

private:
    QMutex *mMutex;

    IDecisionMakerConfig *mSimulatorConfig;
    IDecisionMakerConfig *mAutoPilotConfig;

    bool   mAutorun;
    int    mRefreshTimeout;
    bool   mUseSchedule;
    int    mScheduleStartHour;
    int    mScheduleStartMinute;
    int    mScheduleEndHour;
    int    mScheduleEndMinute;
    bool   mLimitPurchasesPerDay;
    int    mAmountOfPurchasesPerDay;
    bool   mLimitPurchasesPerStock;
    int    mAmountOfPurchasesPerStock;
    float  mCommission;
    bool   mLimitStockPurchase;
    int    mAmountOfStockPurchase;
    int    mStorageMonthLimit;
    bool   mSimulatorConfigCommon;
    bool   mAutoPilotConfigCommon;
};
