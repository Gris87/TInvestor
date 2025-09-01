#pragma once



#include "src/config/iconfig.h"

#include <QReadWriteLock>



class Config : public IConfig
{
public:
    explicit Config(IDecisionMakerConfig* simulatorConfig, IDecisionMakerConfig* autoPilotConfig);
    ~Config() override;

    Config(const Config& another)            = delete;
    Config& operator=(const Config& another) = delete;

    IConfig* clone() override;
    void     deleteRecursively() override;
    void     assign(IConfig* another) override;
    void     makeDefault() override;

    void save(ISettingsEditor* settingsEditor) override;
    void load(ISettingsEditor* settingsEditor) override;

    IDecisionMakerConfig* getSimulatorConfig() override;
    IDecisionMakerConfig* getAutoPilotConfig() override;

    void setAutorun(bool value) override;
    bool isAutorun() override;

    void    setCpuUsage(const QString& value) override;
    QString getCpuUsage() override;

    void setMakeDecisionTimeout(int value) override;
    int  getMakeDecisionTimeout() override;

    void setTradeInNonWorkingHours(bool value) override;
    bool isTradeInNonWorkingHours() override;

    void setLimitStockPurchase(bool value) override;
    bool isLimitStockPurchase() override;

    void  setLimitStockPurchasePart(float value) override;
    float getLimitStockPurchasePart() override;

    void setLimitByTurnover(bool value) override;
    bool isLimitByTurnover() override;

    void  setLimitByTurnoverPercent(float value) override;
    float getLimitByTurnoverPercent() override;

    void setStorageMonthLimit(int value) override;
    int  getStorageMonthLimit() override;

    void setSimulatorConfigCommon(bool value) override;
    bool isSimulatorConfigCommon() override;

    void setAutoPilotConfigCommon(bool value) override;
    bool isAutoPilotConfigCommon() override;

private:
    QReadWriteLock* mRwMutex;

    IDecisionMakerConfig* mSimulatorConfig;
    IDecisionMakerConfig* mAutoPilotConfig;

    bool    mAutorun;
    QString mCpuUsage;
    int     mMakeDecisionTimeout;
    bool    mTradeInNonWorkingHours;
    bool    mLimitStockPurchase;
    float   mLimitStockPurchasePart;
    bool    mLimitByTurnover;
    float   mLimitByTurnoverPercent;
    int     mStorageMonthLimit;
    bool    mSimulatorConfigCommon;
    bool    mAutoPilotConfigCommon;
};
