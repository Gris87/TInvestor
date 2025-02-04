#include "refreshthread.h"

#include <QDebug>



RefreshThread::RefreshThread(IStocksDatabase *stocksDatabase, IStocksStorage *stocksStorage, QObject *parent) :
    IRefreshThread(parent),
    mStocksDatabase(stocksDatabase),
    mStocksStorage(stocksStorage)
{
    qDebug() << "Create RefreshThread";
}

RefreshThread::~RefreshThread()
{
    qDebug() << "Destroy RefreshThread";
}

void RefreshThread::process()
{
    qDebug() << "Running RefreshThread";

    qDebug() << "Finish RefreshThread";
}
