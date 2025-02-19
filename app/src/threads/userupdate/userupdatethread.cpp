#include "src/threads/userupdate/userupdatethread.h"

#include <QDateTime>
#include <QDebug>
#include <QMutexLocker>



UserUpdateThread::UserUpdateThread(
    IConfig* config, IStocksDatabase* stocksDatabase, IStocksStorage* stocksStorage, QObject* parent
) :
    IUserUpdateThread(parent),
    mConfig(config),
    mStocksDatabase(stocksDatabase),
    mStocksStorage(stocksStorage)
{
    qDebug() << "Create UserUpdateThread";
}

UserUpdateThread::~UserUpdateThread()
{
    qDebug() << "Destroy UserUpdateThread";
}

void UserUpdateThread::run()
{
    qDebug() << "Running UserUpdateThread";

    qint64 obsoleteTimestamp = QDateTime::currentSecsSinceEpoch() - mConfig->getStorageMonthLimit() * 31 * 24 * 60 * 60;

    QMutexLocker lock(mStocksStorage->getMutex());

    QList<Stock>* stocks = mStocksStorage->getStocks();
    mStocksDatabase->deleteObsoleteData(obsoleteTimestamp, stocks);

    qDebug() << "Finish UserUpdateThread";
}
