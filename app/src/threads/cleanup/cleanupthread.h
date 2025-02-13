#pragma once



#include "src/threads/cleanup/icleanupthread.h"

#include "src/config/iconfig.h"
#include "src/storage/stocks/istocksstorage.h"



class CleanupThread : public ICleanupThread
{
    Q_OBJECT

public:
    explicit CleanupThread(
        IConfig* config, IStocksDatabase* stocksDatabase, IStocksStorage* stocksStorage, QObject* parent = nullptr
    );
    ~CleanupThread();

    CleanupThread(const CleanupThread& another)            = delete;
    CleanupThread& operator=(const CleanupThread& another) = delete;

    void run() override;

private:
    IConfig*         mConfig;
    IStocksDatabase* mStocksDatabase;
    IStocksStorage*  mStocksStorage;
};
