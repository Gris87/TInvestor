#include "src/threads/pricecollect/pricecollectthread.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/grpc/utils.h"



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

void PriceCollectThread::run()
{
    qDebug() << "Running PriceCollectThread";

    std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks = mGrpcClient->findStocks();

    if (tinkoffStocks != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        QList<Stock> stocks;

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

                stocks.append(stock);
            }
        }

        qInfo() << stocks.size();
    }

    qDebug() << "Finish PriceCollectThread";
}
