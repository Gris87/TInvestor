#include "src/config/config.h"

#include <QDebug>



const char* const CPU_USAGE_DEFAULT = "MAXIMUM";

constexpr bool  AUTORUN_DEFAULT                                     = true;
constexpr int   MAKE_DECISION_TIMEOUT_DEFAULT                       = 1;
constexpr bool  TRADE_IN_NON_WORKING_HOURS_DEFAULT                  = true;
constexpr bool  TRADE_HUGE_BID_DEFAULT                              = true;
constexpr float HUGE_BID_DEFAULT                                    = 2.5f;
constexpr bool  HUGE_BID_LIMIT_STOCK_PURCHASE_DEFAULT               = true;
constexpr float HUGE_BID_LIMIT_STOCK_PURCHASE_PART_DEFAULT          = 3.0f;
constexpr bool  HUGE_BID_LIMIT_BY_TURNOVER_DEFAULT                  = true;
constexpr float HUGE_BID_LIMIT_BY_TURNOVER_PERCENT_DEFAULT          = 1.0f;
constexpr bool  TRADE_HUGE_SPREAD_DEFAULT                           = true;
constexpr float HUGE_SPREAD_DEFAULT                                 = 0.7f;
constexpr bool  HUGE_SPREAD_LIMIT_STOCK_PURCHASE_DEFAULT            = true;
constexpr float HUGE_SPREAD_LIMIT_STOCK_PURCHASE_PART_DEFAULT       = 3.0f;
constexpr bool  HUGE_SPREAD_LIMIT_BY_TURNOVER_DEFAULT               = true;
constexpr float HUGE_SPREAD_LIMIT_BY_TURNOVER_PERCENT_DEFAULT       = 1.0f;
constexpr bool  TRADE_LIQUIDITY_ETF_NIGHTLY_DEFAULT                 = true;
constexpr float LIQUIDITY_ETF_REMAINED_PART_NIGHTLY_DEFAULT         = 5.0f;
constexpr bool  LIMIT_STOCK_PURCHASE_DEFAULT                        = true;
constexpr float LIMIT_STOCK_PURCHASE_PART_DEFAULT                   = 10.0f;
constexpr bool  LIMIT_BY_TURNOVER_DEFAULT                           = true;
constexpr float LIMIT_BY_TURNOVER_PERCENT_DEFAULT                   = 5.0f;
constexpr bool  LIMIT_STOCK_PURCHASE_NON_WORKING_HOURS_DEFAULT      = true;
constexpr float LIMIT_STOCK_PURCHASE_PART_NON_WORKING_HOURS_DEFAULT = 1.0f;
constexpr bool  LIMIT_BY_TURNOVER_NON_WORKING_HOURS_DEFAULT         = true;
constexpr float LIMIT_BY_TURNOVER_PERCENT_NON_WORKING_HOURS_DEFAULT = 1.0f;
constexpr int   STORAGE_MONTH_LIMIT_DEFAULT                         = 12;
constexpr bool  HIGHLIGHT_GOOD_OPERATIONS_DEFAULT                   = true;
constexpr float HIGHLIGHT_GOOD_OPERATIONS_YIELD_DEFAULT             = 1.0f;
constexpr bool  HIGHLIGHT_BAD_OPERATIONS_DEFAULT                    = true;
constexpr float HIGHLIGHT_BAD_OPERATIONS_LOSE_DEFAULT               = 2.0f;
constexpr bool  SIMULATOR_CONFIG_COMMON_DEFAULT                     = true;
constexpr bool  AUTOPILOT_CONFIG_COMMON_DEFAULT                     = false;



