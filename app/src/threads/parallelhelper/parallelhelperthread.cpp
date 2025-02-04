#include "parallelhelperthread.h"

#include <QDebug>



ParallelHelperThread::ParallelHelperThread(QObject *parent) :
    QThread(parent)
{
    qDebug() << "Create ParallelHelperThread";
}

ParallelHelperThread::~ParallelHelperThread()
{
    qDebug() << "Destroy ParallelHelperThread";
}

void ParallelHelperThread::run()
{
    qDebug() << "Running ParallelHelperThread";

    qDebug() << "Finish ParallelHelperThread";
}
