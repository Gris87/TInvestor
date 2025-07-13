#include "src/threads/autopilotmakedecision/autopilotmakedecisionthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



AutoPilotMakeDecisionThread::AutoPilotMakeDecisionThread(
    IStocksStorage* stocksStorage, IDecisionMaker* decisionMaker, IGrpcClient* grpcClient, QObject* parent
) :
    IAutoPilotMakeDecisionThread(parent),
    mMutex(new QMutex()),
    mStocksStorage(stocksStorage),
    mDecisionMaker(decisionMaker),
    mGrpcClient(grpcClient),
    mAccountId(),
    mKeepMoney()
{
    qDebug() << "Create AutoPilotMakeDecisionThread";
}

AutoPilotMakeDecisionThread::~AutoPilotMakeDecisionThread()
{
    qDebug() << "Destroy AutoPilotMakeDecisionThread";

    delete mMutex;
}

void AutoPilotMakeDecisionThread::run()
{
    qDebug() << "Running AutoPilotMakeDecisionThread";

    blockSignals(false);

    const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
        mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
    {
        const Portfolio portfolio = handlePortfolioResponse(*tinkoffPortfolio);

        mStocksStorage->readLock();
        const InstrumentsForTrading& instrumentsForTrading =
            mDecisionMaker->makeDecision(portfolio, mStocksStorage->getStocks(), keepMoney());
        mStocksStorage->readUnlock();

        if (!instrumentsForTrading.isEmpty())
        {
            emit tradeInstruments(instrumentsForTrading);
        }
    }

    qDebug() << "Finish AutoPilotMakeDecisionThread";
}

void AutoPilotMakeDecisionThread::setAccount(const QString& accountId)
{
    const QMutexLocker lock(mMutex);

    mAccountId = accountId;
}

void AutoPilotMakeDecisionThread::setKeepMoney(int value)
{
    const QMutexLocker lock(mMutex);

    mKeepMoney = value;
}

QString AutoPilotMakeDecisionThread::accountId() const
{
    const QMutexLocker lock(mMutex);

    return mAccountId;
}

int AutoPilotMakeDecisionThread::keepMoney() const
{
    const QMutexLocker lock(mMutex);

    return mKeepMoney;
}

void AutoPilotMakeDecisionThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

Portfolio AutoPilotMakeDecisionThread::handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio)
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