Config::Config(IDecisionMakerConfig* simulatorConfig, IDecisionMakerConfig* autoPilotConfig) :
    IConfig(),
    mRwMutex(new QReadWriteLock()),
    mSimulatorConfig(simulatorConfig),
    mAutoPilotConfig(autoPilotConfig),
    mAutorun(),
    mCpuUsage(),
    mMakeDecisionTimeout(),
    mTradeInNonWorkingHours(),
    mTradeHugeBid(),
    mHugeBid(),
    mHugeBidLimitStockPurchase(),
    mHugeBidLimitStockPurchasePart(),
    mHugeBidLimitByTurnover(),
    mHugeBidLimitByTurnoverPercent(),
    mTradeHugeSpread(),
    mHugeSpread(),
    mHugeSpreadLimitStockPurchase(),
    mHugeSpreadLimitStockPurchasePart(),
    mHugeSpreadLimitByTurnover(),
    mHugeSpreadLimitByTurnoverPercent(),
    mTradeLiquidityEtfNightly(),
    mLiquidityEtfRemainedPartNightly(),
    mLimitStockPurchase(),
    mLimitStockPurchasePart(),
    mLimitByTurnover(),
    mLimitByTurnoverPercent(),
    mLimitStockPurchaseNonWorkingHours(),
    mLimitStockPurchasePartNonWorkingHours(),
    mLimitByTurnoverNonWorkingHours(),
    mLimitByTurnoverPercentNonWorkingHours(),
    mStorageMonthLimit(),
    mHighlightGoodOperations(),
    mHighlightGoodOperationsYield(),
    mHighlightBadOperations(),
    mHighlightBadOperationsLose(),
    mSimulatorConfigCommon(),
    mAutoPilotConfigCommon()
{
    qDebug() << "Create Config";
}

Config::~Config()
{
    qDebug() << "Destroy Config";

    delete mRwMutex;
}

IConfig* Config::clone()
{
    IDecisionMakerConfig* simulatorConfig = mSimulatorConfig->clone();
    IDecisionMakerConfig* autoPilotConfig = mAutoPilotConfig->clone();

    Config* res = new Config(simulatorConfig, autoPilotConfig);
    res->assign(this);

    return res;
}

void Config::deleteRecursively()
{
    mSimulatorConfig->deleteRecursively();
    mAutoPilotConfig->deleteRecursively();

    delete this;
}

void Config::assign(IConfig* another)
{
    const QWriteLocker lock(mRwMutex);

    qDebug() << "Assigning Config to Config";

    const Config&     config = *dynamic_cast<Config*>(another);
    const QReadLocker lock2(config.mRwMutex);

    mSimulatorConfig->assign(config.mSimulatorConfig);
    mAutoPilotConfig->assign(config.mAutoPilotConfig);

    mAutorun                               = config.mAutorun;
    mCpuUsage                              = config.mCpuUsage;
    mMakeDecisionTimeout                   = config.mMakeDecisionTimeout;
    mTradeInNonWorkingHours                = config.mTradeInNonWorkingHours;
    mTradeHugeBid                          = config.mTradeHugeBid;
    mHugeBid                               = config.mHugeBid;
    mHugeBidLimitStockPurchase             = config.mHugeBidLimitStockPurchase;
    mHugeBidLimitStockPurchasePart         = config.mHugeBidLimitStockPurchasePart;
    mHugeBidLimitByTurnover                = config.mHugeBidLimitByTurnover;
    mHugeBidLimitByTurnoverPercent         = config.mHugeBidLimitByTurnoverPercent;
    mTradeHugeSpread                       = config.mTradeHugeSpread;
    mHugeSpread                            = config.mHugeSpread;
    mHugeSpreadLimitStockPurchase          = config.mHugeSpreadLimitStockPurchase;
    mHugeSpreadLimitStockPurchasePart      = config.mHugeSpreadLimitStockPurchasePart;
    mHugeSpreadLimitByTurnover             = config.mHugeSpreadLimitByTurnover;
    mHugeSpreadLimitByTurnoverPercent      = config.mHugeSpreadLimitByTurnoverPercent;
    mTradeLiquidityEtfNightly              = config.mTradeLiquidityEtfNightly;
    mLiquidityEtfRemainedPartNightly       = config.mLiquidityEtfRemainedPartNightly;
    mLimitStockPurchase                    = config.mLimitStockPurchase;
    mLimitStockPurchasePart                = config.mLimitStockPurchasePart;
    mLimitByTurnover                       = config.mLimitByTurnover;
    mLimitByTurnoverPercent                = config.mLimitByTurnoverPercent;
    mLimitStockPurchaseNonWorkingHours     = config.mLimitStockPurchaseNonWorkingHours;
    mLimitStockPurchasePartNonWorkingHours = config.mLimitStockPurchasePartNonWorkingHours;
    mLimitByTurnoverNonWorkingHours        = config.mLimitByTurnoverNonWorkingHours;
    mLimitByTurnoverPercentNonWorkingHours = config.mLimitByTurnoverPercentNonWorkingHours;
    mStorageMonthLimit                     = config.mStorageMonthLimit;
    mHighlightGoodOperations               = config.mHighlightGoodOperations;
    mHighlightGoodOperationsYield          = config.mHighlightGoodOperationsYield;
    mHighlightBadOperations                = config.mHighlightBadOperations;
    mHighlightBadOperationsLose            = config.mHighlightBadOperationsLose;
    mSimulatorConfigCommon                 = config.mSimulatorConfigCommon;
    mAutoPilotConfigCommon                 = config.mAutoPilotConfigCommon;
}

