#include "src/threads/detectshorts/detectshortsthread.h"

#include <QDebug>



DetectShortsThread::DetectShortsThread(IStocksStorage* stocksStorage, QObject* parent) :
    IDetectShortsThread(parent),
    mStocksStorage(stocksStorage)
{
    qDebug() << "Create DetectShortsThread";
}

DetectShortsThread::~DetectShortsThread()
{
    qDebug() << "Destroy DetectShortsThread";
}

void DetectShortsThread::run()
{
    qDebug() << "Running DetectShortsThread";

    blockSignals(false);

    qDebug() << "Finish DetectShortsThread";
}

void DetectShortsThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
