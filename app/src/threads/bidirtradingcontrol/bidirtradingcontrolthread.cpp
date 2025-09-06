#include "src/threads/bidirtradingcontrol/bidirtradingcontrolthread.h"

#include <QDateTime>
#include <QDebug>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



const char* const TMON_UID = "498ec3ff-ef27-4729-9703-a5aac48d5789";

constexpr float  HUNDRED_PERCENT             = 100.0f;
constexpr float  LIMIT_COMMISSION            = 0.06f;
constexpr int    NORMAL_SESSION_START_HOUR   = 10;
constexpr int    NORMAL_SESSION_START_MINUTE = 5;
constexpr int    NORMAL_SESSION_END_HOUR     = 18;
constexpr int    NORMAL_SESSION_END_MINUTE   = 20;
constexpr int    EXTRA_SESSION_END_HOUR      = 23;
constexpr int    EXTRA_SESSION_END_MINUTE    = 30;
constexpr qint64 MS_IN_SECOND                = 1000LL;
constexpr qint64 ONE_MINUTE                  = 60LL * MS_IN_SECOND;
constexpr qint64 DETECTION_INTERVAL          = 15LL * ONE_MINUTE; // 15 minutes



BiDirTradingControlThread::BiDirTradingControlThread(
    IStocksStorage* stocksStorage, IUserStorage* userStorage, IConfig* config, IGrpcClient* grpcClient, QObject* parent
) :
    IBiDirTradingControlThread(parent),
    mStocksStorage(stocksStorage),
    mUserStorage(userStorage),
    mConfig(config),
    mGrpcClient(grpcClient),
    mMoscowTimezone("Europe/Moscow"),
    mAccountId(),
    mLastDetectionTimestamp(),
    mLastTradeHugeSpread(),
    mLastTradeLiquidityEtfDaily()
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

    const qint64 timestamp              = QDateTime::currentMSecsSinceEpoch();
    const bool   tradeHugeSpread        = mConfig->isTradeHugeSpread();
    const bool   tradeLiquidityEtfDaily = mConfig->isTradeLiquidityEtfDaily();

    if (timestamp - mLastDetectionTimestamp > DETECTION_INTERVAL || mLastTradeHugeSpread != tradeHugeSpread ||
        mLastTradeLiquidityEtfDaily != tradeLiquidityEtfDaily)
    {
        detectHugeSpreadStocks(timestamp, tradeHugeSpread, tradeLiquidityEtfDaily);

        mLastDetectionTimestamp     = timestamp;
        mLastTradeHugeSpread        = tradeHugeSpread;
        mLastTradeLiquidityEtfDaily = tradeLiquidityEtfDaily;
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

                    const float spread = (askPrice / bidPrice) * HUNDRED_PERCENT - HUNDRED_PERCENT;

                    if (spread > hugeSpread)
                    {
                        resultsArray[threadId][stock->meta.instrumentId] =
                            BiDirTradingInfo(QObject::tr("Decided to start reselling because spread is %1")
                                                 .arg(QString::number(spread, 'f', 3) + "%"));
                    }
                }
            }
        }

        stock->readUnlock();
    }
}

void BiDirTradingControlThread::detectHugeSpreadStocks(qint64 timestamp, bool tradeHugeSpread, bool tradeLiquidityEtfDaily)
{
    InstrumentsForBiDirTrading instrumentsForTrading;

    if (tradeHugeSpread)
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

            for (const InstrumentsForBiDirTrading& result : detectHugeSpreadStocksInfo.results)
            {
                instrumentsForTrading.insert(result);
            }
        }
    }

    if (tradeLiquidityEtfDaily)
    {
        const QDateTime dateTime  = QDateTime::fromMSecsSinceEpoch(timestamp, mMoscowTimezone);
        const QTime     time      = dateTime.time();
        const QTime     startTime = QTime(NORMAL_SESSION_START_HOUR, NORMAL_SESSION_START_MINUTE);
        const QTime     endTime   = mConfig->isTradeInNonWorkingHours() ? QTime(EXTRA_SESSION_END_HOUR, EXTRA_SESSION_END_MINUTE)
                                                                        : QTime(NORMAL_SESSION_END_HOUR, NORMAL_SESSION_END_MINUTE);

        if (time >= startTime && time < endTime)
        {
            instrumentsForTrading[TMON_UID] =
                BiDirTradingInfo(tr("Decided to start reselling of high liquidity ETF because it requested from config"));
        }
    }

    if (!instrumentsForTrading.isEmpty())
    {
        emit tradeInstruments(instrumentsForTrading);
    }
}

void BiDirTradingControlThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void BiDirTradingControlThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
