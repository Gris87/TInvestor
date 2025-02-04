#pragma once



#include "src/threads/irefreshthread.h"

#include "src/db/stocks/istocksdatabase.h"
#include "src/storage/istocksstorage.h"



class RefreshThread : public IRefreshThread
{
    Q_OBJECT

public:
    explicit RefreshThread(IStocksDatabase *stocksDatabase, IStocksStorage *stocksStorage, QObject *parent = nullptr);
    ~RefreshThread();

    RefreshThread(const RefreshThread &another) = delete;
    RefreshThread& operator=(const RefreshThread &another) = delete;

    void process() override;

private:
    IStocksDatabase *mStocksDatabase;
    IStocksStorage  *mStocksStorage;
};
