#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <QDebug>



BiDirTradingThread::BiDirTradingThread(QObject* parent) :
    IBiDirTradingThread(parent)
{
    qDebug() << "Create BiDirTradingThread";
}

BiDirTradingThread::~BiDirTradingThread()
{
    qDebug() << "Destroy BiDirTradingThread";
}

void BiDirTradingThread::run()
{
    qDebug() << "Running BiDirTradingThread";

    blockSignals(false);

    qDebug() << "Finish BiDirTradingThread";
}

void BiDirTradingThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
