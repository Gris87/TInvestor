#pragma once



#include "src/threads/makedecision/imakedecisionthread.h"

#include "src/config/iconfig.h"
#include "src/storage/stocks/istocksstorage.h"



class MakeDecisionThread : public IMakeDecisionThread
{
    Q_OBJECT

public:
    explicit MakeDecisionThread(IConfig* config, IStocksStorage* stocksStorage, QObject* parent = nullptr);
    ~MakeDecisionThread() override;

    MakeDecisionThread(const MakeDecisionThread& another)            = delete;
    MakeDecisionThread& operator=(const MakeDecisionThread& another) = delete;

    void run() override;

private:
    IConfig*        mConfig;
    IStocksStorage* mStocksStorage;
};
