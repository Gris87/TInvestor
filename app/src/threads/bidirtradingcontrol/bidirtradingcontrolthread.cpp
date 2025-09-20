#include "src/threads/bidirtradingcontrol/bidirtradingcontrolthread.h"

#include <QDateTime>
#include <QDebug>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



constexpr float  HUNDRED_PERCENT            = 100.0f;
constexpr float  LIMIT_COMMISSION           = 0.06f;
constexpr int    EXTRA_SESSION_END_HOUR     = 23;
constexpr int    EXTRA_SESSION_END_MINUTE   = 30;
constexpr int    AMOUNT_OF_LAST_INSTRUMENTS = 5;
constexpr qint64 MS_IN_SECOND               = 1000LL;
constexpr qint64 ONE_MINUTE                 = 60LL * MS_IN_SECOND;
constexpr qint64 DETECTION_INTERVAL         = 15LL * ONE_MINUTE; // 15 minutes



BiDirTradingControlThread::BiDirTradingControlThread(
    IStocksStorage* stocksStorage,
    IUserStorage*   userStorage,
    IConfig*        config,
    ITimeUtils*     timeUtils,
    IGrpcClient*    grpcClient,
    QObject*        parent
) :
    IBiDirTradingControlThread(parent),
    mStocksStorage(stocksStorage),
    mUserStorage(userStorage),
    mConfig(config),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mMoscowTimezone("Europe/Moscow"),
    mLastDetectionTimestamp(),
    mLastInstrumentsForBiDirTrading(),
    mLastInstrumentsId()
{
    qDebug() << "Create BiDirTradingControlThread";
}

BiDirTradingControlThread::~BiDirTradingControlThread()
{
    qDebug() << "Destroy BiDirTradingControlThread";
}

void BiDirTradingControlThread::run()
{
    qDebug() << "Running BiDirTradingControlThread";

    blockSignals(false);

    const qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

    if (timestamp - mLastDetectionTimestamp > DETECTION_INTERVAL)
    {
        detectHugeSpreadStocks(timestamp);

        mLastDetectionTimestamp = timestamp;
    }

    qDebug() << "Finish BiDirTradingControlThread";
}

struct DetectHugeSpreadStocksInfo
{
    explicit DetectHugeSpreadStocksInfo(IGrpcClient* _grpcClient, bool _qualifiedUser, float _hugeSpread) :
        grpcClient(_grpcClient),
        qualifiedUser(_qualifiedUser),
        hugeSpread(_hugeSpread)
    {
        results.resize(getCpuCount());
        resultsArray = results.data();
    }

    IGrpcClient*                      grpcClient;
    bool                              qualifiedUser;
    float                             hugeSpread;
    QList<InstrumentsForBiDirTrading> results;
    InstrumentsForBiDirTrading*       resultsArray;
};

