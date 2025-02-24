#include "src/threads/pricecollect/pricecollectthread.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMutexLocker>

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
            fileFactory->newInstance(QString("%1/data/stocks/logos/%2.png").arg(appDir, uid));

        if (!stockLogoFile->exists())
        {
            QString logoName = QString::fromStdString(stock->brand().logo_name()).replace(".png", "x160.png"); // 160 pixels
            QString url      = QString("https://invest-brands.cdn-tinkoff.ru/%1").arg(logoName);

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
        QList<StockMeta>             stocksMeta;

        for (int i = 0; i < tinkoffStocks->instruments_size(); ++i)
        {
            const tinkoff::Share& tinkoffStock = tinkoffStocks->instruments(i);

            if (QString::fromStdString(tinkoffStock.currency()) == "rub" &&
                QString::fromStdString(tinkoffStock.country_of_risk()) == "RU" && tinkoffStock.api_trade_available_flag())
            {
                StockMeta stockMeta;

                stockMeta.uid                    = QString::fromStdString(tinkoffStock.uid());
                stockMeta.ticker                 = QString::fromStdString(tinkoffStock.ticker());
                stockMeta.name                   = QString::fromStdString(tinkoffStock.name());
                stockMeta.forQualInvestorFlag    = tinkoffStock.for_qual_investor_flag();
                stockMeta.lot                    = tinkoffStock.lot();
                stockMeta.minPriceIncrement.units = tinkoffStock.min_price_increment().units();
                stockMeta.minPriceIncrement.nano  = tinkoffStock.min_price_increment().nano();

                qtTinkoffStocks.append(&tinkoffStock);
                stocksMeta.append(stockMeta);
            }
        }

        DownloadLogosInfo downloadLogosInfo;
        downloadLogosInfo.fileFactory = mFileFactory;
        downloadLogosInfo.httpClient  = mHttpClient;

        processInParallel(&qtTinkoffStocks, downloadLogosForParallel, &downloadLogosInfo);

        QMutexLocker locker(mStocksStorage->getMutex());
        mStocksStorage->mergeStocksMeta(stocksMeta);
    }

    qDebug() << "Finish PriceCollectThread";
}
