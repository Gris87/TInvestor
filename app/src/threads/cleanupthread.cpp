#include "cleanupthread.h"

#include <QDebug>



CleanupThread::CleanupThread(IStocksDatabase *stocksDatabase, IStocksStorage *stocksStorage, QObject *parent) :
    ICleanupThread(parent),
    mStocksDatabase(stocksDatabase),
    mStocksStorage(stocksStorage)
{
    qDebug() << "Create CleanupThread";
}

CleanupThread::~CleanupThread()
{
    qDebug() << "Destroy CleanupThread";
}

void CleanupThread::process()
{
    qDebug() << "Running CleanupThread";

    qDebug() << "Finish CleanupThread";
}
