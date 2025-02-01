#include "refreshthread.h"

#include <QDebug>



RefreshThread::RefreshThread(QObject *parent) :
    IRefreshThread(parent)
{
    qDebug() << "Create RefreshThread";
}

RefreshThread::~RefreshThread()
{
    qDebug() << "Destroy RefreshThread";
}

void RefreshThread::process()
{
    qDebug() << "Running RefreshThread";

    qDebug() << "Finish RefreshThread";
}
