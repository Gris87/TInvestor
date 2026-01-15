#include "src/threads/bidirtradingcontrol/bidirtradingcontrolthread.h"

#include <QDateTime>
#include <QDebug>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



constexpr float  HUNDRED_PERCENT            = 100.0f;
constexpr float  LIMIT_COMMISSION           = 0.06f;
constexpr int    ORDER_BOOK_DEPTH           = 20;
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
    mLastDetectionTimestamp(),
    mLastTradeHugeBid(),
    mLastTradeHugeSpread(),
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

    const qint64 timestamp       = QDateTime::currentMSecsSinceEpoch();
    const bool   tradeHugeBid    = mConfig->isTradeHugeBid();
    const bool   tradeHugeSpread = mConfig->isTradeHugeSpread();

    if (timestamp - mLastDetectionTimestamp > DETECTION_INTERVAL || mLastTradeHugeBid != tradeHugeBid ||
        mLastTradeHugeSpread != tradeHugeSpread)
    {
        detectStocksForBiDirTrading(timestamp, tradeHugeBid, tradeHugeSpread);

        mLastDetectionTimestamp = timestamp;
        mLastTradeHugeBid       = tradeHugeBid;
        mLastTradeHugeSpread    = tradeHugeSpread;
    }

    qDebug() << "Finish BiDirTradingControlThread";
}

static BiDirTradingInfo
checkStockForHugeBid(const std::shared_ptr<tinkoff::GetOrderBookResponse>& tinkoffOrderBook, Stock* stock, float hugeBid)
{
    BiDirTradingInfo res;

    qint64 bids = 0;
    qint64 asks = 0;

    for (int i = 0; i < tinkoffOrderBook->bids_size(); ++i)
    {
        bids += tinkoffOrderBook->bids(i).quantity();
    }

    for (int i = 0; i < tinkoffOrderBook->asks_size(); ++i)
    {
        asks += tinkoffOrderBook->asks(i).quantity();
    }

    if (bids > 0 && asks > 0)
    {
        const float coef = static_cast<double>(asks) / static_cast<double>(bids);

        if (coef > hugeBid)
        {
            res = BiDirTradingInfo(
                stock,
                BIDIR_MODE_HUGE_BID,
                QObject::tr("Decided to start reselling because amount of asks more than amount of bids in %1 times")
                    .arg(QString::number(coef, 'f', 2))
            );
        }
    }

    return res;
}

