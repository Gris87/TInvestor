#include "src/threads/autopilotdecisionmaker/autopilotdecisionmakerthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



AutoPilotDecisionMakerThread::AutoPilotDecisionMakerThread(
    IStocksStorage* stocksStorage, IConfig* config, IDecisionMaker* decisionMaker, IGrpcClient* grpcClient, QObject* parent
) :
    IAutoPilotDecisionMakerThread(parent),
    mMutex(new QMutex()),
    mStocksStorage(stocksStorage),
    mConfig(config),
    mDecisionMaker(decisionMaker),
    mGrpcClient(grpcClient),
    mAccountId(),
    mKeepMoney()
{
    qDebug() << "Create AutoPilotDecisionMakerThread";
}

AutoPilotDecisionMakerThread::~AutoPilotDecisionMakerThread()
{
    qDebug() << "Destroy AutoPilotDecisionMakerThread";

    delete mMutex;
}

void AutoPilotDecisionMakerThread::run()
{
    qDebug() << "Running AutoPilotDecisionMakerThread";

    blockSignals(false);

    const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
        mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
    {
        const Portfolio portfolio = handlePortfolioResponse(*tinkoffPortfolio);

        mStocksStorage->readLock();
        const InstrumentsForTrading& instrumentsForTrading = mDecisionMaker->makeDecision(
            QDateTime::currentMSecsSinceEpoch(), mConfig, portfolio, mStocksStorage->getStocks(), true, keepMoney(), false
        );
        mStocksStorage->readUnlock();

        if (!instrumentsForTrading.isEmpty())
        {
            emit tradeInstruments(instrumentsForTrading);
        }
    }

    qDebug() << "Finish AutoPilotDecisionMakerThread";
}

void AutoPilotDecisionMakerThread::setAccountId(const QString& accountId)
{
    const QMutexLocker lock(mMutex);

    mAccountId = accountId;
}

void AutoPilotDecisionMakerThread::setKeepMoney(int value)
{
    const QMutexLocker lock(mMutex);

    mKeepMoney = value;
}

int AutoPilotDecisionMakerThread::keepMoney() const
{
    const QMutexLocker lock(mMutex);

    return mKeepMoney;
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
