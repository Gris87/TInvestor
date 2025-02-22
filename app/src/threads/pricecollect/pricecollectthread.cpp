#include "src/threads/pricecollect/pricecollectthread.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



PriceCollectThread::PriceCollectThread(IStocksStorage* stocksStorage, IGrpcClient* grpcClient, QObject* parent) :
    IPriceCollectThread(parent),
    mStocksStorage(stocksStorage),
    mGrpcClient(grpcClient)
{
    qDebug() << "Create PriceCollectThread";
}

PriceCollectThread::~PriceCollectThread()
{
    qDebug() << "Destroy PriceCollectThread";
}

void downloadLogosForParallel(QThread* parentThread, QList<tinkoff::Share*>* stocks, int start, int end, void* /*additionalArgs*/)
{
    tinkoff::Share** stockArray = stocks->data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        tinkoff::Share* stock = stockArray[i];

        qInfo() << stock->isin();
    }
}

void PriceCollectThread::run()
{
    qDebug() << "Running PriceCollectThread";

    std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks = mGrpcClient->findStocks();

    if (tinkoffStocks != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        QList<tinkoff::Share*> qtTinkoffStocks;
        QList<Stock>           stocks;

        for (int i = 0; i < tinkoffStocks->instruments_size(); ++i)
        {
            const tinkoff::Share& tinkoffStock = tinkoffStocks->instruments(i);

            if (QString::fromStdString(tinkoffStock.currency()) == "rub" &&
                QString::fromStdString(tinkoffStock.country_of_risk()) == "RU" && tinkoffStock.api_trade_available_flag())
            {
                Stock stock;

                stock.uid                 = QString::fromStdString(tinkoffStock.uid());
                stock.ticker              = QString::fromStdString(tinkoffStock.ticker());
                stock.name                = QString::fromStdString(tinkoffStock.name());
                stock.forQualInvestorFlag = tinkoffStock.for_qual_investor_flag();
                stock.lot                 = tinkoffStock.lot();
                stock.minPriceIncrement   = quotationToFloat(tinkoffStock.min_price_increment());

                qtTinkoffStocks.append(const_cast<tinkoff::Share*>(&tinkoffStock));
                stocks.append(stock);
            }
        }

        processInParallel(&qtTinkoffStocks, downloadLogosForParallel);
    }

    qDebug() << "Finish PriceCollectThread";
}
