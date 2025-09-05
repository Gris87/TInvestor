#include "src/threads/bidirtrading/bidirtradingthreadfactory.h"

#include <QDebug>

#include "src/threads/bidirtrading/bidirtradingthread.h"



BiDirTradingThreadFactory::BiDirTradingThreadFactory() :
    IBiDirTradingThreadFactory()
{
    qDebug() << "Create BiDirTradingThreadFactory";
}

BiDirTradingThreadFactory::~BiDirTradingThreadFactory()
{
    qDebug() << "Destroy BiDirTradingThreadFactory";
}

IBiDirTradingThread* BiDirTradingThreadFactory::newInstance(QObject* parent) const
{
    return new BiDirTradingThread(parent);
}
