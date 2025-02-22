#include "src/threads/lastprice/lastpricethread.h"

#include <QDebug>
#include <QMutexLocker>



LastPriceThread::LastPriceThread(IGrpcClient* grpcClient, QObject* parent) :
    ILastPriceThread(parent),
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
