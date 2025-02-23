#include "src/threads/pricecollect/pricecollectthread.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMutexLocker>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



PriceCollectThread::PriceCollectThread(
    IStocksStorage* stocksStorage, IFileFactory* fileFactory, IHttpClient* httpClient, IGrpcClient* grpcClient, QObject* parent
) :
    IPriceCollectThread(parent),
    mStocksStorage(stocksStorage),
    mFileFactory(fileFactory),
    mHttpClient(httpClient),
    mGrpcClient(grpcClient)
{
    qDebug() << "Create PriceCollectThread";
}

PriceCollectThread::~PriceCollectThread()
{
    qDebug() << "Destroy PriceCollectThread";
}

struct DownloadLogosInfo
{
    IFileFactory* fileFactory;
    IHttpClient*  httpClient;
};

void
downloadLogosForParallel(QThread* parentThread, QList<const tinkoff::Share*>* stocks, int start, int end, void* additionalArgs)
{
    DownloadLogosInfo* downloadLogosInfo = reinterpret_cast<DownloadLogosInfo*>(additionalArgs);
    IFileFactory*      fileFactory       = downloadLogosInfo->fileFactory;
    IHttpClient*       httpClient        = downloadLogosInfo->httpClient;

    QString appDir = qApp->applicationDirPath();

    const tinkoff::Share** stockArray = stocks->data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        const tinkoff::Share* stock = stockArray[i];

        QString uid = QString::fromStdString(stock->uid());

        std::shared_ptr<IFile> stockLogoFile =
            fileFactory->newInstance(QString("%1/data/db/stocks/logos/%2.png").arg(appDir, uid));

        if (!stockLogoFile->exists())
        {
            QString isin = QString::fromStdString(stock->isin());
            QString url  = QString("https://invest-brands.cdn-tinkoff.ru/%1x160.png").arg(isin); // 160 pixels

            std::shared_ptr<QByteArray> data = httpClient->download(url);

            if (data)
            {
                stockLogoFile->open(QIODevice::WriteOnly);
                stockLogoFile->write(*data);
                stockLogoFile->close();
            }
        }
    }
}

void PriceCollectThread::run()
{
    qDebug() << "Running PriceCollectThread";

    std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks = mGrpcClient->findStocks();

    if (tinkoffStocks != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        QList<const tinkoff::Share*> qtTinkoffStocks;
        QList<Stock>                 stocks;

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

                qtTinkoffStocks.append(&tinkoffStock);
                stocks.append(stock);
            }
        }

        DownloadLogosInfo downloadLogosInfo;
        downloadLogosInfo.fileFactory = mFileFactory;
        downloadLogosInfo.httpClient  = mHttpClient;

        processInParallel(&qtTinkoffStocks, downloadLogosForParallel, &downloadLogosInfo);
    }

    qDebug() << "Finish PriceCollectThread";
}
