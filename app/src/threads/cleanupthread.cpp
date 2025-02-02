#include "cleanupthread.h"

#include <QDebug>



CleanupThread::CleanupThread(IStocksDatabase *stocksDatabase, QObject *parent) :
    ICleanupThread(parent),
    mStocksDatabase(stocksDatabase)
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
