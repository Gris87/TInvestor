#include "src/threads/marketwaves/marketwavesthread.h"

#include <QDebug>



MarketWavesThread::MarketWavesThread(QObject* parent) :
    IMarketWavesThread(parent)
{
    qDebug() << "Create MarketWavesThread";
}

MarketWavesThread::~MarketWavesThread()
{
    qDebug() << "Destroy MarketWavesThread";
}

void MarketWavesThread::run()
{
    qDebug() << "Running MarketWavesThread";

    qDebug() << "Finish MarketWavesThread";
}
