#include "src/threads/makedecision/makedecisionthread.h"

#include <QDebug>



MakeDecisionThread::MakeDecisionThread(IConfig* config, IStocksStorage* stocksStorage, QObject* parent) :
    IMakeDecisionThread(parent),
    mConfig(config),
    mStocksStorage(stocksStorage)
{
    qDebug() << "Create MakeDecisionThread";
}

MakeDecisionThread::~MakeDecisionThread()
{
    qDebug() << "Destroy MakeDecisionThread";
}

void MakeDecisionThread::run()
{
    qDebug() << "Running MakeDecisionThread";

    // TODO: Do we need it?

    qDebug() << "Finish MakeDecisionThread";
}
