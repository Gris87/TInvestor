#include "src/threads/operations/operationsthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID    = "a92e2e25-a698-45cc-a781-167cf465257c";
constexpr qint64  MS_IN_SECOND = 1000LL;
constexpr qint64  ONE_MINUTE   = 60LL * MS_IN_SECOND;
constexpr qint64  ONE_HOUR     = 60LL * ONE_MINUTE;
constexpr qint64  ONE_DAY      = 24LL * ONE_HOUR;



OperationsThread::OperationsThread(
    IUserStorage* userStorage, IOperationsDatabase* operationsDatabase, IGrpcClient* grpcClient, QObject* parent
) :
    IOperationsThread(parent),
    mUserStorage(userStorage),
    mOperationsDatabase(operationsDatabase),
    mGrpcClient(grpcClient),
    mAccountId(),
    mPortfolioStream(),
    mLastRequestTimestamp(),
    mOperations()
{
    qDebug() << "Create OperationsThread";
}

OperationsThread::~OperationsThread()
{
    qDebug() << "Destroy OperationsThread";
}

void OperationsThread::run()
{
    qDebug() << "Running OperationsThread";

    if (mAccountId != "")
    {
        mOperations           = mOperationsDatabase->readOperations();
        mLastRequestTimestamp = !mOperations.isEmpty() ? mOperations.constLast().timestamp + MS_IN_SECOND : 0;

        emit operationsRead(mOperations);

        const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
            mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);

        if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
        {
            Quotation money = handlePortfolioResponse(*tinkoffPortfolio);

            createPortfolioStream();
            requestOperations();

            while (true)
            {
                const std::shared_ptr<tinkoff::PortfolioStreamResponse> portfolioStreamResponse =
                    mGrpcClient->readPortfolioStream(mPortfolioStream);

                if (QThread::currentThread()->isInterruptionRequested() || portfolioStreamResponse == nullptr)
                {
                    break;
                }

                if (portfolioStreamResponse->has_portfolio())
                {
                    const Quotation newMoney = handlePortfolioResponse(portfolioStreamResponse->portfolio());

                    if (money != newMoney)
                    {
                        money = newMoney;

                        requestOperations();
                    }
                }
            }

            mGrpcClient->finishPortfolioStream(mPortfolioStream);
            mPortfolioStream = nullptr;
        }
    }
    else
    {
        emit accountNotFound();
    }

    qDebug() << "Finish OperationsThread";
}

void OperationsThread::setAccount(const QString& account)
{
    mOperationsDatabase->setAccount(account);

    const QMutexLocker lock(mUserStorage->getMutex());
    Accounts           accounts = mUserStorage->getAccounts();

    mAccountId = accounts[account].id;
}

void OperationsThread::terminateThread()
{
    if (mPortfolioStream != nullptr)
    {
        mGrpcClient->cancelPortfolioStream(mPortfolioStream);
    }

    requestInterruption();
}

void OperationsThread::createPortfolioStream()
{
    mPortfolioStream = mGrpcClient->createPortfolioStream(mAccountId);
}

Quotation OperationsThread::handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio)
{
    Quotation res;

    for (int i = 0; i < tinkoffPortfolio.positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        if (position.instrument_uid() == RUBLE_UID)
        {
            res.units = position.quantity().units();
            res.nano  = position.quantity().nano();

            break;
        }
    }

    return res;
}

void OperationsThread::requestOperations()
{
    const qint64 endTimestamp = QDateTime::currentMSecsSinceEpoch() + ONE_DAY;
    QString      cursor;

    QList<std::shared_ptr<tinkoff::GetOperationsByCursorResponse>> allTinkoffOperations;

    while (true)
    {
        const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> tinkoffOperations =
            mGrpcClient->getOperations(QThread::currentThread(), mAccountId, mLastRequestTimestamp, endTimestamp, cursor);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffOperations == nullptr)
        {
            return;
        }

        if (tinkoffOperations->items_size() > 0)
        {
            allTinkoffOperations.append(tinkoffOperations);
        }

        if (!tinkoffOperations->has_next())
        {
            break;
        }

        cursor = QString::fromStdString(tinkoffOperations->next_cursor());
    }

    if (!QThread::currentThread()->isInterruptionRequested() && !allTinkoffOperations.isEmpty())
    {
        const int lastIndex = mOperations.size();

        for (int i = allTinkoffOperations.size() - 1; i >= 0; --i)
        {
            const std::shared_ptr<tinkoff::GetOperationsByCursorResponse>& tinkoffOperations = allTinkoffOperations.at(i);

            for (int j = tinkoffOperations->items_size() - 1; j >= 0; --j)
            {
                const tinkoff::OperationItem& tinkoffOperation = tinkoffOperations->items(j);
                Operation                     operation;

                operation.timestamp    = timeToTimestamp(tinkoffOperation.date());
                operation.instrumentId = QString::fromStdString(tinkoffOperation.instrument_uid());
                operation.description  = QString::fromStdString(tinkoffOperation.description());
                operation.price        = moneyToFloat(tinkoffOperation.price());
                operation.quantity     = tinkoffOperation.quantity();
                operation.payment      = moneyToFloat(tinkoffOperation.payment());
                operation.commission   = moneyToFloat(tinkoffOperation.commission());
                operation.yield        = moneyToFloat(tinkoffOperation.yield());

                mOperations.append(operation);
            }
        }

        if (lastIndex == 0)
        {
            mOperationsDatabase->writeOperations(mOperations);

            emit operationsRead(mOperations);
        }
        else
        {
            const QList<Operation> newOperations = mOperations.mid(lastIndex);

            mOperationsDatabase->appendOperations(newOperations);
            emit operationsAdded(newOperations);
        }

        mLastRequestTimestamp = mOperations.constLast().timestamp + MS_IN_SECOND;
    }
}
