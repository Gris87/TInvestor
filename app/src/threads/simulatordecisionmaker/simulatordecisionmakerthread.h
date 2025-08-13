#pragma once



#include "src/threads/simulatordecisionmaker/isimulatordecisionmakerthread.h"

#include "src/db/logs/ilogsdatabase.h"
#include "src/db/operations/ioperationsdatabase.h"
#include "src/db/portfolio/iportfoliodatabase.h"
#include "src/decisions/idecisionmaker.h"
#include "src/domain/quantityandcost/quantityandcost.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/logos/ilogosstorage.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/utils/optimizer/ioptimizer.h"
#include "src/utils/settingseditor/isettingseditor.h"



class SimulatorDecisionMakerThread : public ISimulatorDecisionMakerThread
{
    Q_OBJECT

public:
    explicit SimulatorDecisionMakerThread(
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
        IOptimizer*          optimizer,
        QObject*             parent = nullptr
    );
    ~SimulatorDecisionMakerThread() override;

    SimulatorDecisionMakerThread(const SimulatorDecisionMakerThread& another)            = delete;
    SimulatorDecisionMakerThread& operator=(const SimulatorDecisionMakerThread& another) = delete;

    void run() override;

    void reset() override;
    void terminateThread() override;

#ifdef TESTING_MODE
    void testSetLimitOperations(int limitOperations)
    {
        mLimitOperations = limitOperations;
    }

    void testSetOptimizeOperationsSize(int optimizeSize)
    {
        mOptimizeOperationsSize = optimizeSize;
    }

    void testSetLimitLogs(int limitLogs)
    {
        mLimitLogs = limitLogs;
    }

    void testSetOptimizeLogsSize(int optimizeSize)
    {
        mOptimizeLogsSize = optimizeSize;
    }
#endif

private:
    void             init();
    void             readSimulationConfig();
    void             initOperations();
    void             initLogs();
    void             initPortfolio();
    void             load();
    void             loadOperations();
    void             loadLogs();
    void             loadPortfolio();
    QList<Operation> reverseOperations(const QList<Operation>& operations);
    void             updateCostAndPart();
    void             updatePrice();
    void             optimizeOperations();
    void             optimizeLogs();

    ISettingsEditor*                 mSettingsEditor;
    IOperationsDatabase*             mOperationsDatabase;
    ILogsDatabase*                   mLogsDatabase;
    IPortfolioDatabase*              mPortfolioDatabase;
    IInstrumentsStorage*             mInstrumentsStorage;
    ILogosStorage*                   mLogosStorage;
    IUserStorage*                    mUserStorage;
    IStocksStorage*                  mStocksStorage;
    IConfig*                         mConfig;
    IDecisionMaker*                  mDecisionMaker;
    IOptimizer*                      mOptimizer;
    QList<Operation>                 mOperations;
    Portfolio                        mPortfolio;
    int                              mAmountOfOperations;
    int                              mAmountOfLogs;
    int                              mLimitOperations;
    int                              mOptimizeOperationsSize;
    int                              mLimitLogs;
    int                              mOptimizeLogsSize;
    QMap<QString, Stock*>            mStocksMap; // Instrument Id => Stock
    QuantityAndCostDoubleInstruments mInstruments;
    bool                             mResetted;
    bool                             mLoaded;
    int                              mStartMoney;
    double                           mTotalMoney;
};