static BiDirTradingInfo
checkStockForHugeSpread(const std::shared_ptr<tinkoff::GetOrderBookResponse>& tinkoffOrderBook, Stock* stock, float hugeSpread)
{
    BiDirTradingInfo res;

    const float bidPrice = quotationToFloat(tinkoffOrderBook->bids(0).price());
    const float askPrice = quotationToFloat(tinkoffOrderBook->asks(0).price());

    const float spread = ((askPrice / bidPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

    if (spread > hugeSpread)
    {
        res = BiDirTradingInfo(
            stock,
            BIDIR_MODE_HUGE_SPREAD,
            QObject::tr("Decided to start reselling because spread is %1").arg(QString::number(spread, 'f', 3) + "%")
        );
    }

    return res;
}

static BiDirTradingInfo checkStockForHugeBidOrSpread(
    const std::shared_ptr<tinkoff::GetOrderBookResponse>& tinkoffOrderBook,
    Stock*                                                stock,
    bool                                                  tradeHugeBid,
    bool                                                  tradeHugeSpread,
    float                                                 hugeBid,
    float                                                 hugeSpread
)
{
    BiDirTradingInfo res;

    if (tradeHugeBid)
    {
        const BiDirTradingInfo biDirTradingInfo = checkStockForHugeBid(tinkoffOrderBook, stock, hugeBid);

        if (biDirTradingInfo.cause != "")
        {
            res = biDirTradingInfo;
        }
    }

    if (tradeHugeSpread)
    {
        const BiDirTradingInfo biDirTradingInfo = checkStockForHugeSpread(tinkoffOrderBook, stock, hugeSpread);

        if (biDirTradingInfo.cause != "")
        {
            res = biDirTradingInfo;
        }
    }

    return res;
}

struct DetectStocksForBiDirTradingInfo
{
    explicit DetectStocksForBiDirTradingInfo(
        IGrpcClient* _grpcClient,
        bool         _qualifiedUser,
        bool         _tradeHugeBid,
        bool         _tradeHugeSpread,
        float        _hugeBid,
        float        _hugeSpread
    ) :
        grpcClient(_grpcClient),
        qualifiedUser(_qualifiedUser),
        tradeHugeBid(_tradeHugeBid),
        tradeHugeSpread(_tradeHugeSpread),
        hugeBid(_hugeBid),
        hugeSpread(_hugeSpread)
    {
        results.resize(getCpuCount());
        resultsArray = results.data();
    }

    IGrpcClient*                      grpcClient;
    bool                              qualifiedUser;
    bool                              tradeHugeBid;
    bool                              tradeHugeSpread;
    float                             hugeBid;
    float                             hugeSpread;
    QList<InstrumentsForBiDirTrading> results;
    InstrumentsForBiDirTrading*       resultsArray;
};

static void detectStocksForBiDirTradingForParallel(
    QThread* parentThread, int threadId, Stock** stocks, int /*size*/, int start, int end, void* additionalArgs
)
{
    const DetectStocksForBiDirTradingInfo* detectStocksForBiDirTradingInfo =
        reinterpret_cast<DetectStocksForBiDirTradingInfo*>(additionalArgs);

    IGrpcClient*                grpcClient      = detectStocksForBiDirTradingInfo->grpcClient;
    const bool                  qualifiedUser   = detectStocksForBiDirTradingInfo->qualifiedUser;
    const bool                  tradeHugeBid    = detectStocksForBiDirTradingInfo->tradeHugeBid;
    const bool                  tradeHugeSpread = detectStocksForBiDirTradingInfo->tradeHugeSpread;
    const float                 hugeBid         = detectStocksForBiDirTradingInfo->hugeBid;
    const float                 hugeSpread      = detectStocksForBiDirTradingInfo->hugeSpread;
    InstrumentsForBiDirTrading* resultsArray    = detectStocksForBiDirTradingInfo->resultsArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks[i];

        stock->readLock();

        if (qualifiedUser || !stock->meta.forQualInvestorFlag)
        {
            const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
                grpcClient->getOrderBook(parentThread, stock->meta.instrumentId, ORDER_BOOK_DEPTH);

            if (!parentThread->isInterruptionRequested() && tinkoffOrderBook != nullptr)
            {
                if (tinkoffOrderBook->bids_size() > 0 && tinkoffOrderBook->asks_size() > 0)
                {
                    const BiDirTradingInfo biDirTradingInfo =
                        checkStockForHugeBidOrSpread(tinkoffOrderBook, stock, tradeHugeBid, tradeHugeSpread, hugeBid, hugeSpread);

                    if (biDirTradingInfo.cause != "")
                    {
                        resultsArray[threadId][stock->meta.instrumentId] = biDirTradingInfo;
                    }
                }
            }
        }

        stock->readUnlock();
    }
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void BiDirTradingControlThread::detectStocksForBiDirTrading(qint64 timestamp, bool tradeHugeBid, bool tradeHugeSpread)
{
    InstrumentsForBiDirTrading instrumentsForTrading;

    if (mTimeUtils->isWorkingHours(timestamp))
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

            const float hugeBid    = mConfig->getHugeBid();
            const float hugeSpread = mConfig->getHugeSpread();

            DetectStocksForBiDirTradingInfo detectStocksForBiDirTradingInfo(
                mGrpcClient, qualifiedUser, tradeHugeBid, tradeHugeSpread, hugeBid, hugeSpread
            );
            processInParallel(
                QThread::currentThread(), stocks, detectStocksForBiDirTradingForParallel, &detectStocksForBiDirTradingInfo
            );

            for (const InstrumentsForBiDirTrading& result : std::as_const(detectStocksForBiDirTradingInfo.results))
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

            for (auto it = lastInstruments.constBegin(), end = lastInstruments.constEnd(); it != end; ++it)
            {
                const QString& instrumentId = it.key();

                if (!instrumentsForTrading.contains(instrumentId))
                {
                    instrumentsForTrading[instrumentId] = it.value();
                }
            }

            --i;

            if (i < 0)
            {
                i = mLastInstrumentsForBiDirTrading.size() - 1;
            }
        }

        mLastInstrumentsId = (mLastInstrumentsId + 1) % AMOUNT_OF_LAST_INSTRUMENTS;
    }

    emit tradeInstruments(instrumentsForTrading);
}
// NOLINTEND(readability-function-cognitive-complexity)

void BiDirTradingControlThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
