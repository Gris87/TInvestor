#pragma once



#include "src/threads/simulatormakedecision/isimulatormakedecisionthread.h"

#include "src/decisions/idecisionmaker.h"
#include "src/storage/stocks/istocksstorage.h"



class SimulatorMakeDecisionThread : public ISimulatorMakeDecisionThread
{
    Q_OBJECT

public:
    explicit SimulatorMakeDecisionThread(IStocksStorage* stocksStorage, IDecisionMaker* decisionMaker, QObject* parent = nullptr);
    ~SimulatorMakeDecisionThread() override;

    SimulatorMakeDecisionThread(const SimulatorMakeDecisionThread& another)            = delete;
    SimulatorMakeDecisionThread& operator=(const SimulatorMakeDecisionThread& another) = delete;

    void run() override;

    void reset() override;
    void setStartMoney(int value) override;
    void terminateThread() override;

private:
    void init();
    void load();
    void simulateTrading(const InstrumentsForTrading& instrumentsForTrading);

    IStocksStorage* mStocksStorage;
    IDecisionMaker* mDecisionMaker;
    Portfolio       mPortfolio;
    bool            mResetted;
    bool            mLoaded;
    int             mStartMoney;
};
