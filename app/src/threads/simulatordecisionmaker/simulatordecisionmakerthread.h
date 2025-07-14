#pragma once



#include "src/threads/simulatordecisionmaker/isimulatordecisionmakerthread.h"

#include "src/db/logs/ilogsdatabase.h"
#include "src/db/operations/ioperationsdatabase.h"
#include "src/db/portfolio/iportfoliodatabase.h"
#include "src/decisions/idecisionmaker.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/logos/ilogosstorage.h"
#include "src/storage/stocks/istocksstorage.h"



class SimulatorDecisionMakerThread : public ISimulatorDecisionMakerThread
{
    Q_OBJECT

public:
    explicit SimulatorDecisionMakerThread(
        IOperationsDatabase* operationsDatabase,
        ILogsDatabase*       logsDatabase,
        IPortfolioDatabase*  portfolioDatabase,
        IInstrumentsStorage* instrumentsStorage,
        ILogosStorage*       logosStorage,
        IStocksStorage*      stocksStorage,
        IDecisionMaker*      decisionMaker,
        QObject*             parent = nullptr
    );
    ~SimulatorDecisionMakerThread() override;

    SimulatorDecisionMakerThread(const SimulatorDecisionMakerThread& another)            = delete;
    SimulatorDecisionMakerThread& operator=(const SimulatorDecisionMakerThread& another) = delete;

    void run() override;

    void reset() override;
    void setStartMoney(int value) override;
    void terminateThread() override;

private:
    void init();
    void initOperations();
    void initLogs();
    void initPortfolio();
    void load();
    void loadOperations();
    void loadLogs();
    void loadPortfolio();
    void simulateTrading(const InstrumentsForTrading& instrumentsForTrading);

    IOperationsDatabase* mOperationsDatabase;
    ILogsDatabase*       mLogsDatabase;
    IPortfolioDatabase*  mPortfolioDatabase;
    IInstrumentsStorage* mInstrumentsStorage;
    ILogosStorage*       mLogosStorage;
    IStocksStorage*      mStocksStorage;
    IDecisionMaker*      mDecisionMaker;
    Portfolio            mPortfolio;
    bool                 mResetted;
    bool                 mLoaded;
    int                  mStartMoney;
};