void Config::makeDefault(float commission)
{
    const QWriteLocker lock(mRwMutex);

    qInfo() << "Set Config to default";

    mSimulatorConfig->makeDefault(qRound(commission * 100));
    mAutoPilotConfig->makeDefault(qRound(commission * 100));

    mAutorun                               = AUTORUN_DEFAULT;
    mCpuUsage                              = CPU_USAGE_DEFAULT;
    mMakeDecisionTimeout                   = MAKE_DECISION_TIMEOUT_DEFAULT;
    mTradeInNonWorkingHours                = TRADE_IN_NON_WORKING_HOURS_DEFAULT;
    mTradeHugeBid                          = TRADE_HUGE_BID_DEFAULT;
    mHugeBid                               = HUGE_BID_DEFAULT;
    mHugeBidLimitStockPurchase             = HUGE_BID_LIMIT_STOCK_PURCHASE_DEFAULT;
    mHugeBidLimitStockPurchasePart         = HUGE_BID_LIMIT_STOCK_PURCHASE_PART_DEFAULT;
    mHugeBidLimitByTurnover                = HUGE_BID_LIMIT_BY_TURNOVER_DEFAULT;
    mHugeBidLimitByTurnoverPercent         = HUGE_BID_LIMIT_BY_TURNOVER_PERCENT_DEFAULT;
    mTradeHugeSpread                       = TRADE_HUGE_SPREAD_DEFAULT;
    mHugeSpread                            = HUGE_SPREAD_DEFAULT;
    mHugeSpreadLimitStockPurchase          = HUGE_SPREAD_LIMIT_STOCK_PURCHASE_DEFAULT;
    mHugeSpreadLimitStockPurchasePart      = HUGE_SPREAD_LIMIT_STOCK_PURCHASE_PART_DEFAULT;
    mHugeSpreadLimitByTurnover             = HUGE_SPREAD_LIMIT_BY_TURNOVER_DEFAULT;
    mHugeSpreadLimitByTurnoverPercent      = HUGE_SPREAD_LIMIT_BY_TURNOVER_PERCENT_DEFAULT;
    mTradeLiquidityEtfNightly              = TRADE_LIQUIDITY_ETF_NIGHTLY_DEFAULT;
    mLiquidityEtfRemainedPartNightly       = LIQUIDITY_ETF_REMAINED_PART_NIGHTLY_DEFAULT;
    mLimitStockPurchase                    = LIMIT_STOCK_PURCHASE_DEFAULT;
    mLimitStockPurchasePart                = LIMIT_STOCK_PURCHASE_PART_DEFAULT;
    mLimitByTurnover                       = LIMIT_BY_TURNOVER_DEFAULT;
    mLimitByTurnoverPercent                = LIMIT_BY_TURNOVER_PERCENT_DEFAULT;
    mLimitStockPurchaseNonWorkingHours     = LIMIT_STOCK_PURCHASE_NON_WORKING_HOURS_DEFAULT;
    mLimitStockPurchasePartNonWorkingHours = LIMIT_STOCK_PURCHASE_PART_NON_WORKING_HOURS_DEFAULT;
    mLimitByTurnoverNonWorkingHours        = LIMIT_BY_TURNOVER_NON_WORKING_HOURS_DEFAULT;
    mLimitByTurnoverPercentNonWorkingHours = LIMIT_BY_TURNOVER_PERCENT_NON_WORKING_HOURS_DEFAULT;
    mStorageMonthLimit                     = STORAGE_MONTH_LIMIT_DEFAULT;
    mHighlightGoodOperations               = HIGHLIGHT_GOOD_OPERATIONS_DEFAULT;
    mHighlightGoodOperationsYield          = HIGHLIGHT_GOOD_OPERATIONS_YIELD_DEFAULT;
    mHighlightBadOperations                = HIGHLIGHT_BAD_OPERATIONS_DEFAULT;
    mHighlightBadOperationsLose            = HIGHLIGHT_BAD_OPERATIONS_LOSE_DEFAULT;
    mSimulatorConfigCommon                 = SIMULATOR_CONFIG_COMMON_DEFAULT;
    mAutoPilotConfigCommon                 = AUTOPILOT_CONFIG_COMMON_DEFAULT;
}

