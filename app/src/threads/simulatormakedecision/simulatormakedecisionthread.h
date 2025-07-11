#pragma once



#include "src/threads/simulatormakedecision/isimulatormakedecisionthread.h"

#include <QMutex>

#include "src/config/iconfig.h"
#include "src/storage/stocks/istocksstorage.h"



class SimulatorMakeDecisionThread : public ISimulatorMakeDecisionThread
{
    Q_OBJECT

public:
    explicit SimulatorMakeDecisionThread(IConfig* config, IStocksStorage* stocksStorage, QObject* parent = nullptr);
    ~SimulatorMakeDecisionThread() override;

    SimulatorMakeDecisionThread(const SimulatorMakeDecisionThread& another)            = delete;
    SimulatorMakeDecisionThread& operator=(const SimulatorMakeDecisionThread& another) = delete;

    void run() override;

    void terminateThread() override;

private:
    QMutex*         mMutex;
    IConfig*        mConfig;
    IStocksStorage* mStocksStorage;
    QString         mAccountId;
    int             mKeepMoney;
};
