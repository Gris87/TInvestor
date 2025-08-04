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
#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"
#include "src/utils/optimizer/ioptimizer.h"
#include "src/utils/settingseditor/isettingseditor.h"



class SimulatorDateRangeDecisionMakerThread : public ISimulatorDateRangeDecisionMakerThread
{
    Q_OBJECT

public:
    explicit SimulatorDateRangeDecisionMakerThread(
        IDirFactory*         dirFactory,
        IFileFactory*        fileFactory,
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
    ~SimulatorDateRangeDecisionMakerThread() override;

    SimulatorDateRangeDecisionMakerThread(const SimulatorDateRangeDecisionMakerThread& another)            = delete;
    SimulatorDateRangeDecisionMakerThread& operator=(const SimulatorDateRangeDecisionMakerThread& another) = delete;

    void run() override;

    void reset() override;
    void terminateThread() override;

    void simulationWithBestConfigForBuyDecision(qint64 startTime, int buyDecisionId, IConfig* config);

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
    void init();
    void readSimulationConfig();
    void initOperations();
    void initLogs();
    void initPortfolio();
    void initConfigs();
    void load();
    void loadBestOperations();
    void loadBestLogs();
    void loadBestPortfolio();
    void loadConfigs();
    void simulationWithBestConfig(qint64 startTime);
    void simulationWithoutBestConfig(qint64 startTime);
    void simulateTrading(qint64 timestamp, const InstrumentsForTrading& instrumentsForTrading);
    void simulateSell(qint64& timestamp, const QString& instrumentId, const TradingInfo& tradingInfo);
    void simulateSellForOperations(
        qint64&           timestamp,
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        qint64            quantity,
        double            costFifo,
        float             price,
        double            cost,
        double            totalCommission
    );
    void simulateSellForLogs(
        qint64&           timestamp,
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        const QString&    cause,
        qint64            quantity,
        float             price
    );
    void simulateSellForPortfolio(const QString& instrumentId, double cost, double totalCommission);
    void simulateSellForInstruments(const QString& instrumentId);
    void simulateBuy(qint64& timestamp, const QString& instrumentId, const TradingInfo& tradingInfo);
    void simulateBuyForOperations(
        qint64&           timestamp,
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        qint64            quantity,
        float             price,
        double            cost,
        double            totalCommission
    );
    void simulateBuyForLogs(
        qint64&           timestamp,
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        const QString&    cause,
        qint64            quantity,
        float             price
    );
    void simulateBuyForPortfolio(
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        qint64            quantity,
        float             price,
        double            cost,
        double            totalCommission
    );
    void             simulateBuyForInstruments(const QString& instrumentId, qint64 quantity, double cost);
    QList<Operation> reverseOperations();
    QList<LogEntry>  reverseEntries();
    void             updateCostAndPart();
    void             updatePrice();
    void             notifyBestResult();
    void             optimizeOperations();
    void             optimizeLogs();

    IDirFactory*                     mDirFactory;
    IFileFactory*                    mFileFactory;
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
    QList<Operation>                 mInitOperations;
    QList<LogEntry>                  mInitEntries;
    Portfolio                        mInitPortfolio;
    QList<Operation>                 mOperations;
    QList<LogEntry>                  mEntries;
    Portfolio                        mPortfolio;
    QList<Operation>                 mBestOperations;
    QList<LogEntry>                  mBestEntries;
    Portfolio                        mBestPortfolio;
    int                              mLimitOperations;
    int                              mOptimizeOperationsSize;
    int                              mLimitLogs;
    int                              mOptimizeLogsSize;
    QuantityAndCostDoubleInstruments mInstruments;
    bool                             mResetted;
    int                              mStartMoney;
    qint64                           mStartTimestamp;
    qint64                           mEndTimestamp;
    bool                             mBestConfig;
    QStringList                      mConfigVariants;
    double                           mTotalMoney;
    double                           mBestTotalMoney;
};
