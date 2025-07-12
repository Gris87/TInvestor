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

    void terminateThread() override;

private:
    IStocksStorage* mStocksStorage;
    IDecisionMaker* mDecisionMaker;
};
