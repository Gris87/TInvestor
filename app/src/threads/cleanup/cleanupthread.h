#pragma once



#include "src/threads/cleanup/icleanupthread.h"

#include "src/db/stocks/istocksdatabase.h"
#include "src/storage/stocks/istocksstorage.h"



class CleanupThread : public ICleanupThread
{
    Q_OBJECT

public:
    explicit CleanupThread(IStocksDatabase *stocksDatabase, IStocksStorage *stocksStorage, QObject *parent = nullptr);
    ~CleanupThread();

    CleanupThread(const CleanupThread &another) = delete;
    CleanupThread& operator=(const CleanupThread &another) = delete;

    void process() override;

private:
    IStocksDatabase *mStocksDatabase;
    IStocksStorage  *mStocksStorage;
};