void Config::save(ISettingsEditor* settingsEditor)
{
    const QReadLocker lock(mRwMutex);

    qInfo() << "Save Config";

    mSimulatorConfig->save(settingsEditor, "Config/Simulator");
    mAutoPilotConfig->save(settingsEditor, "Config/AutoPilot");

    // clang-format off
    settingsEditor->setValue("Config/Autorun",                               mAutorun);
    settingsEditor->setValue("Config/CpuUsage",                              mCpuUsage);
    settingsEditor->setValue("Config/MakeDecisionTimeout",                   mMakeDecisionTimeout);
    settingsEditor->setValue("Config/TradeInNonWorkingHours",                mTradeInNonWorkingHours);
    settingsEditor->setValue("Config/TradeHugeBid",                          mTradeHugeBid);
    settingsEditor->setValue("Config/HugeBid",                               mHugeBid);
    settingsEditor->setValue("Config/HugeBidLimitStockPurchase",             mHugeBidLimitStockPurchase);
    settingsEditor->setValue("Config/HugeBidLimitStockPurchasePart",         mHugeBidLimitStockPurchasePart);
    settingsEditor->setValue("Config/HugeBidLimitByTurnover",                mHugeBidLimitByTurnover);
    settingsEditor->setValue("Config/HugeBidLimitByTurnoverPercent",         mHugeBidLimitByTurnoverPercent);
    settingsEditor->setValue("Config/TradeHugeSpread",                       mTradeHugeSpread);
    settingsEditor->setValue("Config/HugeSpread",                            mHugeSpread);
    settingsEditor->setValue("Config/HugeSpreadLimitStockPurchase",          mHugeSpreadLimitStockPurchase);
    settingsEditor->setValue("Config/HugeSpreadLimitStockPurchasePart",      mHugeSpreadLimitStockPurchasePart);
    settingsEditor->setValue("Config/HugeSpreadLimitByTurnover",             mHugeSpreadLimitByTurnover);
    settingsEditor->setValue("Config/HugeSpreadLimitByTurnoverPercent",      mHugeSpreadLimitByTurnoverPercent);
    settingsEditor->setValue("Config/TradeLiquidityEtfNightly",              mTradeLiquidityEtfNightly);
    settingsEditor->setValue("Config/LiquidityEtfRemainedPartNightly",       mLiquidityEtfRemainedPartNightly);
    settingsEditor->setValue("Config/LimitStockPurchase",                    mLimitStockPurchase);
    settingsEditor->setValue("Config/LimitStockPurchasePart",                mLimitStockPurchasePart);
    settingsEditor->setValue("Config/LimitByTurnover",                       mLimitByTurnover);
    settingsEditor->setValue("Config/LimitByTurnoverPercent",                mLimitByTurnoverPercent);
    settingsEditor->setValue("Config/LimitStockPurchaseNonWorkingHours",     mLimitStockPurchaseNonWorkingHours);
    settingsEditor->setValue("Config/LimitStockPurchasePartNonWorkingHours", mLimitStockPurchasePartNonWorkingHours);
    settingsEditor->setValue("Config/LimitByTurnoverNonWorkingHours",        mLimitByTurnoverNonWorkingHours);
    settingsEditor->setValue("Config/LimitByTurnoverPercentNonWorkingHours", mLimitByTurnoverPercentNonWorkingHours);
    settingsEditor->setValue("Config/StorageMonthLimit",                     mStorageMonthLimit);
    settingsEditor->setValue("Config/HighlightGoodOperations",               mHighlightGoodOperations);
    settingsEditor->setValue("Config/HighlightGoodOperationsYield",          mHighlightGoodOperationsYield);
    settingsEditor->setValue("Config/HighlightBadOperations",                mHighlightBadOperations);
    settingsEditor->setValue("Config/HighlightBadOperationsLose",            mHighlightBadOperationsLose);
    settingsEditor->setValue("Config/SimulatorConfigCommon",                 mSimulatorConfigCommon);
    settingsEditor->setValue("Config/AutoPilotConfigCommon",                 mAutoPilotConfigCommon);
    // clang-format on
}

