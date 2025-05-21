#include "src/threads/operations/operationsthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID       = "a92e2e25-a698-45cc-a781-167cf465257c";
constexpr float   HUNDRED_PERCENT = 100.0f;
constexpr qint64  MS_IN_SECOND    = 1000LL;
constexpr qint64  ONE_MINUTE      = 60LL * MS_IN_SECOND;
constexpr qint64  ONE_HOUR        = 60LL * ONE_MINUTE;
constexpr qint64  ONE_DAY         = 24LL * ONE_HOUR;



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
    mLastOperationTimestamp(),
    mAmountOfOperationsWithSameTimestamp(),
    mLastPositionUidForExtAccount(),
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

    mLastOperationTimestamp              = 0;
    mAmountOfOperationsWithSameTimestamp = 0;
    mLastPositionUidForExtAccount        = "";

    mInstruments.clear();

    for (const Operation& operation : operations)
    {
        QuantityAndCost& quantityAndCost = mInstruments[operation.instrumentId]; // clazy:exclude=detaching-member

        quantityAndCost.quantity = operation.remainedQuantity;
        quantityAndCost.cost     = operation.cost;
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
            res = quotationConvert(position.quantity());

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

    QString                      instrumentId  = QString::fromStdString(tinkoffOperation.instrument_uid());
    const QString                positionUid   = QString::fromStdString(tinkoffOperation.position_uid());
    const qint64                 timestamp     = timeToTimestamp(tinkoffOperation.date());
    const tinkoff::OperationType operationType = tinkoffOperation.type();

    double    avgPrice = 0.0;
    double    avgCost  = 0.0;
    Quotation yield;
    Quotation yieldWithCommission;
    float     yieldWithCommissionPercent = 0.0f;

    if (timestamp == mLastOperationTimestamp)
    {
        ++mAmountOfOperationsWithSameTimestamp;
    }
    else
    {
        mLastOperationTimestamp              = timestamp;
        mAmountOfOperationsWithSameTimestamp = 0;
    }

    QuantityAndCost& quantityAndCost = mInstruments[instrumentId]; // clazy:exclude=detaching-member

    if (operationType == tinkoff::OPERATION_TYPE_BUY)
    {
        quantityAndCost.quantity += tinkoffOperation.quantity();
        quantityAndCost.cost =
            quotationDiff(quantityAndCost.cost, tinkoffOperation.payment()); // Diff == Sum with negative payment

        avgPrice = quotationToDouble(quantityAndCost.cost) / quantityAndCost.quantity;
        avgCost  = -quotationToDouble(tinkoffOperation.payment());

        yieldWithCommission        = quotationConvert(tinkoffOperation.commission());
        yieldWithCommissionPercent = quotationToFloat(yieldWithCommission) / avgCost * HUNDRED_PERCENT;
    }
    else if (operationType == tinkoff::OPERATION_TYPE_SELL)
    {
        avgPrice = quotationToDouble(quantityAndCost.cost) / quantityAndCost.quantity;
        avgCost  = avgPrice * tinkoffOperation.quantity();

        quantityAndCost.quantity -= tinkoffOperation.quantity();

        if (quantityAndCost.quantity > 0)
        {
            quantityAndCost.cost = quotationDiff(quantityAndCost.cost, quotationFromDouble(avgCost));
        }
        else
        {
            quantityAndCost.cost = Quotation();
        }

        yield                      = quotationDiff(tinkoffOperation.payment(), quotationFromDouble(avgCost));
        yieldWithCommission        = quotationSum(yield, tinkoffOperation.commission());
        yieldWithCommissionPercent = quotationToFloat(yieldWithCommission) / avgCost * HUNDRED_PERCENT;
    }

    if (!isOperationTypeWithExtAccount(operationType, positionUid))
    {
        mRemainedMoney = quotationSum(quotationSum(mRemainedMoney, tinkoffOperation.payment()), tinkoffOperation.commission());

        if (operationType == tinkoff::OPERATION_TYPE_BUY || operationType == tinkoff::OPERATION_TYPE_SELL)
        {
            mTotalMoney = quotationSum(mTotalMoney, yieldWithCommission);
        }
        else
        {
            // Real server sends empty instrument_uid for some operations with ruble
            if (instrumentId == "")
            {
                instrumentId = RUBLE_UID;
            }

            mTotalMoney = quotationSum(mTotalMoney, tinkoffOperation.payment());
        }
    }
    else
    {
        mLastPositionUidForExtAccount = positionUid;
    }

    res.timestamp                  = timestamp + mAmountOfOperationsWithSameTimestamp;
    res.instrumentId               = instrumentId;
    res.description                = QString::fromStdString(tinkoffOperation.description());
    res.price                      = quotationToFloat(tinkoffOperation.price());
    res.avgPrice                   = avgPrice;
    res.quantity                   = tinkoffOperation.quantity();
    res.remainedQuantity           = quantityAndCost.quantity;
    res.payment                    = quotationToFloat(tinkoffOperation.payment());
    res.avgCost                    = avgCost;
    res.cost                       = quantityAndCost.cost;
    res.commission                 = quotationToFloat(tinkoffOperation.commission());
    res.yield                      = quotationToFloat(yield);
    res.yieldWithCommission        = quotationToFloat(yieldWithCommission);
    res.yieldWithCommissionPercent = yieldWithCommissionPercent;
    res.remainedMoney              = mRemainedMoney;
    res.totalMoney                 = mTotalMoney;
    res.paymentPrecision           = quotationPrecision(tinkoffOperation.payment());
    res.commissionPrecision        = quotationPrecision(tinkoffOperation.commission());

    return res;
}

bool OperationsThread::isOperationTypeWithExtAccount(tinkoff::OperationType operationType, const QString& positionUid) const
{
    return operationType == tinkoff::OPERATION_TYPE_DIV_EXT ||
           (operationType == tinkoff::OPERATION_TYPE_DIVIDEND_TAX && positionUid == mLastPositionUidForExtAccount);
}
