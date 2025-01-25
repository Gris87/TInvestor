#include "refreshthread.h"

#include <QDebug>



RefreshThread::RefreshThread(QObject *parent) :
    QThread(parent)
{
    qDebug() << "Create RefreshThread";
}

RefreshThread::~RefreshThread()
{
    qDebug() << "Destroy RefreshThread";
}

void RefreshThread::run()
{
    qDebug() << "Running RefreshThread";

    qDebug() << "Finish RefreshThread";
}