void Config::load(ISettingsEditor* settingsEditor)
{
    const QWriteLocker lock(mRwMutex);

    qInfo() << "Load Config";

    mSimulatorConfig->load(settingsEditor, "Config/Simulator");
    mAutoPilotConfig->load(settingsEditor, "Config/AutoPilot");

    // clang-format off
    mAutorun                               = settingsEditor->value("Config/Autorun",                               mAutorun).toBool();
    mCpuUsage                              = settingsEditor->value("Config/CpuUsage",                              mCpuUsage).toString();
    mMakeDecisionTimeout                   = settingsEditor->value("Config/MakeDecisionTimeout",                   mMakeDecisionTimeout).toInt();
    mTradeInNonWorkingHours                = settingsEditor->value("Config/TradeInNonWorkingHours",                mTradeInNonWorkingHours).toBool();
    mTradeHugeBid                          = settingsEditor->value("Config/TradeHugeBid",                          mTradeHugeBid).toBool();
    mHugeBid                               = settingsEditor->value("Config/HugeBid",                               mHugeBid).toFloat();
    mHugeBidLimitStockPurchase             = settingsEditor->value("Config/HugeBidLimitStockPurchase",             mHugeBidLimitStockPurchase).toBool();
    mHugeBidLimitStockPurchasePart         = settingsEditor->value("Config/HugeBidLimitStockPurchasePart",         mHugeBidLimitStockPurchasePart).toFloat();
    mHugeBidLimitByTurnover                = settingsEditor->value("Config/HugeBidLimitByTurnover",                mHugeBidLimitByTurnover).toBool();
    mHugeBidLimitByTurnoverPercent         = settingsEditor->value("Config/HugeBidLimitByTurnoverPercent",         mHugeBidLimitByTurnoverPercent).toFloat();
    mTradeHugeSpread                       = settingsEditor->value("Config/TradeHugeSpread",                       mTradeHugeSpread).toBool();
    mHugeSpread                            = settingsEditor->value("Config/HugeSpread",                            mHugeSpread).toFloat();
    mHugeSpreadLimitStockPurchase          = settingsEditor->value("Config/HugeSpreadLimitStockPurchase",          mHugeSpreadLimitStockPurchase).toBool();
    mHugeSpreadLimitStockPurchasePart      = settingsEditor->value("Config/HugeSpreadLimitStockPurchasePart",      mHugeSpreadLimitStockPurchasePart).toFloat();
    mHugeSpreadLimitByTurnover             = settingsEditor->value("Config/HugeSpreadLimitByTurnover",             mHugeSpreadLimitByTurnover).toBool();
    mHugeSpreadLimitByTurnoverPercent      = settingsEditor->value("Config/HugeSpreadLimitByTurnoverPercent",      mHugeSpreadLimitByTurnoverPercent).toFloat();
    mTradeLiquidityEtfNightly              = settingsEditor->value("Config/TradeLiquidityEtfNightly",              mTradeLiquidityEtfNightly).toBool();
    mLiquidityEtfRemainedPartNightly       = settingsEditor->value("Config/LiquidityEtfRemainedPartNightly",       mLiquidityEtfRemainedPartNightly).toFloat();
    mLimitStockPurchase                    = settingsEditor->value("Config/LimitStockPurchase",                    mLimitStockPurchase).toBool();
    mLimitStockPurchasePart                = settingsEditor->value("Config/LimitStockPurchasePart",                mLimitStockPurchasePart).toFloat();
    mLimitByTurnover                       = settingsEditor->value("Config/LimitByTurnover",                       mLimitByTurnover).toBool();
    mLimitByTurnoverPercent                = settingsEditor->value("Config/LimitByTurnoverPercent",                mLimitByTurnoverPercent).toFloat();
    mLimitStockPurchaseNonWorkingHours     = settingsEditor->value("Config/LimitStockPurchaseNonWorkingHours",     mLimitStockPurchaseNonWorkingHours).toBool();
    mLimitStockPurchasePartNonWorkingHours = settingsEditor->value("Config/LimitStockPurchasePartNonWorkingHours", mLimitStockPurchasePartNonWorkingHours).toFloat();
    mLimitByTurnoverNonWorkingHours        = settingsEditor->value("Config/LimitByTurnoverNonWorkingHours",        mLimitByTurnoverNonWorkingHours).toBool();
    mLimitByTurnoverPercentNonWorkingHours = settingsEditor->value("Config/LimitByTurnoverPercentNonWorkingHours", mLimitByTurnoverPercentNonWorkingHours).toFloat();
    mStorageMonthLimit                     = settingsEditor->value("Config/StorageMonthLimit",                     mStorageMonthLimit).toInt();
    mHighlightGoodOperations               = settingsEditor->value("Config/HighlightGoodOperations",               mHighlightGoodOperations).toBool();
    mHighlightGoodOperationsYield          = settingsEditor->value("Config/HighlightGoodOperationsYield",          mHighlightGoodOperationsYield).toFloat();
    mHighlightBadOperations                = settingsEditor->value("Config/HighlightBadOperations",                mHighlightBadOperations).toBool();
    mHighlightBadOperationsLose            = settingsEditor->value("Config/HighlightBadOperationsLose",            mHighlightBadOperationsLose).toFloat();
    mSimulatorConfigCommon                 = settingsEditor->value("Config/SimulatorConfigCommon",                 mSimulatorConfigCommon).toBool();
    mAutoPilotConfigCommon                 = settingsEditor->value("Config/AutoPilotConfigCommon",                 mAutoPilotConfigCommon).toBool();
    // clang-format on
}

