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
    mInstruments(),
    mRemainedMoney(),
    mTotalMoney()
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
        readOperations();

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

void OperationsThread::readOperations()
{
    const QList<Operation> operations = mOperationsDatabase->readOperations();

    if (!operations.isEmpty())
    {
        const Operation& lastOperation = operations.constLast();

        mLastRequestTimestamp = lastOperation.timestamp + MS_IN_SECOND;
        mRemainedMoney        = lastOperation.remainedMoney;
        mTotalMoney           = lastOperation.totalMoney;
    }
    else
    {
        mLastRequestTimestamp = 0;
        mRemainedMoney        = Quotation();
        mTotalMoney           = Quotation();
    }

    emit operationsRead(operations);
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
        QList<Operation> operations;

        for (int i = allTinkoffOperations.size() - 1; i >= 0; --i)
        {
            const std::shared_ptr<tinkoff::GetOperationsByCursorResponse>& tinkoffOperations = allTinkoffOperations.at(i);

            for (int j = tinkoffOperations->items_size() - 1; j >= 0; --j)
            {
                const tinkoff::OperationItem& tinkoffOperation = tinkoffOperations->items(j);

                if (tinkoffOperation.type() != tinkoff::OPERATION_TYPE_BROKER_FEE)
                {
                    operations.append(handleOperationItem(tinkoffOperation));
                }
            }
        }

        if (!operations.isEmpty())
        {
            if (mLastRequestTimestamp == 0)
            {
                mOperationsDatabase->writeOperations(operations);
                emit operationsRead(operations);
            }
            else
            {
                mOperationsDatabase->appendOperations(operations);
                emit operationsAdded(operations);
            }

            mLastRequestTimestamp = operations.constLast().timestamp + MS_IN_SECOND;
        }
    }
}

Operation OperationsThread::handleOperationItem(const tinkoff::OperationItem& tinkoffOperation)
{
    Operation res;

    QString              instrumentId        = QString::fromStdString(tinkoffOperation.instrument_uid());
    QuantityAndAvgPrice& quantityAndAvgPrice = mInstruments[instrumentId]; // clazy:exclude=detaching-member

    if (tinkoffOperation.type() == tinkoff::OPERATION_TYPE_BUY)
    {
        const Quotation totalValue = quotationSum(
            quotationMultiply(quantityAndAvgPrice.avgPrice, quantityAndAvgPrice.quantity),
            quotationMultiply(tinkoffOperation.price(), tinkoffOperation.quantity())
        );

        quantityAndAvgPrice.quantity += tinkoffOperation.quantity();
        quantityAndAvgPrice.avgPrice  = quotationDivide(totalValue, quantityAndAvgPrice.quantity);
    }
    else if (tinkoffOperation.type() == tinkoff::OPERATION_TYPE_SELL)
    {
        quantityAndAvgPrice.quantity -= tinkoffOperation.quantity();
    }

    mRemainedMoney = quotationSum(quotationSum(mRemainedMoney, tinkoffOperation.payment()), tinkoffOperation.commission());

    if (isOperationTypeWithMoney(tinkoffOperation.type()))
    {
        Q_ASSERT_X(
            instrumentId == "" || instrumentId == RUBLE_UID, "OperationsThread::handleOperationItem()", "Expecting for ruble"
        );
        instrumentId = RUBLE_UID; // Real server sends empty instrument_uid

        mTotalMoney = quotationSum(mTotalMoney, tinkoffOperation.payment());
    }
    else
    {
        mTotalMoney = quotationSum(mTotalMoney, tinkoffOperation.commission());
    }

    res.timestamp              = timeToTimestamp(tinkoffOperation.date());
    res.instrumentId           = instrumentId;
    res.description            = QString::fromStdString(tinkoffOperation.description());
    res.price                  = moneyToFloat(tinkoffOperation.price());
    res.avgPrice               = quantityAndAvgPrice.avgPrice;
    res.quantity               = tinkoffOperation.quantity();
    res.remainedQuantity       = quantityAndAvgPrice.quantity;
    res.payment                = moneyToFloat(tinkoffOperation.payment());
    res.commission             = moneyToFloat(tinkoffOperation.commission());
    res.yield                  = moneyToFloat(tinkoffOperation.yield());
    res.remainedMoney          = mRemainedMoney;
    res.totalMoney             = mTotalMoney;
    res.pricePrecision         = moneyPrecision(tinkoffOperation.price());
    res.paymentPrecision       = moneyPrecision(tinkoffOperation.payment());
    res.commissionPrecision    = moneyPrecision(tinkoffOperation.commission());
    res.yieldPrecision         = moneyPrecision(tinkoffOperation.yield());
    res.remainedMoneyPrecision = quotationPrecision(mRemainedMoney);
    res.totalMoneyPrecision    = quotationPrecision(mTotalMoney);

    return res;
}

bool OperationsThread::isOperationTypeWithMoney(tinkoff::OperationType operationType) const
{
    return operationType == tinkoff::OPERATION_TYPE_INPUT || operationType == tinkoff::OPERATION_TYPE_OUTPUT ||
           operationType == tinkoff::OPERATION_TYPE_TAX || operationType == tinkoff::OPERATION_TYPE_TAX_CORRECTION ||
           operationType == tinkoff::OPERATION_TYPE_TRACK_MFEE;
}
