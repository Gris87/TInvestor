#include "src/threads/orderbook/orderbookthread.h"

#include <QDebug>



OrderBookThread::OrderBookThread(QObject* parent) :
    IOrderBookThread(parent)
{
    qDebug() << "Create OrderBookThread";
}

OrderBookThread::~OrderBookThread()
{
    qDebug() << "Destroy OrderBookThread";
}

void OrderBookThread::run()
{
    qDebug() << "Running OrderBookThread";

    qDebug() << "Finish OrderBookThread";
}