IDecisionMakerConfig* Config::getSimulatorConfig()
{
    return mSimulatorConfig;
}

IDecisionMakerConfig* Config::getAutoPilotConfig()
{
    return mAutoPilotConfig;
}

void Config::setAutorun(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mAutorun = value;
}

bool Config::isAutorun()
{
    const QReadLocker lock(mRwMutex);

    return mAutorun;
}

void Config::setCpuUsage(const QString& value)
{
    const QWriteLocker lock(mRwMutex);

    mCpuUsage = value;
}

QString Config::getCpuUsage()
{
    const QReadLocker lock(mRwMutex);

    return mCpuUsage;
}

void Config::setMakeDecisionTimeout(int value)
{
    const QWriteLocker lock(mRwMutex);

    mMakeDecisionTimeout = value;
}

int Config::getMakeDecisionTimeout()
{
    const QReadLocker lock(mRwMutex);

    return mMakeDecisionTimeout;
}

void Config::setTradeInNonWorkingHours(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mTradeInNonWorkingHours = value;
}

bool Config::isTradeInNonWorkingHours()
{
    const QReadLocker lock(mRwMutex);

    return mTradeInNonWorkingHours;
}

void Config::setTradeHugeBid(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mTradeHugeBid = value;
}

bool Config::isTradeHugeBid()
{
    const QReadLocker lock(mRwMutex);

    return mTradeHugeBid;
}

