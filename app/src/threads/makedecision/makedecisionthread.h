#pragma once



#include "src/threads/makedecision/imakedecisionthread.h"

#include "src/config/iconfig.h"
#include "src/db/stocks/istocksdatabase.h"
#include "src/storage/stocks/istocksstorage.h"



class MakeDecisionThread : public IMakeDecisionThread
{
    Q_OBJECT

public:
    explicit MakeDecisionThread(
        IConfig *config,
        IStocksDatabase *stocksDatabase,
        IStocksStorage *stocksStorage,
        QObject *parent = nullptr
    );
    ~MakeDecisionThread();

    MakeDecisionThread(const MakeDecisionThread &another) = delete;
    MakeDecisionThread& operator=(const MakeDecisionThread &another) = delete;

    void run() override;

private:
    IConfig         *mConfig;
    IStocksDatabase *mStocksDatabase;
    IStocksStorage  *mStocksStorage;
};
