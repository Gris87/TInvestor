#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

#include <QMutex>

#include "src/config/decisionmakerconfig.h"



class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config(QObject *parent = nullptr);
    explicit Config(const Config &config, QObject *parent = nullptr);
    ~Config();

    DecisionMakerConfig simulatorConfig;
    DecisionMakerConfig autoPilotConfig;

    Config& operator=(const Config &config);

    void makeDefault();

    void save();
    void load();

    void setAutorun(bool value);
    bool isAutorun();

    void setRefreshTimeout(int value);
    int getRefreshTimeout();

    void setUseSchedule(bool value);
    bool isUseSchedule();

    void setScheduleStartHour(int value);
    int getScheduleStartHour();

    void setScheduleStartMinute(int value);
    int getScheduleStartMinute();

    void setScheduleEndHour(int value);
    int getScheduleEndHour();

    void setScheduleEndMinute(int value);
    int getScheduleEndMinute();

    void setLimitPurchasesPerDay(bool value);
    bool isLimitPurchasesPerDay();

    void setAmountOfPurchasesPerDay(int value);
    int getAmountOfPurchasesPerDay();

    void setLimitPurchasesPerStock(bool value);
    bool isLimitPurchasesPerStock();

    void setAmountOfPurchasesPerStock(int value);
    int getAmountOfPurchasesPerStock();

    void setCommission(float value);
    float getCommission();

    void setLimitStockPurchase(bool value);
    bool isLimitStockPurchase();

    void setAmountOfStockPurchase(int value);
    int getAmountOfStockPurchase();

    void setStorageMonthLimit(int value);
    int getStorageMonthLimit();

    void setSimulatorConfigCommon(bool value);
    bool isSimulatorConfigCommon();

    void setAutoPilotConfigCommon(bool value);
    bool isAutoPilotConfigCommon();

private:
    void assign(const Config &config);

    QMutex mMutex;
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



#endif // CONFIG_H
