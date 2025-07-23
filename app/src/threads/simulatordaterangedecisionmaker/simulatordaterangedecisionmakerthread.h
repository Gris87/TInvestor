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
    void initOperations();
    void initLogs();
    void initPortfolio();
    void initConfigs();
    void load();
    void loadBestOperations();
    void loadBestLogs();
    void loadBestPortfolio();
    void loadConfigs();
    void simulateTrading(const InstrumentsForTrading& instrumentsForTrading);
    void simulateSell(const QString& instrumentId, const TradingInfo& tradingInfo);
    void simulateSellForOperations(
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
        const QString& instrumentId, Logo* logo, const Instrument& instrument, const QString& cause, qint64 quantity, float price
    );
    void simulateSellForPortfolio(const QString& instrumentId, double cost, double totalCommission);
    void simulateSellForInstruments(const QString& instrumentId);
    void simulateBuy(const QString& instrumentId, const TradingInfo& tradingInfo);
    void simulateBuyForOperations(
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        qint64            quantity,
        float             price,
        double            cost,
        double            totalCommission
    );
    void simulateBuyForLogs(
        const QString& instrumentId, Logo* logo, const Instrument& instrument, const QString& cause, qint64 quantity, float price
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
    void simulateBuyForInstruments(const QString& instrumentId, qint64 quantity, double cost);
    void notifyBestResult();

    IDirFactory*                         mDirFactory;
    IFileFactory*                        mFileFactory;
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
    QList<Operation>                     mInitOperations;
    QList<LogEntry>                      mInitEntries;
    Portfolio                            mInitPortfolio;
    QList<Operation>                     mOperations;
    QList<LogEntry>                      mEntries;
    Portfolio                            mPortfolio;
    QList<Operation>                     mBestOperations;
    QList<LogEntry>                      mBestEntries;
    Portfolio                            mBestPortfolio;
    QMap<QString, QuantityAndCostDouble> mInstruments; // Instrument Id => QuantityAndCostDouble
    bool                                 mResetted;
    int                                  mStartMoney;
    qint64                               mStartTimestamp;
    qint64                               mEndTimestamp;
    bool                                 mBestConfig;
    QString                              mConfigVariants;
    double                               mTotalMoney;
    double                               mBestTotalMoney;
};
