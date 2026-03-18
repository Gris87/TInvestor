#include "src/threads/detectshorts/detectshortsthread.h"

#include <QDebug>



DetectShortsThread::DetectShortsThread(IStocksStorage* stocksStorage, IGrpcClient* grpcClient, QObject* parent) :
    IDetectShortsThread(parent),
    mStocksStorage(stocksStorage),
    mGrpcClient(grpcClient)
{
    qDebug() << "Create DetectShortsThread";
}

DetectShortsThread::~DetectShortsThread()
{
    qDebug() << "Destroy DetectShortsThread";
}

void DetectShortsThread::run()
{
    qDebug() << "Running DetectShortsThread";

    blockSignals(false);

    const std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks =
        mGrpcClient->findStocks(QThread::currentThread(), tinkoff::INSTRUMENT_STATUS_BASE);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffStocks != nullptr)
    {
        const QMap<QString, Stock*> stocksMap = buildStocksMap();
        const qint64                timestamp = QDateTime::currentMSecsSinceEpoch();

        for (int i = 0; i < tinkoffStocks->instruments_size(); ++i)
        {
            const tinkoff::Share& tinkoffStock = tinkoffStocks->instruments(i);
            Stock*                stock        = stocksMap.value(QString::fromStdString(tinkoffStock.uid()));

            if (stock != nullptr)
            {
                stock->writeLock();
                stock->meta.shorts.enabled = tinkoffStock.short_enabled_flag();

                if (stock->meta.shorts.enabled)
                {
                    stock->meta.shorts.lastEnabledTimestamp = timestamp;
                }

                stock->writeUnlock();
            }
        }

        mStocksStorage->readLock();
        mStocksStorage->writeStocksMeta();
        mStocksStorage->readUnlock();
    }

    qDebug() << "Finish DetectShortsThread";
}

void DetectShortsThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

QMap<QString, Stock*> DetectShortsThread::buildStocksMap()
{
    QMap<QString, Stock*> res; // Instrument ID => Stock

    mStocksStorage->readLock();
    const QList<Stock*>& stocks = mStocksStorage->getStocks();

    for (Stock* stock : stocks)
    {
        stock->readLock();
        res[stock->meta.instrumentId] = stock;
        stock->readUnlock();
    }
    mStocksStorage->readUnlock();

    return res;
}
