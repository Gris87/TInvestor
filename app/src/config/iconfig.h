#pragma once



#include "src/config/decisions/idecisionmakerconfig.h"
#include "src/utils/settingseditor/isettingseditor.h"



class IConfig
{
public:
    IConfig()          = default;
    virtual ~IConfig() = default;

    IConfig(const IConfig& another)            = delete;
    IConfig& operator=(const IConfig& another) = delete;

    virtual IConfig* clone()                       = 0;
    virtual void     deleteRecursively()           = 0;
    virtual void     assign(IConfig* another)      = 0;
    virtual void     makeDefault(float commission) = 0;

    virtual void save(ISettingsEditor* settingsEditor) = 0;
    virtual void load(ISettingsEditor* settingsEditor) = 0;

    virtual IDecisionMakerConfig* getSimulatorConfig() = 0;
    virtual IDecisionMakerConfig* getAutoPilotConfig() = 0;

    virtual void setAutorun(bool value) = 0;
    virtual bool isAutorun()            = 0;

    virtual void    setCpuUsage(const QString& value) = 0;
    virtual QString getCpuUsage()                     = 0;

    virtual void setMakeDecisionTimeout(int value) = 0;
    virtual int  getMakeDecisionTimeout()          = 0;

    virtual void setTradeInNonWorkingHours(bool value) = 0;
    virtual bool isTradeInNonWorkingHours()            = 0;

    virtual void setTradeHugeBid(bool value) = 0;
    virtual bool isTradeHugeBid()            = 0;

    virtual void  setHugeBid(float value) = 0;
    virtual float getHugeBid()            = 0;

    virtual void setHugeBidLimitStockPurchase(bool value) = 0;
    virtual bool isHugeBidLimitStockPurchase()            = 0;

    virtual void  setHugeBidLimitStockPurchasePart(float value) = 0;
    virtual float getHugeBidLimitStockPurchasePart()            = 0;

    virtual void setHugeBidLimitByTurnover(bool value) = 0;
    virtual bool isHugeBidLimitByTurnover()            = 0;

    virtual void  setHugeBidLimitByTurnoverPercent(float value) = 0;
    virtual float getHugeBidLimitByTurnoverPercent()            = 0;

    virtual void setTradeHugeSpread(bool value) = 0;
    virtual bool isTradeHugeSpread()            = 0;

    virtual void  setHugeSpread(float value) = 0;
    virtual float getHugeSpread()            = 0;

    virtual void setHugeSpreadLimitStockPurchase(bool value) = 0;
    virtual bool isHugeSpreadLimitStockPurchase()            = 0;

    virtual void  setHugeSpreadLimitStockPurchasePart(float value) = 0;
    virtual float getHugeSpreadLimitStockPurchasePart()            = 0;

    virtual void setHugeSpreadLimitByTurnover(bool value) = 0;
    virtual bool isHugeSpreadLimitByTurnover()            = 0;

    virtual void  setHugeSpreadLimitByTurnoverPercent(float value) = 0;
    virtual float getHugeSpreadLimitByTurnoverPercent()            = 0;

    virtual void setTradeLiquidityEtfNightly(bool value) = 0;
    virtual bool isTradeLiquidityEtfNightly()            = 0;

    virtual void  setLiquidityEtfRemainedPartNightly(float value) = 0;
    virtual float getLiquidityEtfRemainedPartNightly()            = 0;

    virtual void setLimitStockPurchase(bool value) = 0;
    virtual bool isLimitStockPurchase()            = 0;

    virtual void  setLimitStockPurchasePart(float value) = 0;
    virtual float getLimitStockPurchasePart()            = 0;

    virtual void setLimitByTurnover(bool value) = 0;
    virtual bool isLimitByTurnover()            = 0;

    virtual void  setLimitByTurnoverPercent(float value) = 0;
    virtual float getLimitByTurnoverPercent()            = 0;

    virtual void setLimitStockPurchaseNonWorkingHours(bool value) = 0;
    virtual bool isLimitStockPurchaseNonWorkingHours()            = 0;

    virtual void  setLimitStockPurchasePartNonWorkingHours(float value) = 0;
    virtual float getLimitStockPurchasePartNonWorkingHours()            = 0;

    virtual void setLimitByTurnoverNonWorkingHours(bool value) = 0;
    virtual bool isLimitByTurnoverNonWorkingHours()            = 0;

    virtual void  setLimitByTurnoverPercentNonWorkingHours(float value) = 0;
    virtual float getLimitByTurnoverPercentNonWorkingHours()            = 0;

    virtual void setStorageMonthLimit(int value) = 0;
    virtual int  getStorageMonthLimit()          = 0;

    virtual void setHighlightGoodOperations(bool value) = 0;
    virtual bool isHighlightGoodOperations()            = 0;

    virtual void  setHighlightGoodOperationsYield(float value) = 0;
    virtual float getHighlightGoodOperationsYield()            = 0;

    virtual void setHighlightBadOperations(bool value) = 0;
    virtual bool isHighlightBadOperations()            = 0;

    virtual void  setHighlightBadOperationsLose(float value) = 0;
    virtual float getHighlightBadOperationsLose()            = 0;

    virtual void setSimulatorConfigCommon(bool value) = 0;
    virtual bool isSimulatorConfigCommon()            = 0;

    virtual void setAutoPilotConfigCommon(bool value) = 0;
    virtual bool isAutoPilotConfigCommon()            = 0;
};
