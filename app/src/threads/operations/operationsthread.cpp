#include "src/threads/operations/operationsthread.h"

#include <QDebug>



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



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

        if (tinkoffPortfolio != nullptr && !QThread::currentThread()->isInterruptionRequested())
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
                    Quotation newMoney = handlePortfolioResponse(portfolioStreamResponse->portfolio());

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
    const qint64 currentTimestamp = QDateTime::currentMSecsSinceEpoch();

    const std::shared_ptr<tinkoff::OperationsResponse> tinkoffOperations =
        mGrpcClient->getOperations(QThread::currentThread(), mAccountId, mLastRequestTimestamp, currentTimestamp);

    if (tinkoffOperations != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        mLastRequestTimestamp = currentTimestamp;

        qInfo() << "=======================================";
        qInfo() << tinkoffOperations->operations_size();
        qInfo() << "=======================================";

        for (int i = 0; i < tinkoffOperations->operations_size(); ++i)
        {
            const tinkoff::Operation& operation = tinkoffOperations->operations(i);

            qInfo() << QString::fromStdString(operation.id());
            qInfo() << QString::fromStdString(operation.parent_operation_id());
            qInfo() << QString::fromStdString(operation.currency());
            qInfo() << QString::fromStdString(operation.payment().currency());
            qInfo() << operation.payment().units();
            qInfo() << operation.payment().nano();
            qInfo() << QString::fromStdString(operation.price().currency());
            qInfo() << operation.price().units();
            qInfo() << operation.price().nano();
            qInfo() << operation.state();
            qInfo() << operation.quantity();
            qInfo() << operation.quantity_rest();
            qInfo() << QString::fromStdString(operation.figi());
            qInfo() << QString::fromStdString(operation.instrument_type());
            qInfo() << operation.date().seconds();
            qInfo() << QString::fromStdString(operation.type());
            qInfo() << operation.operation_type();
            qInfo() << QString::fromStdString(operation.asset_uid());
            qInfo() << QString::fromStdString(operation.position_uid());
            qInfo() << QString::fromStdString(operation.instrument_uid());
            qInfo() << "-------------------------------------";
        }
    }
}
