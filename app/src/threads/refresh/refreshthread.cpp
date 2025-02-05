#include "refreshthread.h"

#include <QDebug>



RefreshThread::RefreshThread(
    IConfig *config,
    IStocksDatabase *stocksDatabase,
    IStocksStorage *stocksStorage,
    QObject *parent
) :
    IRefreshThread(parent),
    mConfig(config),
    mStocksDatabase(stocksDatabase),
    mStocksStorage(stocksStorage)
{
    qDebug() << "Create RefreshThread";
}

RefreshThread::~RefreshThread()
{
    qDebug() << "Destroy RefreshThread";
}

void RefreshThread::run()
{
    qDebug() << "Running RefreshThread";

    qDebug() << "Finish RefreshThread";
}
