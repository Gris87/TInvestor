#pragma once



#include "src/threads/icleanupthread.h"

#include "src/db/stocks/istocksdatabase.h"



class CleanupThread : public ICleanupThread
{
    Q_OBJECT

public:
    explicit CleanupThread(IStocksDatabase *stocksDatabase, QObject *parent = nullptr);
    ~CleanupThread();

    CleanupThread(const CleanupThread &another) = delete;
    CleanupThread& operator=(const CleanupThread &another) = delete;

    void process() override;

private:
    IStocksDatabase *mStocksDatabase;
};
