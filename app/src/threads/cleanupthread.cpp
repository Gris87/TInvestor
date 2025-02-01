#include "cleanupthread.h"

#include <QDebug>



CleanupThread::CleanupThread(QObject *parent) :
    ICleanupThread(parent)
{
    qDebug() << "Create CleanupThread";
}

CleanupThread::~CleanupThread()
{
    qDebug() << "Destroy CleanupThread";
}

void CleanupThread::process()
{
    qDebug() << "Running CleanupThread";

    qDebug() << "Finish CleanupThread";
}
