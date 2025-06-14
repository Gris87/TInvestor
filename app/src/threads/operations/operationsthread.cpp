#include "src/threads/operations/operationsthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID       = "a92e2e25-a698-45cc-a781-167cf465257c";
constexpr float   HUNDRED_PERCENT = 100.0f;
constexpr qint64  MS_IN_SECOND    = 1000LL;
constexpr qint64  ONE_MINUTE      = 60LL * MS_IN_SECOND;
constexpr qint64  ONE_HOUR        = 60LL * ONE_MINUTE;
constexpr qint64  ONE_DAY         = 24LL * ONE_HOUR;



OperationsThread::OperationsThread(IOperationsDatabase* operationsDatabase, IGrpcClient* grpcClient, QObject* parent) :
    IOperationsThread(parent),
    mOperationsDatabase(operationsDatabase),
    mGrpcClient(grpcClient),
    mAccountId(),
    mPositionsStream(),
    mLastRequestTimestamp(),
    mLastOperationTimestamp(),
    mAmountOfOperationsWithSameTimestamp(),
    mLastPositionUidForExtAccount(),
    mInstruments(),
    mInputMoney(),
    mMaxInputMoney(),
    mTotalYieldWithCommission(),
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

    blockSignals(false);
    readOperations();

    const std::shared_ptr<tinkoff::PositionsResponse> tinkoffPositions =
        mGrpcClient->getPositions(QThread::currentThread(), mAccountId);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPositions != nullptr)
    {
        Quotation money = handlePositionsResponse(*tinkoffPositions);

        createPositionsStream();
        requestOperations();

        while (true)
        {
            const std::shared_ptr<tinkoff::PositionsStreamResponse> positionsStreamResponse =
                mGrpcClient->readPositionsStream(mPositionsStream);

            if (QThread::currentThread()->isInterruptionRequested() || positionsStreamResponse == nullptr)
            {
                break;
            }

            if (positionsStreamResponse->has_position())
            {
                const Quotation newMoney = handlePositionsResponse(positionsStreamResponse->position());

                if (money != newMoney)
                {
                    money = newMoney;

                    requestOperations();
                }
            }
        }

        mGrpcClient->finishPositionsStream(mPositionsStream);
        mPositionsStream = nullptr;
    }

    qDebug() << "Finish OperationsThread";
}

void OperationsThread::setAccountId(const QString& account, const QString& accountId)
{
    mOperationsDatabase->setAccount(account);

    mAccountId = accountId;
}

void OperationsThread::terminateThread()
{
    blockSignals(true);

    if (mPositionsStream != nullptr)
    {
        mGrpcClient->cancelPositionsStream(mPositionsStream);
    }

    requestInterruption();
}

void OperationsThread::createPositionsStream()
{
    mPositionsStream = mGrpcClient->createPositionsStream(mAccountId);
}

void OperationsThread::readOperations()
{
    const QList<Operation> operations = mOperationsDatabase->readOperations();

    if (!operations.isEmpty())
    {
        const Operation& lastOperation = operations.constLast();

        mLastRequestTimestamp     = lastOperation.timestamp + MS_IN_SECOND;
        mInputMoney               = lastOperation.inputMoney;
        mMaxInputMoney            = lastOperation.maxInputMoney;
        mTotalYieldWithCommission = lastOperation.totalYieldWithCommission;
        mRemainedMoney            = lastOperation.remainedMoney;
        mTotalMoney               = lastOperation.totalMoney;
    }
    else
    {
        mLastRequestTimestamp     = 0;
        mInputMoney               = Quotation();
        mMaxInputMoney            = Quotation();
        mTotalYieldWithCommission = Quotation();
        mRemainedMoney            = Quotation();
        mTotalMoney               = Quotation();
    }

    mLastOperationTimestamp              = 0;
    mAmountOfOperationsWithSameTimestamp = 0;
    mLastPositionUidForExtAccount        = "";

    mInstruments.clear();

    for (const Operation& operation : operations)
    {
        if (operation.remainedQuantity > 0)
        {
            QuantityAndCost& quantityAndCost = mInstruments[operation.instrumentId]; // clazy:exclude=detaching-member

            quantityAndCost.quantity  = operation.remainedQuantity;
            quantityAndCost.fifoItems = operation.fifoItems;
            quantityAndCost.costFifo  = operation.costFifo;
            quantityAndCost.costWavg  = operation.costWavg;
        }
        else
        {
            mInstruments.remove(operation.instrumentId);
        }
    }

    emit operationsRead(operations);
}

Quotation OperationsThread::handlePositionsResponse(const tinkoff::PositionsResponse& tinkoffPositions)
{
    Quotation res;

    for (int i = 0; i < tinkoffPositions.money_size(); ++i)
    {
        const tinkoff::MoneyValue& money = tinkoffPositions.money(i);

        if (money.currency() == "rub")
        {
            res = quotationConvert(money);

            break;
        }
    }

    return res;
}

