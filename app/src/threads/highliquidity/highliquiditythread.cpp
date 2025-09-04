#include "src/threads/highliquidity/highliquiditythread.h"

#include <QDebug>



HighLiquidityThread::HighLiquidityThread(QObject* parent) :
    IHighLiquidityThread(parent)
{
    qDebug() << "Create HighLiquidityThread";
}

HighLiquidityThread::~HighLiquidityThread()
{
    qDebug() << "Destroy HighLiquidityThread";
}

void HighLiquidityThread::run()
{
    qDebug() << "Running HighLiquidityThread";

    blockSignals(false);

    qDebug() << "Finish HighLiquidityThread";
}

void HighLiquidityThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
