#pragma once



#include "src/config/iconfig.h"

#include <gmock/gmock.h>



class ConfigMock : public IConfig
{
public:
    ConfigMock() :
        IConfig()
    {
    }
    ~ConfigMock() override = default;

    ConfigMock(const ConfigMock& another)            = delete;
    ConfigMock& operator=(const ConfigMock& another) = delete;

    MOCK_METHOD(IConfig*, clone, (), (override));
    MOCK_METHOD(void, deleteRecursively, (), (override));
    MOCK_METHOD(void, assign, (IConfig * another), (override));
    MOCK_METHOD(void, makeDefault, (float commission), (override));

    MOCK_METHOD(void, save, (ISettingsEditor * settingsEditor), (override));
    MOCK_METHOD(void, load, (ISettingsEditor * settingsEditor), (override));

    MOCK_METHOD(IDecisionMakerConfig*, getSimulatorConfig, (), (override));
    MOCK_METHOD(IDecisionMakerConfig*, getAutoPilotConfig, (), (override));

    MOCK_METHOD(void, setAutorun, (bool value), (override));
    MOCK_METHOD(bool, isAutorun, (), (override));

    MOCK_METHOD(void, setCpuUsage, (const QString& value), (override));
    MOCK_METHOD(QString, getCpuUsage, (), (override));

    MOCK_METHOD(void, setTradeInNonWorkingHours, (bool value), (override));
    MOCK_METHOD(bool, isTradeInNonWorkingHours, (), (override));

    MOCK_METHOD(void, setTradeHugeBid, (bool value), (override));
    MOCK_METHOD(bool, isTradeHugeBid, (), (override));

    MOCK_METHOD(void, setHugeBid, (float value), (override));
    MOCK_METHOD(float, getHugeBid, (), (override));

    MOCK_METHOD(void, setHugeBidLimitStockPurchase, (bool value), (override));
    MOCK_METHOD(bool, isHugeBidLimitStockPurchase, (), (override));

    MOCK_METHOD(void, setHugeBidLimitStockPurchasePart, (float value), (override));
    MOCK_METHOD(float, getHugeBidLimitStockPurchasePart, (), (override));

    MOCK_METHOD(void, setHugeBidLimitByTurnover, (bool value), (override));
    MOCK_METHOD(bool, isHugeBidLimitByTurnover, (), (override));

    MOCK_METHOD(void, setHugeBidLimitByTurnoverPercent, (float value), (override));
    MOCK_METHOD(float, getHugeBidLimitByTurnoverPercent, (), (override));

    MOCK_METHOD(void, setTradeHugeSpread, (bool value), (override));
    MOCK_METHOD(bool, isTradeHugeSpread, (), (override));

    MOCK_METHOD(void, setHugeSpread, (float value), (override));
    MOCK_METHOD(float, getHugeSpread, (), (override));

    MOCK_METHOD(void, setHugeSpreadLimitStockPurchase, (bool value), (override));
    MOCK_METHOD(bool, isHugeSpreadLimitStockPurchase, (), (override));

    MOCK_METHOD(void, setHugeSpreadLimitStockPurchasePart, (float value), (override));
    MOCK_METHOD(float, getHugeSpreadLimitStockPurchasePart, (), (override));

    MOCK_METHOD(void, setHugeSpreadLimitByTurnover, (bool value), (override));
    MOCK_METHOD(bool, isHugeSpreadLimitByTurnover, (), (override));

    MOCK_METHOD(void, setHugeSpreadLimitByTurnoverPercent, (float value), (override));
    MOCK_METHOD(float, getHugeSpreadLimitByTurnoverPercent, (), (override));

    MOCK_METHOD(void, setTradeLiquidityEtfNightly, (bool value), (override));
    MOCK_METHOD(bool, isTradeLiquidityEtfNightly, (), (override));

    MOCK_METHOD(void, setLiquidityEtfRemainedPartNightly, (float value), (override));
    MOCK_METHOD(float, getLiquidityEtfRemainedPartNightly, (), (override));

    MOCK_METHOD(void, setLimitStockPurchase, (bool value), (override));
    MOCK_METHOD(bool, isLimitStockPurchase, (), (override));

    MOCK_METHOD(void, setLimitStockPurchasePart, (float value), (override));
    MOCK_METHOD(float, getLimitStockPurchasePart, (), (override));

    MOCK_METHOD(void, setLimitByTurnover, (bool value), (override));
    MOCK_METHOD(bool, isLimitByTurnover, (), (override));

    MOCK_METHOD(void, setLimitByTurnoverPercent, (float value), (override));
    MOCK_METHOD(float, getLimitByTurnoverPercent, (), (override));

    MOCK_METHOD(void, setLimitStockPurchaseNonWorkingHours, (bool value), (override));
    MOCK_METHOD(bool, isLimitStockPurchaseNonWorkingHours, (), (override));

    MOCK_METHOD(void, setLimitStockPurchasePartNonWorkingHours, (float value), (override));
    MOCK_METHOD(float, getLimitStockPurchasePartNonWorkingHours, (), (override));

    MOCK_METHOD(void, setLimitByTurnoverNonWorkingHours, (bool value), (override));
    MOCK_METHOD(bool, isLimitByTurnoverNonWorkingHours, (), (override));

    MOCK_METHOD(void, setLimitByTurnoverPercentNonWorkingHours, (float value), (override));
    MOCK_METHOD(float, getLimitByTurnoverPercentNonWorkingHours, (), (override));

    MOCK_METHOD(void, setStorageMonthLimit, (int value), (override));
    MOCK_METHOD(int, getStorageMonthLimit, (), (override));

    MOCK_METHOD(void, setHighlightGoodOperations, (bool value), (override));
    MOCK_METHOD(bool, isHighlightGoodOperations, (), (override));

    MOCK_METHOD(void, setHighlightGoodOperationsYield, (float value), (override));
    MOCK_METHOD(float, getHighlightGoodOperationsYield, (), (override));

    MOCK_METHOD(void, setHighlightBadOperations, (bool value), (override));
    MOCK_METHOD(bool, isHighlightBadOperations, (), (override));

    MOCK_METHOD(void, setHighlightBadOperationsLose, (float value), (override));
    MOCK_METHOD(float, getHighlightBadOperationsLose, (), (override));

    MOCK_METHOD(void, setSimulatorConfigCommon, (bool value), (override));
    MOCK_METHOD(bool, isSimulatorConfigCommon, (), (override));

    MOCK_METHOD(void, setAutoPilotConfigCommon, (bool value), (override));
    MOCK_METHOD(bool, isAutoPilotConfigCommon, (), (override));
};