Quotation OperationsThread::handlePositionsResponse(const tinkoff::PositionData& tinkoffPositions)
{
    Quotation res;

    for (int i = 0; i < tinkoffPositions.money_size(); ++i)
    {
        const tinkoff::MoneyValue& money = tinkoffPositions.money(i).available_value();

        if (money.currency() == "rub")
        {
            res = quotationConvert(money);

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

// NOLINTBEGIN(readability-function-cognitive-complexity)
Operation OperationsThread::handleOperationItem(const tinkoff::OperationItem& tinkoffOperation)
{
    Operation res;

    QString                      instrumentId  = QString::fromStdString(tinkoffOperation.instrument_uid());
    const QString                positionUid   = QString::fromStdString(tinkoffOperation.position_uid());
    const qint64                 timestamp     = timeToTimestamp(tinkoffOperation.date());
    const tinkoff::OperationType operationType = tinkoffOperation.type();
    const double                 payment       = quotationToDouble(tinkoffOperation.payment());

    double    avgPriceFifo = 0.0;
    double    avgPriceWavg = 0.0;
    double    avgCostFifo  = 0.0;
    Quotation yield;
    Quotation yieldWithCommission;
    float     yieldWithCommissionPercent      = 0.0f;
    float     totalYieldWithCommissionPercent = 0.0f;

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
        OperationFifoItem fifoItem;

        fifoItem.quantity = tinkoffOperation.quantity_done();
        fifoItem.cost     = quotationNegative(tinkoffOperation.payment());

        quantityAndCost.fifoItems.append(fifoItem);

        quantityAndCost.quantity += fifoItem.quantity;
        quantityAndCost.costFifo  = quotationSum(quantityAndCost.costFifo, fifoItem.cost);
        quantityAndCost.costWavg  = quotationSum(quantityAndCost.costWavg, fifoItem.cost);

        avgPriceFifo = quotationToDouble(quantityAndCost.costFifo) / quantityAndCost.quantity;
        avgPriceWavg = quotationToDouble(quantityAndCost.costWavg) / quantityAndCost.quantity;
        avgCostFifo  = -payment;

        yieldWithCommission        = quotationConvert(tinkoffOperation.commission());
        yieldWithCommissionPercent = quotationToDouble(yieldWithCommission) / avgCostFifo * HUNDRED_PERCENT;

        mTotalYieldWithCommission = quotationSum(mTotalYieldWithCommission, yieldWithCommission);
    }
    else if (operationType == tinkoff::OPERATION_TYPE_SELL)
    {
        avgPriceWavg = quotationToDouble(quantityAndCost.costWavg) / quantityAndCost.quantity;

        Quotation avgCostFifoQuotation;

        if (quantityAndCost.quantity > tinkoffOperation.quantity_done())
        {
            const double avgCostWavg = avgPriceWavg * tinkoffOperation.quantity_done();

            qint64 quantityForCalculation = tinkoffOperation.quantity_done();
            int    fifoIndex              = 0;

            while (quantityForCalculation > 0)
            {
                OperationFifoItem& fifoItem = quantityAndCost.fifoItems[fifoIndex]; // clazy:exclude=detaching-member

                if (quantityForCalculation >= fifoItem.quantity)
                {
                    avgCostFifoQuotation    = quotationSum(avgCostFifoQuotation, fifoItem.cost);
                    quantityForCalculation -= fifoItem.quantity;

                    ++fifoIndex;
                }
                else
                {
                    const Quotation deltaCost =
                        quotationDivide(quotationMultiply(fifoItem.cost, quantityForCalculation), fifoItem.quantity);

                    avgCostFifoQuotation = quotationSum(avgCostFifoQuotation, deltaCost);
                    fifoItem.cost        = quotationDiff(fifoItem.cost, deltaCost);

                    fifoItem.quantity -= quantityForCalculation;

                    break;
                }
            }

            quantityAndCost.fifoItems.remove(0, fifoIndex);

            quantityAndCost.quantity -= tinkoffOperation.quantity_done();
            quantityAndCost.costFifo  = quotationDiff(quantityAndCost.costFifo, avgCostFifoQuotation);
            quantityAndCost.costWavg  = quotationDiff(quantityAndCost.costWavg, quotationFromDouble(avgCostWavg));

            avgPriceFifo = quotationToDouble(quantityAndCost.costFifo) / quantityAndCost.quantity;
        }
        else
        {
            avgPriceFifo         = quotationToDouble(quantityAndCost.costFifo) / quantityAndCost.quantity;
            avgCostFifoQuotation = quantityAndCost.costFifo;

            quantityAndCost.quantity = 0;
            quantityAndCost.fifoItems.clear();
            quantityAndCost.costFifo = Quotation();
            quantityAndCost.costWavg = Quotation();
        }

        avgCostFifo = quotationToDouble(avgCostFifoQuotation);

        yield                      = quotationDiff(tinkoffOperation.payment(), avgCostFifoQuotation);
        yieldWithCommission        = quotationSum(yield, tinkoffOperation.commission());
        yieldWithCommissionPercent = quotationToDouble(yieldWithCommission) / avgCostFifo * HUNDRED_PERCENT;

        mTotalYieldWithCommission = quotationSum(mTotalYieldWithCommission, yieldWithCommission);
    }

    if (!isOperationTypeWithExtAccount(operationType, positionUid))
    {
        mRemainedMoney = quotationSum(quotationSum(mRemainedMoney, tinkoffOperation.payment()), tinkoffOperation.commission());

        if (operationType == tinkoff::OPERATION_TYPE_BUY || operationType == tinkoff::OPERATION_TYPE_SELL)
        {
            mTotalMoney = quotationSum(mTotalMoney, yieldWithCommission);
        }
        else if (operationType == tinkoff::OPERATION_TYPE_BOND_REPAYMENT_FULL)
        {
            mTotalMoney = quotationDiff(quotationSum(mTotalMoney, tinkoffOperation.payment()), quantityAndCost.costFifo);

            quantityAndCost.quantity = 0;
            quantityAndCost.fifoItems.clear();
            quantityAndCost.costFifo = Quotation();
            quantityAndCost.costWavg = Quotation();
        }
        else
        {
            // Real server sends empty instrument_uid for some operations with ruble
            if (instrumentId == "")
            {
                instrumentId = RUBLE_UID;
            }

            if (operationType == tinkoff::OPERATION_TYPE_INPUT)
            {
                mInputMoney = quotationSum(mInputMoney, tinkoffOperation.payment());

                if (mInputMoney > mMaxInputMoney)
                {
                    mMaxInputMoney = mInputMoney;
                }
            }
            else if (operationType == tinkoff::OPERATION_TYPE_OUTPUT)
            {
                mInputMoney = quotationSum(mInputMoney, tinkoffOperation.payment());
            }

            mTotalMoney = quotationSum(mTotalMoney, tinkoffOperation.payment());
        }
    }
    else
    {
        mLastPositionUidForExtAccount = positionUid;
    }

    if (mMaxInputMoney.units != 0 || mMaxInputMoney.nano != 0)
    {
        totalYieldWithCommissionPercent =
            quotationToDouble(mTotalYieldWithCommission) / quotationToDouble(mMaxInputMoney) * HUNDRED_PERCENT;
    }

    res.timestamp                       = timestamp + mAmountOfOperationsWithSameTimestamp;
    res.instrumentId                    = instrumentId;
    res.description                     = QString::fromStdString(tinkoffOperation.description());
    res.price                           = quotationToFloat(tinkoffOperation.price());
    res.fifoItems                       = quantityAndCost.fifoItems;
    res.avgPriceFifo                    = avgPriceFifo;
    res.avgPriceWavg                    = avgPriceWavg;
    res.quantity                        = tinkoffOperation.quantity_done();
    res.remainedQuantity                = quantityAndCost.quantity;
    res.payment                         = payment;
    res.avgCostFifo                     = avgCostFifo;
    res.costFifo                        = quantityAndCost.costFifo;
    res.costWavg                        = quantityAndCost.costWavg;
    res.commission                      = quotationToFloat(tinkoffOperation.commission());
    res.yield                           = quotationToFloat(yield);
    res.yieldWithCommission             = quotationToFloat(yieldWithCommission);
    res.yieldWithCommissionPercent      = yieldWithCommissionPercent;
    res.inputMoney                      = mInputMoney;
    res.maxInputMoney                   = mMaxInputMoney;
    res.totalYieldWithCommission        = mTotalYieldWithCommission;
    res.totalYieldWithCommissionPercent = totalYieldWithCommissionPercent;
    res.remainedMoney                   = mRemainedMoney;
    res.totalMoney                      = mTotalMoney;
    res.paymentPrecision                = quotationPrecision(tinkoffOperation.payment());
    res.commissionPrecision             = quotationPrecision(tinkoffOperation.commission());

    if (quantityAndCost.quantity <= 0)
    {
        mInstruments.remove(instrumentId);
    }

    return res;
}
// NOLINTEND(readability-function-cognitive-complexity)

bool OperationsThread::isOperationTypeWithExtAccount(tinkoff::OperationType operationType, const QString& positionUid) const
{
    return operationType == tinkoff::OPERATION_TYPE_DIV_EXT ||
           (operationType == tinkoff::OPERATION_TYPE_DIVIDEND_TAX && positionUid == mLastPositionUidForExtAccount);
}
