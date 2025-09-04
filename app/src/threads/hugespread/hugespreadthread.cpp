#include "src/threads/hugespread/hugespreadthread.h"

#include <QDebug>



HugeSpreadThread::HugeSpreadThread(QObject* parent) :
    IHugeSpreadThread(parent)
{
    qDebug() << "Create HugeSpreadThread";
}

HugeSpreadThread::~HugeSpreadThread()
{
    qDebug() << "Destroy HugeSpreadThread";
}

void HugeSpreadThread::run()
{
    qDebug() << "Running HugeSpreadThread";

    blockSignals(false);

    qDebug() << "Finish HugeSpreadThread";
}

void HugeSpreadThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
