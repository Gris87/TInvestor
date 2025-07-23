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
        IDecisionMaker*      decisionMaker,
        QObject*             parent = nullptr
    );
    ~SimulatorDecisionMakerThread() override;

    SimulatorDecisionMakerThread(const SimulatorDecisionMakerThread& another)            = delete;
    SimulatorDecisionMakerThread& operator=(const SimulatorDecisionMakerThread& another) = delete;

    void run() override;

    void reset() override;
    void terminateThread() override;

private:
    void init();
    void readSimulationConfig();
    void initOperations();
    void initLogs();
    void initPortfolio();
    void load();
    void loadOperations();
    void loadLogs();
    void loadPortfolio();
    void simulateTrading(const InstrumentsForTrading& instrumentsForTrading);
    void simulateSell(
        const QString& instrumentId, const TradingInfo& tradingInfo, QList<Operation>& operations, QList<LogEntry>& entries
    );
    void simulateSellForOperations(
        QList<Operation>& operations,
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
        QList<LogEntry>&  entries,
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        const QString&    cause,
        qint64            quantity,
        float             price
    );
    void simulateSellForPortfolio(const QString& instrumentId, double cost, double totalCommission);
    void simulateSellForInstruments(const QString& instrumentId);
    void simulateBuy(
        const QString& instrumentId, const TradingInfo& tradingInfo, QList<Operation>& operations, QList<LogEntry>& entries
    );
    void simulateBuyForOperations(
        QList<Operation>& operations,
        const QString&    instrumentId,
        Logo*             logo,
        const Instrument& instrument,
        qint64            quantity,
        float             price,
        double            cost,
        double            totalCommission
    );
    void simulateBuyForLogs(
        QList<LogEntry>&  entries,
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
    void simulateBuyForInstruments(const QString& instrumentId, qint64 quantity, double cost);
    void updateCostAndPart();
    void updatePrice();

    ISettingsEditor*                     mSettingsEditor;
    IOperationsDatabase*                 mOperationsDatabase;
    ILogsDatabase*                       mLogsDatabase;
    IPortfolioDatabase*                  mPortfolioDatabase;
    IInstrumentsStorage*                 mInstrumentsStorage;
    ILogosStorage*                       mLogosStorage;
    IUserStorage*                        mUserStorage;
    IStocksStorage*                      mStocksStorage;
    IDecisionMaker*                      mDecisionMaker;
    Portfolio                            mPortfolio;
    QMap<QString, Stock*>                mStocksMap;   // Instrument Id => Stock
    QMap<QString, QuantityAndCostDouble> mInstruments; // Instrument Id => QuantityAndCostDouble
    bool                                 mResetted;
    bool                                 mLoaded;
    int                                  mStartMoney;
    double                               mTotalMoney;
};
