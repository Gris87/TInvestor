#pragma once



#include "src/threads/simulatordaterangedecisionmaker/isimulatordaterangedecisionmakerthread.h"

#include "src/config/iconfig.h"
#include "src/db/logs/ilogsdatabase.h"
#include "src/db/operations/ioperationsdatabase.h"
#include "src/db/portfolio/iportfoliodatabase.h"
#include "src/decisions/idecisionmaker.h"
#include "src/domain/quantityandcost/quantityandcost.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/logos/ilogosstorage.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/utils/settingseditor/isettingseditor.h"



class SimulatorDateRangeDecisionMakerThread : public ISimulatorDateRangeDecisionMakerThread
{
    Q_OBJECT

public:
    explicit SimulatorDateRangeDecisionMakerThread(
        ISettingsEditor*     settingsEditor,
        IOperationsDatabase* operationsDatabase,
        ILogsDatabase*       logsDatabase,
        IPortfolioDatabase*  portfolioDatabase,
        IInstrumentsStorage* instrumentsStorage,
        ILogosStorage*       logosStorage,
        IUserStorage*        userStorage,
        IStocksStorage*      stocksStorage,
        IConfig*             config,
        IDecisionMaker*      decisionMaker,
        QObject*             parent = nullptr
    );
    ~SimulatorDateRangeDecisionMakerThread() override;

    SimulatorDateRangeDecisionMakerThread(const SimulatorDateRangeDecisionMakerThread& another)            = delete;
    SimulatorDateRangeDecisionMakerThread& operator=(const SimulatorDateRangeDecisionMakerThread& another) = delete;

    void run() override;

    void reset() override;
    void terminateThread() override;

private:
    void init();
    void readSimulationConfig();
    void initConfigs();
    void load();
    void loadConfigs();

    ISettingsEditor*                     mSettingsEditor;
    IOperationsDatabase*                 mOperationsDatabase;
    ILogsDatabase*                       mLogsDatabase;
    IPortfolioDatabase*                  mPortfolioDatabase;
    IInstrumentsStorage*                 mInstrumentsStorage;
    ILogosStorage*                       mLogosStorage;
    IUserStorage*                        mUserStorage;
    IStocksStorage*                      mStocksStorage;
    IConfig*                             mConfig;
    IDecisionMaker*                      mDecisionMaker;
    Portfolio                            mPortfolio;
    QMap<QString, QuantityAndCostDouble> mInstruments; // Instrument Id => QuantityAndCostDouble
    bool                                 mResetted;
    int                                  mStartMoney;
    qint64                               mStartTimestamp;
    qint64                               mEndTimestamp;
    bool                                 mBestConfig;
    double                               mTotalYieldWithCommission;
    double                               mTotalMoney;
};
