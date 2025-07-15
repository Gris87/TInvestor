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
    void simulateSell(const QString& instrumentId);
    void simulateBuy(const QString& instrumentId, const TradingInfo& tradingInfo);
    void updateCostAndPart();

    IOperationsDatabase*                 mOperationsDatabase;
    ILogsDatabase*                       mLogsDatabase;
    IPortfolioDatabase*                  mPortfolioDatabase;
    IInstrumentsStorage*                 mInstrumentsStorage;
    ILogosStorage*                       mLogosStorage;
    IUserStorage*                        mUserStorage;
    IStocksStorage*                      mStocksStorage;
    IDecisionMaker*                      mDecisionMaker;
    Portfolio                            mPortfolio;
    QMap<QString, QuantityAndCostDouble> mInstruments; // Instrument Id => QuantityAndCostDouble
    bool                                 mResetted;
    bool                                 mLoaded;
    int                                  mStartMoney;
};
