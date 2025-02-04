#include "cleanupthread.h"

#include <QDateTime>
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

    qint64 startTime = QDateTime::currentMSecsSinceEpoch(); // TODO: Remove it

    QList<Stock> *stocks = mStocksStorage->getStocks();
    mStocksDatabase->readStocksData(stocks);

    qInfo() << "AAAAAAAAA" << QDateTime::currentMSecsSinceEpoch() - startTime; // TODO: Remove it

    qDebug() << "Finish CleanupThread";
}