void Config::setHugeBid(float value)
{
    const QWriteLocker lock(mRwMutex);

    mHugeBid = value;
}

float Config::getHugeBid()
{
    const QReadLocker lock(mRwMutex);

    return mHugeBid;
}

void Config::setHugeBidLimitStockPurchase(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mHugeBidLimitStockPurchase = value;
}

bool Config::isHugeBidLimitStockPurchase()
{
    const QReadLocker lock(mRwMutex);

    return mHugeBidLimitStockPurchase;
}

void Config::setHugeBidLimitStockPurchasePart(float value)
{
    const QWriteLocker lock(mRwMutex);

    mHugeBidLimitStockPurchasePart = value;
}

float Config::getHugeBidLimitStockPurchasePart()
{
    const QReadLocker lock(mRwMutex);

    return mHugeBidLimitStockPurchasePart;
}

void Config::setHugeBidLimitByTurnover(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mHugeBidLimitByTurnover = value;
}

bool Config::isHugeBidLimitByTurnover()
{
    const QReadLocker lock(mRwMutex);

    return mHugeBidLimitByTurnover;
}

void Config::setHugeBidLimitByTurnoverPercent(float value)
{
    const QWriteLocker lock(mRwMutex);

    mHugeBidLimitByTurnoverPercent = value;
}

float Config::getHugeBidLimitByTurnoverPercent()
{
    const QReadLocker lock(mRwMutex);

    return mHugeBidLimitByTurnoverPercent;
}

void Config::setTradeHugeSpread(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mTradeHugeSpread = value;
}

bool Config::isTradeHugeSpread()
{
    const QReadLocker lock(mRwMutex);

    return mTradeHugeSpread;
}

void Config::setHugeSpread(float value)
{
    const QWriteLocker lock(mRwMutex);

    mHugeSpread = value;
}

float Config::getHugeSpread()
{
    const QReadLocker lock(mRwMutex);

    return mHugeSpread;
}

void Config::setHugeSpreadLimitStockPurchase(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mHugeSpreadLimitStockPurchase = value;
}

bool Config::isHugeSpreadLimitStockPurchase()
{
    const QReadLocker lock(mRwMutex);

    return mHugeSpreadLimitStockPurchase;
}

void Config::setHugeSpreadLimitStockPurchasePart(float value)
{
    const QWriteLocker lock(mRwMutex);

    mHugeSpreadLimitStockPurchasePart = value;
}

float Config::getHugeSpreadLimitStockPurchasePart()
{
    const QReadLocker lock(mRwMutex);

    return mHugeSpreadLimitStockPurchasePart;
}

void Config::setHugeSpreadLimitByTurnover(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mHugeSpreadLimitByTurnover = value;
}

bool Config::isHugeSpreadLimitByTurnover()
{
    const QReadLocker lock(mRwMutex);

    return mHugeSpreadLimitByTurnover;
}

void Config::setHugeSpreadLimitByTurnoverPercent(float value)
{
    const QWriteLocker lock(mRwMutex);

    mHugeSpreadLimitByTurnoverPercent = value;
}

float Config::getHugeSpreadLimitByTurnoverPercent()
{
    const QReadLocker lock(mRwMutex);

    return mHugeSpreadLimitByTurnoverPercent;
}

void Config::setTradeLiquidityEtfNightly(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mTradeLiquidityEtfNightly = value;
}

bool Config::isTradeLiquidityEtfNightly()
{
    const QReadLocker lock(mRwMutex);

    return mTradeLiquidityEtfNightly;
}

void Config::setLiquidityEtfRemainedPartNightly(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLiquidityEtfRemainedPartNightly = value;
}

float Config::getLiquidityEtfRemainedPartNightly()
{
    const QReadLocker lock(mRwMutex);

    return mLiquidityEtfRemainedPartNightly;
}

void Config::setLimitStockPurchase(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mLimitStockPurchase = value;
}

