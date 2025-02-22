#include "src/threads/pricecollect/pricecollectthread.h"

#include <QDebug>
#include <QMutexLocker>



PriceCollectThread::PriceCollectThread(IGrpcClient* grpcClient, QObject* parent) :
    IPriceCollectThread(parent),
    mGrpcClient(grpcClient)
{
    qDebug() << "Create PriceCollectThread";
}

PriceCollectThread::~PriceCollectThread()
{
    qDebug() << "Destroy PriceCollectThread";
}

void PriceCollectThread::run()
{
    qDebug() << "Running PriceCollectThread";

    qDebug() << "Finish PriceCollectThread";
}
