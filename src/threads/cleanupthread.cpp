#include "cleanupthread.h"

#include <QDebug>



CleanupThread::CleanupThread(QObject *parent) :
    QThread(parent)
{
    qDebug() << "Create CleanupThread";
}

CleanupThread::~CleanupThread()
{
    qDebug() << "Destroy CleanupThread";
}

void CleanupThread::run()
{
    qDebug() << "Running CleanupThread";
}
