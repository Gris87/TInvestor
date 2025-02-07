#include "makedecisionthread.h"

#include <QDebug>



MakeDecisionThread::MakeDecisionThread(
    IConfig *config,
    IStocksDatabase *stocksDatabase,
    IStocksStorage *stocksStorage,
    QObject *parent
) :
    IMakeDecisionThread(parent),
    mConfig(config),
    mStocksDatabase(stocksDatabase),
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

    qDebug() << "Finish MakeDecisionThread";
}