bool Config::isLimitStockPurchase()
{
    const QReadLocker lock(mRwMutex);

    return mLimitStockPurchase;
}

void Config::setLimitStockPurchasePart(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLimitStockPurchasePart = value;
}

float Config::getLimitStockPurchasePart()
{
    const QReadLocker lock(mRwMutex);

    return mLimitStockPurchasePart;
}

void Config::setLimitByTurnover(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mLimitByTurnover = value;
}

bool Config::isLimitByTurnover()
{
    const QReadLocker lock(mRwMutex);

    return mLimitByTurnover;
}

void Config::setLimitByTurnoverPercent(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLimitByTurnoverPercent = value;
}

float Config::getLimitByTurnoverPercent()
{
    const QReadLocker lock(mRwMutex);

    return mLimitByTurnoverPercent;
}

void Config::setLimitStockPurchaseNonWorkingHours(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mLimitStockPurchaseNonWorkingHours = value;
}

bool Config::isLimitStockPurchaseNonWorkingHours()
{
    const QReadLocker lock(mRwMutex);

    return mLimitStockPurchaseNonWorkingHours;
}

void Config::setLimitStockPurchasePartNonWorkingHours(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLimitStockPurchasePartNonWorkingHours = value;
}

float Config::getLimitStockPurchasePartNonWorkingHours()
{
    const QReadLocker lock(mRwMutex);

    return mLimitStockPurchasePartNonWorkingHours;
}

void Config::setLimitByTurnoverNonWorkingHours(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mLimitByTurnoverNonWorkingHours = value;
}

bool Config::isLimitByTurnoverNonWorkingHours()
{
    const QReadLocker lock(mRwMutex);

    return mLimitByTurnoverNonWorkingHours;
}

void Config::setLimitByTurnoverPercentNonWorkingHours(float value)
{
    const QWriteLocker lock(mRwMutex);

    mLimitByTurnoverPercentNonWorkingHours = value;
}

float Config::getLimitByTurnoverPercentNonWorkingHours()
{
    const QReadLocker lock(mRwMutex);

    return mLimitByTurnoverPercentNonWorkingHours;
}

void Config::setStorageMonthLimit(int value)
{
    const QWriteLocker lock(mRwMutex);

    mStorageMonthLimit = value;
}

int Config::getStorageMonthLimit()
{
    const QReadLocker lock(mRwMutex);

    return mStorageMonthLimit;
}

void Config::setHighlightGoodOperations(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mHighlightGoodOperations = value;
}

bool Config::isHighlightGoodOperations()
{
    const QReadLocker lock(mRwMutex);

    return mHighlightGoodOperations;
}

void Config::setHighlightGoodOperationsYield(float value)
{
    const QWriteLocker lock(mRwMutex);

    mHighlightGoodOperationsYield = value;
}

float Config::getHighlightGoodOperationsYield()
{
    const QReadLocker lock(mRwMutex);

    return mHighlightGoodOperationsYield;
}

void Config::setHighlightBadOperations(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mHighlightBadOperations = value;
}

bool Config::isHighlightBadOperations()
{
    const QReadLocker lock(mRwMutex);

    return mHighlightBadOperations;
}

void Config::setHighlightBadOperationsLose(float value)
{
    const QWriteLocker lock(mRwMutex);

    mHighlightBadOperationsLose = value;
}

float Config::getHighlightBadOperationsLose()
{
    const QReadLocker lock(mRwMutex);

    return mHighlightBadOperationsLose;
}

void Config::setSimulatorConfigCommon(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mSimulatorConfigCommon = value;
}

bool Config::isSimulatorConfigCommon()
{
    const QReadLocker lock(mRwMutex);

    return mSimulatorConfigCommon;
}

void Config::setAutoPilotConfigCommon(bool value)
{
    const QWriteLocker lock(mRwMutex);

    mAutoPilotConfigCommon = value;
}

bool Config::isAutoPilotConfigCommon()
{
    const QReadLocker lock(mRwMutex);

    return mAutoPilotConfigCommon;
}
