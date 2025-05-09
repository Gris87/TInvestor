#include "src/threads/operations/operationsthread.h"

#include <QDebug>



OperationsThread::OperationsThread(IUserStorage* userStorage, QObject* parent) :
    IOperationsThread(parent),
    mUserStorage(userStorage)
{
    qDebug() << "Create OperationsThread";
}

OperationsThread::~OperationsThread()
{
    qDebug() << "Destroy OperationsThread";
}

void OperationsThread::run()
{
    qDebug() << "Running OperationsThread";

    qDebug() << "Finish OperationsThread";
}
