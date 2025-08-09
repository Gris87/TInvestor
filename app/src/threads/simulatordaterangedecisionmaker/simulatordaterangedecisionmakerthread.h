#pragma once



#include "src/threads/simulatordaterangedecisionmaker/isimulatordaterangedecisionmakerthread.h"

#include <QMutex>

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

    QString simulationWithBestConfigForBuyDecision(
        QThread* parentThread,
        qint64   startTime,
        int      buyDecisionId,
        IConfig* config,
        int*     configIdArray,
        int*     amountOfConfigsArray,
        double*  processedMinutesArray,
        double*  remainingMinutesArray,
        qint64*  currentMinuteArray,
        double*  bestTotalMoneyArray
    );
    QString simulationWithBestConfigParallelEnter(
        QThread*             parentThread,
        int                  threadId,
        int                  threadsCount,
        QString*             configVariants,
        int                  amountOfConfigs,
        qint64               startTime,
        IConfig*             config,
        int                  buyDecisionId,
        const QString&       settingsSuffix,
        int                  startConfigId,
        int&                 lastConfigId,
        QAtomicInt&          currentConfigId,
        QAtomicInt&          processedConfigId,
        qint64               totalMinutes,
        const QList<Stock*>& stocks,
        qint64*              currentMinuteArray,
        QMutex*              mutex,
        QList<int>*          processedIds,
        double*              bestGlobalTotalMoney,
        double*              bestLocalTotalMoney,
        QList<Operation>*    bestOperations,
        QList<LogEntry>*     bestEntries,
        Portfolio*           bestPortfolio
    );
    void simulationWithBestConfigForParallel(
        QThread*             parentThread,
        int                  threadId,
        int                  threadsCount,
        qint64               startTime,
        IConfig*             config,
        int                  buyDecisionId,
        int                  startConfigId,
        QAtomicInt&          processedConfigId,
        int                  amountOfConfigs,
        qint64               totalMinutes,
        const QList<Stock*>& stocks,
        double&              totalMoney,
        QList<Operation>&    operations,
        QList<LogEntry>&     entries,
        Portfolio&           portfolio,
        qint64*              currentMinuteArray
    );

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
    void simulationWithBestConfig();
    void simulationWithBestConfigStep1(double& bestGlobalTotalMoney, qint64 totalMinutes, const QList<Stock*>& stocks);
    void simulationWithBestConfigStep2(double& bestGlobalTotalMoney, qint64 totalMinutes, const QList<Stock*>& stocks);
    void simulationWithoutBestConfig();
    void simulateTrading(
        qint64                            timestamp,
        const InstrumentsForTrading&      instrumentsForTrading,
        double&                           totalMoney,
        QList<Operation>&                 operations,
        QList<LogEntry>&                  entries,
        Portfolio&                        portfolio,
        QuantityAndCostDoubleInstruments& instruments
    ) const;
    void simulateSell(
        qint64&                           timestamp,
        const QString&                    instrumentId,
        const TradingInfo&                tradingInfo,
        double&                           totalMoney,
        QList<Operation>&                 operations,
        QList<LogEntry>&                  entries,
        Portfolio&                        portfolio,
        QuantityAndCostDoubleInstruments& instruments
    ) const;
    void simulateSellForOperations(
        qint64&           timestamp,
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        qint64            quantity,
        double            costFifo,
        float             price,
        double            cost,
        double            totalCommission,
        double&           totalMoney,
        QList<Operation>& operations,
        Portfolio&        portfolio
    ) const;
    void simulateSellForLogs(
        qint64&           timestamp,
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        const QString&    cause,
        qint64            quantity,
        float             price,
        QList<LogEntry>&  entries
    ) const;
    void simulateSellForPortfolio(const QString& instrumentId, double cost, double totalCommission, Portfolio& portfolio) const;
    void simulateSellForInstruments(const QString& instrumentId, QuantityAndCostDoubleInstruments& instruments) const;
    void simulateBuy(
        qint64&                           timestamp,
        const QString&                    instrumentId,
        const TradingInfo&                tradingInfo,
        double&                           totalMoney,
        QList<Operation>&                 operations,
        QList<LogEntry>&                  entries,
        Portfolio&                        portfolio,
        QuantityAndCostDoubleInstruments& instruments
    ) const;
    void simulateBuyForOperations(
        qint64&           timestamp,
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        qint64            quantity,
        float             price,
        double            cost,
        double            totalCommission,
        double&           totalMoney,
        QList<Operation>& operations,
        Portfolio&        portfolio
    ) const;
    void simulateBuyForLogs(
        qint64&           timestamp,
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        const QString&    cause,
        qint64            quantity,
        float             price,
        QList<LogEntry>&  entries
    ) const;
    void simulateBuyForPortfolio(
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        qint64            quantity,
        float             price,
        double            cost,
        double            totalCommission,
        Portfolio&        portfolio
    ) const;
    void simulateBuyForInstruments(
        const QString& instrumentId, qint64 quantity, double cost, QuantityAndCostDoubleInstruments& instruments
    ) const;

    [[nodiscard]]
    QList<Operation> reverseOperations(const QList<Operation>& operations) const;

    [[nodiscard]]
    QList<LogEntry> reverseEntries(const QList<LogEntry>& entries) const;

    void updateCostAndPart();
    void updatePrice();
    void notifyProgressChanged(
        qint64  startTime,
        int     buyDecisionId,
        int     startConfigId,
        int     processedConfigId,
        int     amountOfConfigs,
        qint64* currentMinuteArray,
        int     threadId,
        int     threadsCount,
        qint64  currentMinute,
        qint64  totalMinutes
    );
    void        notifyBestResult(double bestTotalMoney);
    void        optimizeOperations();
    void        optimizeLogs();
    QStringList splitConfigVariants(const QString& configVariants);
    void        applyToConfig(IConfig* config, const QString& configVariant);

    IDirFactory*         mDirFactory;
    IFileFactory*        mFileFactory;
    ISettingsEditor*     mSettingsEditor;
    IOperationsDatabase* mOperationsDatabase;
    ILogsDatabase*       mLogsDatabase;
    IPortfolioDatabase*  mPortfolioDatabase;
    IInstrumentsStorage* mInstrumentsStorage;
    ILogosStorage*       mLogosStorage;
    IUserStorage*        mUserStorage;
    IStocksStorage*      mStocksStorage;
    IConfig*             mConfig;
    IDecisionMaker*      mDecisionMaker;
    IOptimizer*          mOptimizer;
    QList<Operation>     mInitOperations;
    QList<LogEntry>      mInitEntries;
    Portfolio            mInitPortfolio;
    QList<Operation>     mBestOperations;
    QList<LogEntry>      mBestEntries;
    Portfolio            mBestPortfolio;
    int                  mLimitOperations;
    int                  mOptimizeOperationsSize;
    int                  mLimitLogs;
    int                  mOptimizeLogsSize;
    bool                 mResetted;
    int                  mStartMoney;
    qint64               mStartTimestamp;
    qint64               mEndTimestamp;
    bool                 mBestConfig;
    QStringList          mConfigVariants;
};
