#include "src/threads/operations/operationsthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID    = "a92e2e25-a698-45cc-a781-167cf465257c";
constexpr qint64  MS_IN_SECOND = 1000LL;
constexpr qint64  ONE_MINUTE   = 60LL * MS_IN_SECOND;
constexpr qint64  ONE_HOUR     = 60LL * ONE_MINUTE;
constexpr qint64  ONE_DAY      = 24LL * ONE_HOUR;



OperationsThread::OperationsThread(IUserStorage* userStorage, IGrpcClient* grpcClient, QObject* parent) :
    IOperationsThread(parent),
    mUserStorage(userStorage),
    mGrpcClient(grpcClient),
    mAccountId(),
    mLastRequestTimestamp()
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
        mLastRequestTimestamp = timeToTimestamp(allTinkoffOperations.first()->items(0).date()) + MS_IN_SECOND;

        qInfo() << "=======================================";
        qInfo() << allTinkoffOperations.size();
        qInfo() << "=======================================";

        for (int i = allTinkoffOperations.size() - 1; i >= 0; --i)
        {
            const std::shared_ptr<tinkoff::GetOperationsByCursorResponse>& tinkoffOperations = allTinkoffOperations.at(i);

            qInfo() << "---------------------------------------";
            qInfo() << tinkoffOperations->items_size();
            qInfo() << "---------------------------------------";

            for (int j = tinkoffOperations->items_size() - 1; j >= 0; --j)
            {
                const tinkoff::OperationItem& operation = tinkoffOperations->items(j);

                qInfo() << QString::fromStdString(operation.cursor());
                qInfo() << QString::fromStdString(operation.broker_account_id());
                qInfo() << QString::fromStdString(operation.id());
                qInfo() << QString::fromStdString(operation.parent_operation_id());
                qInfo() << QString::fromStdString(operation.name());
                qInfo() << operation.date().seconds();
                qInfo() << operation.type();
                qInfo() << QString::fromStdString(operation.description());
                qInfo() << operation.state();
                qInfo() << QString::fromStdString(operation.instrument_uid());
                qInfo() << QString::fromStdString(operation.figi());
                qInfo() << QString::fromStdString(operation.instrument_type());
                qInfo() << operation.instrument_kind();
                qInfo() << QString::fromStdString(operation.position_uid());
                qInfo() << QString::fromStdString(operation.payment().currency());
                qInfo() << operation.payment().units();
                qInfo() << operation.payment().nano();
                qInfo() << QString::fromStdString(operation.price().currency());
                qInfo() << operation.price().units();
                qInfo() << operation.price().nano();
                qInfo() << QString::fromStdString(operation.commission().currency());
                qInfo() << operation.commission().units();
                qInfo() << operation.commission().nano();
                qInfo() << QString::fromStdString(operation.yield().currency());
                qInfo() << operation.yield().units();
                qInfo() << operation.yield().nano();
                qInfo() << operation.yield_relative().units();
                qInfo() << operation.yield_relative().nano();
                qInfo() << QString::fromStdString(operation.accrued_int().currency());
                qInfo() << operation.accrued_int().units();
                qInfo() << operation.accrued_int().nano();
                qInfo() << operation.quantity();
                qInfo() << operation.quantity_rest();
                qInfo() << operation.quantity_done();
                qInfo() << operation.cancel_date_time().seconds();
                qInfo() << QString::fromStdString(operation.cancel_reason());
                qInfo() << QString::fromStdString(operation.asset_uid());
                qInfo() << ".......................................";
            }
        }
    }
}
