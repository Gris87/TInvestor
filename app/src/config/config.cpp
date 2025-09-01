#include "src/config/config.h"

#include <QDebug>



const char* const CPU_USAGE_DEFAULT = "MAXIMUM";

constexpr bool  AUTORUN_DEFAULT                    = true;
constexpr int   MAKE_DECISION_TIMEOUT_DEFAULT      = 1;
constexpr bool  TRADE_IN_NON_WORKING_HOURS_DEFAULT = true;
constexpr bool  LIMIT_STOCK_PURCHASE_DEFAULT       = true;
constexpr float LIMIT_STOCK_PURCHASE_PART_DEFAULT  = 10.0f;
constexpr bool  LIMIT_BY_TURNOVER_DEFAULT          = true;
constexpr float LIMIT_BY_TURNOVER_PERCENT_DEFAULT  = 5.0f;
constexpr int   STORAGE_MONTH_LIMIT_DEFAULT        = 12;
constexpr bool  SIMULATOR_CONFIG_COMMON_DEFAULT    = true;
constexpr bool  AUTOPILOT_CONFIG_COMMON_DEFAULT    = false;



Config::Config(IDecisionMakerConfig* simulatorConfig, IDecisionMakerConfig* autoPilotConfig) :
    IConfig(),
    mRwMutex(new QReadWriteLock()),
    mSimulatorConfig(simulatorConfig),
    mAutoPilotConfig(autoPilotConfig),
    mAutorun(),
    mCpuUsage(),
    mMakeDecisionTimeout(),
    mTradeInNonWorkingHours(),
    mLimitStockPurchase(),
    mLimitStockPurchasePart(),
    mLimitByTurnover(),
    mLimitByTurnoverPercent(),
    mStorageMonthLimit(),
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

    mAutorun                = config.mAutorun;
    mCpuUsage               = config.mCpuUsage;
    mMakeDecisionTimeout    = config.mMakeDecisionTimeout;
    mTradeInNonWorkingHours = config.mTradeInNonWorkingHours;
    mLimitStockPurchase     = config.mLimitStockPurchase;
    mLimitStockPurchasePart = config.mLimitStockPurchasePart;
    mLimitByTurnover        = config.mLimitByTurnover;
    mLimitByTurnoverPercent = config.mLimitByTurnoverPercent;
    mStorageMonthLimit      = config.mStorageMonthLimit;
    mSimulatorConfigCommon  = config.mSimulatorConfigCommon;
    mAutoPilotConfigCommon  = config.mAutoPilotConfigCommon;
}

void Config::makeDefault()
{
    const QWriteLocker lock(mRwMutex);

    qInfo() << "Set Config to default";

    mSimulatorConfig->makeDefault();
    mAutoPilotConfig->makeDefault();

    mAutorun                = AUTORUN_DEFAULT;
    mCpuUsage               = CPU_USAGE_DEFAULT;
    mMakeDecisionTimeout    = MAKE_DECISION_TIMEOUT_DEFAULT;
    mTradeInNonWorkingHours = TRADE_IN_NON_WORKING_HOURS_DEFAULT;
    mLimitStockPurchase     = LIMIT_STOCK_PURCHASE_DEFAULT;
    mLimitStockPurchasePart = LIMIT_STOCK_PURCHASE_PART_DEFAULT;
    mLimitByTurnover        = LIMIT_BY_TURNOVER_DEFAULT;
    mLimitByTurnoverPercent = LIMIT_BY_TURNOVER_PERCENT_DEFAULT;
    mStorageMonthLimit      = STORAGE_MONTH_LIMIT_DEFAULT;
    mSimulatorConfigCommon  = SIMULATOR_CONFIG_COMMON_DEFAULT;
    mAutoPilotConfigCommon  = AUTOPILOT_CONFIG_COMMON_DEFAULT;
}

void Config::save(ISettingsEditor* settingsEditor)
{
    const QReadLocker lock(mRwMutex);

    qInfo() << "Save Config";

    mSimulatorConfig->save(settingsEditor, "Config/Simulator");
    mAutoPilotConfig->save(settingsEditor, "Config/AutoPilot");

    // clang-format off
    settingsEditor->setValue("Config/Autorun",                mAutorun);
    settingsEditor->setValue("Config/CpuUsage",               mCpuUsage);
    settingsEditor->setValue("Config/MakeDecisionTimeout",    mMakeDecisionTimeout);
    settingsEditor->setValue("Config/TradeInNonWorkingHours", mTradeInNonWorkingHours);
    settingsEditor->setValue("Config/LimitStockPurchase",     mLimitStockPurchase);
    settingsEditor->setValue("Config/LimitStockPurchasePart", mLimitStockPurchasePart);
    settingsEditor->setValue("Config/LimitByTurnover",        mLimitByTurnover);
    settingsEditor->setValue("Config/LimitByTurnoverPercent", mLimitByTurnoverPercent);
    settingsEditor->setValue("Config/StorageMonthLimit",      mStorageMonthLimit);
    settingsEditor->setValue("Config/SimulatorConfigCommon",  mSimulatorConfigCommon);
    settingsEditor->setValue("Config/AutoPilotConfigCommon",  mAutoPilotConfigCommon);
    // clang-format on
}

void Config::load(ISettingsEditor* settingsEditor)
{
    const QWriteLocker lock(mRwMutex);

    qInfo() << "Load Config";

    mSimulatorConfig->load(settingsEditor, "Config/Simulator");
    mAutoPilotConfig->load(settingsEditor, "Config/AutoPilot");

    // clang-format off
    mAutorun                   = settingsEditor->value("Config/Autorun",                mAutorun).toBool();
    mCpuUsage                  = settingsEditor->value("Config/CpuUsage",               mCpuUsage).toString();
    mMakeDecisionTimeout       = settingsEditor->value("Config/MakeDecisionTimeout",    mMakeDecisionTimeout).toInt();
    mTradeInNonWorkingHours    = settingsEditor->value("Config/TradeInNonWorkingHours", mTradeInNonWorkingHours).toBool();
    mLimitStockPurchase        = settingsEditor->value("Config/LimitStockPurchase",     mLimitStockPurchase).toBool();
    mLimitStockPurchasePart    = settingsEditor->value("Config/LimitStockPurchasePart", mLimitStockPurchasePart).toFloat();
    mLimitByTurnover           = settingsEditor->value("Config/LimitByTurnover",        mLimitByTurnover).toBool();
    mLimitByTurnoverPercent    = settingsEditor->value("Config/LimitByTurnoverPercent", mLimitByTurnoverPercent).toFloat();
    mStorageMonthLimit         = settingsEditor->value("Config/StorageMonthLimit",      mStorageMonthLimit).toInt();
    mSimulatorConfigCommon     = settingsEditor->value("Config/SimulatorConfigCommon",  mSimulatorConfigCommon).toBool();
    mAutoPilotConfigCommon     = settingsEditor->value("Config/AutoPilotConfigCommon",  mAutoPilotConfigCommon).toBool();
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
