#include "src/threads/detectdividends/detectdividendsthread.h"

#include <QDebug>



DetectDividendsThread::DetectDividendsThread(IStocksStorage* stocksStorage, QObject* parent) :
    IDetectDividendsThread(parent),
    mStocksStorage(stocksStorage)
{
    qDebug() << "Create DetectDividendsThread";
}

DetectDividendsThread::~DetectDividendsThread()
{
    qDebug() << "Destroy DetectDividendsThread";
}

void DetectDividendsThread::run()
{
    /*
    qDebug() << "Running DetectDividendsThread";

    blockSignals(false);

    qInfo() << "DetectDividendsThread";

    qDebug() << "Finish DetectDividendsThread";
    */
}

void DetectDividendsThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
