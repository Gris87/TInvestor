#include "src/threads/lastprice/lastpricethread.h"

#include <QDebug>
#include <QMutexLocker>



LastPriceThread::LastPriceThread(IStocksStorage* stocksStorage, IGrpcClient* grpcClient, QObject* parent) :
    ILastPriceThread(parent),
    mStocksStorage(stocksStorage),
    mGrpcClient(grpcClient)
{
    qDebug() << "Create LastPriceThread";
}

LastPriceThread::~LastPriceThread()
{
    qDebug() << "Destroy LastPriceThread";
}

void LastPriceThread::run()
{
    qDebug() << "Running LastPriceThread";

    qDebug() << "Finish LastPriceThread";
}
