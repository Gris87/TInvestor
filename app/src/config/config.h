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

    void setTradeHugeSpread(bool value) override;
    bool isTradeHugeSpread() override;

    void  setHugeSpread(float value) override;
    float getHugeSpread() override;

    void setHugeSpreadLimitStockPurchase(bool value) override;
    bool isHugeSpreadLimitStockPurchase() override;

    void  setHugeSpreadLimitStockPurchasePart(float value) override;
    float getHugeSpreadLimitStockPurchasePart() override;

    void setHugeSpreadLimitByTurnover(bool value) override;
    bool isHugeSpreadLimitByTurnover() override;

    void  setHugeSpreadLimitByTurnoverPercent(float value) override;
    float getHugeSpreadLimitByTurnoverPercent() override;

    void setTradeLiquidityEtfNightly(bool value) override;
    bool isTradeLiquidityEtfNightly() override;

    void  setLiquidityEtfRemainedPartNightly(float value) override;
    float getLiquidityEtfRemainedPartNightly() override;

    void setLimitStockPurchase(bool value) override;
    bool isLimitStockPurchase() override;

    void  setLimitStockPurchasePart(float value) override;
    float getLimitStockPurchasePart() override;

    void setLimitByTurnover(bool value) override;
    bool isLimitByTurnover() override;

    void  setLimitByTurnoverPercent(float value) override;
    float getLimitByTurnoverPercent() override;

    void setLimitStockPurchaseNonWorkingHours(bool value) override;
    bool isLimitStockPurchaseNonWorkingHours() override;

    void  setLimitStockPurchasePartNonWorkingHours(float value) override;
    float getLimitStockPurchasePartNonWorkingHours() override;

    void setLimitByTurnoverNonWorkingHours(bool value) override;
    bool isLimitByTurnoverNonWorkingHours() override;

    void  setLimitByTurnoverPercentNonWorkingHours(float value) override;
    float getLimitByTurnoverPercentNonWorkingHours() override;

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
    bool    mTradeHugeSpread;
    float   mHugeSpread;
    bool    mHugeSpreadLimitStockPurchase;
    float   mHugeSpreadLimitStockPurchasePart;
    bool    mHugeSpreadLimitByTurnover;
    float   mHugeSpreadLimitByTurnoverPercent;
    bool    mTradeLiquidityEtfNightly;
    float   mLiquidityEtfRemainedPartNightly;
    bool    mLimitStockPurchase;
    float   mLimitStockPurchasePart;
    bool    mLimitByTurnover;
    float   mLimitByTurnoverPercent;
    bool    mLimitStockPurchaseNonWorkingHours;
    float   mLimitStockPurchasePartNonWorkingHours;
    bool    mLimitByTurnoverNonWorkingHours;
    float   mLimitByTurnoverPercentNonWorkingHours;
    int     mStorageMonthLimit;
    bool    mSimulatorConfigCommon;
    bool    mAutoPilotConfigCommon;
};