static void detectHugeSpreadStocksForParallel(
    QThread* parentThread, int threadId, Stock** stocks, int /*size*/, int start, int end, void* additionalArgs
)
{
    DetectHugeSpreadStocksInfo* detectHugeSpreadStocksInfo = reinterpret_cast<DetectHugeSpreadStocksInfo*>(additionalArgs);

    IGrpcClient*                grpcClient    = detectHugeSpreadStocksInfo->grpcClient;
    const bool                  qualifiedUser = detectHugeSpreadStocksInfo->qualifiedUser;
    const float                 hugeSpread    = detectHugeSpreadStocksInfo->hugeSpread;
    InstrumentsForBiDirTrading* resultsArray  = detectHugeSpreadStocksInfo->resultsArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks[i];

        stock->readLock();

        if (qualifiedUser || !stock->meta.forQualInvestorFlag)
        {
            const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
                grpcClient->getOrderBook(parentThread, stock->meta.instrumentId, 1);

            if (!parentThread->isInterruptionRequested() && tinkoffOrderBook != nullptr)
            {
                if (tinkoffOrderBook->bids_size() > 0 && tinkoffOrderBook->asks_size() > 0)
                {
                    const float bidPrice = quotationToFloat(tinkoffOrderBook->bids(0).price());
                    const float askPrice = quotationToFloat(tinkoffOrderBook->asks(0).price());

                    const float spread = ((askPrice / bidPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    if (spread > hugeSpread)
                    {
                        resultsArray[threadId][stock->meta.instrumentId] = BiDirTradingInfo(
                            stock->meta.turnover,
                            QObject::tr("Decided to start reselling because spread is %1")
                                .arg(QString::number(spread, 'f', 3) + "%")
                        );
                    }
                }
            }
        }

        stock->readUnlock();
    }
}

void BiDirTradingControlThread::detectHugeSpreadStocks(qint64 timestamp)
{
    InstrumentsForBiDirTrading instrumentsForTrading;

    const QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(timestamp, mMoscowTimezone);
    const QTime     time     = dateTime.time();

    const QTime endTime = QTime(EXTRA_SESSION_END_HOUR, EXTRA_SESSION_END_MINUTE);

    if ((mConfig->isTradeInNonWorkingHours() || mTimeUtils->isWorkingHours(timestamp)) && time < endTime)
    {
        mUserStorage->readLock();
        const bool  qualifiedUser = mUserStorage->isQualified();
        const float commission    = mUserStorage->getCommission();
        mUserStorage->readUnlock();

        if (commission < LIMIT_COMMISSION)
        {
            mStocksStorage->readLock();
            QList<Stock*> stocks = mStocksStorage->getStocks();
            mStocksStorage->readUnlock();

            DetectHugeSpreadStocksInfo detectHugeSpreadStocksInfo(mGrpcClient, qualifiedUser, mConfig->getHugeSpread());
            processInParallel(QThread::currentThread(), stocks, detectHugeSpreadStocksForParallel, &detectHugeSpreadStocksInfo);

            for (const InstrumentsForBiDirTrading& result : std::as_const(detectHugeSpreadStocksInfo.results))
            {
                instrumentsForTrading.insert(result);
            }
        }
    }
    else
    {
        mLastInstrumentsForBiDirTrading.clear();
        mLastInstrumentsId = 0;
    }

    if (!instrumentsForTrading.isEmpty())
    {
        QStringList instrumentsToRemove;

        if (mLastInstrumentsForBiDirTrading.size() < AMOUNT_OF_LAST_INSTRUMENTS)
        {
            mLastInstrumentsForBiDirTrading.append(instrumentsForTrading);
        }
        else
        {
            mLastInstrumentsForBiDirTrading[mLastInstrumentsId] = instrumentsForTrading;
        }

        int i = mLastInstrumentsId == 0 ? mLastInstrumentsForBiDirTrading.size() - 1 : mLastInstrumentsId - 1;

        while (i != mLastInstrumentsId)
        {
            const InstrumentsForBiDirTrading& lastInstruments = mLastInstrumentsForBiDirTrading.at(i);

            for (auto it = instrumentsForTrading.constBegin(), end = instrumentsForTrading.constEnd(); it != end; ++it)
            {
                const QString& instrumentId = it.key();

                if (!lastInstruments.contains(instrumentId))
                {
                    instrumentsToRemove.append(instrumentId);
                }
            }

            --i;

            if (i < 0)
            {
                i = mLastInstrumentsForBiDirTrading.size() - 1;
            }
        }

        for (const QString& instrumentToRemove : instrumentsToRemove)
        {
            instrumentsForTrading.remove(instrumentToRemove);
        }

        mLastInstrumentsId = (mLastInstrumentsId + 1) % AMOUNT_OF_LAST_INSTRUMENTS;
    }

    if (!instrumentsForTrading.isEmpty())
    {
        emit tradeInstruments(instrumentsForTrading);
    }
}

void BiDirTradingControlThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
