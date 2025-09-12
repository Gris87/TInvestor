#include "src/threads/autopilotdecisionmaker/autopilotdecisionmakerthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 ONE_MINUTE           = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR             = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY              = 24LL * ONE_HOUR;
constexpr qint64 DATE_RANGE           = 90LL * ONE_DAY;
constexpr qint64 SLEEP_BEFORE_REQUEST = 1LL * MS_IN_SECOND; // 1 second



AutoPilotDecisionMakerThread::AutoPilotDecisionMakerThread(
    IStocksStorage*   stocksStorage,
    IConfig*          config,
    IDecisionMaker*   decisionMaker,
    ITimeUtils*       timeUtils,
    IGrpcClient*      grpcClient,
    IGrpcRetryClient* grpcRetryClient,
    QObject*          parent
) :
    IAutoPilotDecisionMakerThread(parent),
    mRwMutex(new QReadWriteLock()),
    mStocksStorage(stocksStorage),
    mConfig(config),
    mDecisionMaker(decisionMaker),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mGrpcRetryClient(grpcRetryClient),
    mAccountId(),
    mSellNotifications()
{
    qDebug() << "Create AutoPilotDecisionMakerThread";
}

AutoPilotDecisionMakerThread::~AutoPilotDecisionMakerThread()
{
    qDebug() << "Destroy AutoPilotDecisionMakerThread";

    delete mRwMutex;
}

void AutoPilotDecisionMakerThread::run()
{
    qDebug() << "Running AutoPilotDecisionMakerThread";

    blockSignals(false);

    const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
        mGrpcRetryClient->getValidPortfolio(QThread::currentThread(), mAccountId);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
    {
        const Portfolio portfolio = handlePortfolioResponse(*tinkoffPortfolio);

        const qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

        const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> tinkoffOperations =
            mGrpcClient->getOperations(QThread::currentThread(), mAccountId, timestamp - DATE_RANGE, timestamp + ONE_DAY, "");

        if (!QThread::currentThread()->isInterruptionRequested() && tinkoffOperations != nullptr)
        {
            const InstrumentSells instrumentSells = handleGetOperationsByCursorResponse(*tinkoffOperations);

            mStocksStorage->readLock();
            const InstrumentsForTrading& instrumentsForTrading = mDecisionMaker->makeDecision(
                QThread::currentThread(),
                QDateTime::currentMSecsSinceEpoch(),
                mConfig,
                instrumentSells,
                portfolio,
                mStocksStorage->getStocks(),
                true,
                false,
                true
            );
            mStocksStorage->readUnlock();

            if (!instrumentsForTrading.isEmpty())
            {
                emit tradeInstruments(instrumentsForTrading);
            }
        }
    }

    qDebug() << "Finish AutoPilotDecisionMakerThread";
}

void AutoPilotDecisionMakerThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void AutoPilotDecisionMakerThread::notifyAboutSell(const QString& instrumentId)
{
    const QWriteLocker lock(mRwMutex);

    mSellNotifications[instrumentId] = QDateTime::currentMSecsSinceEpoch();
}

void AutoPilotDecisionMakerThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

Portfolio AutoPilotDecisionMakerThread::handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio)
{
    Portfolio portfolio;

    QMap<QString, QList<PortfolioItem>> categories; // Instrument type => category

    for (int i = 0; i < tinkoffPortfolio.positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        const QString instrumentId   = QString::fromStdString(position.instrument_uid());
        const QString instrumentType = QString::fromStdString(position.instrument_type());

        PortfolioItem item;

        item.instrumentId = instrumentId;
        item.showPrices   = instrumentId != RUBLE_UID;
        item.available    = quotationToDouble(position.quantity());
        item.price        = item.showPrices ? quotationToFloat(position.current_price()) : 1.0f;
        item.avgPriceFifo = item.showPrices ? quotationToFloat(position.average_position_price_fifo()) : 1.0f;
        item.avgPriceWavg = item.showPrices ? quotationToFloat(position.average_position_price()) : 1.0f;
        item.cost         = item.available * item.avgPriceFifo;

        categories[instrumentType].append(item);
    }

    for (auto it = categories.constBegin(); it != categories.constEnd(); ++it)
    {
        PortfolioCategoryItem categoryItem;

        categoryItem.name  = it.key();
        categoryItem.items = it.value();

        portfolio.positions.append(categoryItem);
    }

    return portfolio;
}

InstrumentSells
AutoPilotDecisionMakerThread::handleGetOperationsByCursorResponse(const tinkoff::GetOperationsByCursorResponse& tinkoffOperations)
{
    InstrumentSells res;

    for (int i = tinkoffOperations.items_size() - 1; i >= 0; --i)
    {
        const tinkoff::OperationItem& tinkoffOperation = tinkoffOperations.items(i);

        if (tinkoffOperation.type() == tinkoff::OPERATION_TYPE_SELL)
        {
            const QString instrumentId = QString::fromStdString(tinkoffOperation.instrument_uid());
            const qint64  timestamp    = timeToTimestamp(tinkoffOperation.date());

            res[instrumentId] = timestamp;
        }
    }

    return mergeInstrumentSells(res);
}

InstrumentSells AutoPilotDecisionMakerThread::mergeInstrumentSells(InstrumentSells instrumentsFromOperations)
{
    const QReadLocker lock(mRwMutex);

    for (auto it = mSellNotifications.constBegin(), end = mSellNotifications.constEnd(); it != end; ++it)
    {
        const QString& instrumentId = it.key();
        const qint64   timestamp    = it.value();

        const qint64 timestampFromOperations = instrumentsFromOperations.value(instrumentId);

        if (timestamp > timestampFromOperations)
        {
            instrumentsFromOperations[instrumentId] = timestamp;
        }
    }

    return instrumentsFromOperations;
}
