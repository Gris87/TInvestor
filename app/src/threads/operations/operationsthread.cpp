#include "src/threads/operations/operationsthread.h"

#include <QDebug>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr int    LIMIT_OPERATIONS     = 100000;
constexpr int    OPTIMIZE_SIZE        = 10000;
constexpr float  HUNDRED_PERCENT      = 100.0f;
constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 ONE_MINUTE           = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR             = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY              = 24LL * ONE_HOUR;
constexpr qint64 SLEEP_DELAY          = 5LL * MS_IN_SECOND;  // 5 seconds
constexpr qint64 SLEEP_BEFORE_REQUEST = 10LL * MS_IN_SECOND; // 10 seconds



OperationsThread::OperationsThread(
    IOperationsDatabase* operationsDatabase,
    IInstrumentsStorage* instrumentsStorage,
    ILogosStorage*       logosStorage,
    ITimeUtils*          timeUtils,
    IGrpcClient*         grpcClient,
    IGrpcRetryClient*    grpcRetryClient,
    IOptimizer*          optimizer,
    QObject*             parent
) :
    IOperationsThread(parent),
    mRwMutex(new QReadWriteLock()),
    mOperationsDatabase(operationsDatabase),
    mInstrumentsStorage(instrumentsStorage),
    mLogosStorage(logosStorage),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mGrpcRetryClient(grpcRetryClient),
    mOptimizer(optimizer),
    mAccountId(),
    mPortfolioStream(),
    mLastRequestTimestamp(),
    mLastOperationTimestamp(),
    mAmountOfOperationsWithSameTimestamp(),
    mAmountOfEntries(),
    mLimitOperations(LIMIT_OPERATIONS),
    mOptimizeSize(OPTIMIZE_SIZE),
    mLastPositionUidForExtAccount(),
    mOperationsLastDay(),
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

    delete mRwMutex;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void OperationsThread::run()
{
    qDebug() << "Running OperationsThread";

    blockSignals(false);
    readOperations();

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        if (createPortfolioStream())
        {
            if (requestOperations())
            {
                while (true)
                {
                    optimize();

                    const std::shared_ptr<tinkoff::PortfolioStreamResponse> portfolioStreamResponse =
                        mGrpcClient->readPortfolioStream(mPortfolioStream);

                    if (QThread::currentThread()->isInterruptionRequested() || portfolioStreamResponse == nullptr)
                    {
                        mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread());

                        break;
                    }

                    if (portfolioStreamResponse->has_portfolio())
                    {
                        if (mTimeUtils->interruptibleSleep(SLEEP_BEFORE_REQUEST, QThread::currentThread()))
                        {
                            break;
                        }

                        requestOperations();
                    }
                }
            }
            else
            {
                if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread()))
                {
                    const QWriteLocker lock(mRwMutex);

                    mGrpcClient->finishPortfolioStream(mPortfolioStream);
                    mPortfolioStream = nullptr;

                    break;
                }
            }

            const QWriteLocker lock(mRwMutex);

            mGrpcClient->finishPortfolioStream(mPortfolioStream);
            mPortfolioStream = nullptr;
        }
        else
        {
            if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread()))
            {
                break;
            }
        }
    }

    qDebug() << "Finish OperationsThread";
}
// NOLINTEND(readability-function-cognitive-complexity)

void OperationsThread::setAccountId(const QString& account, const QString& accountId)
{
    mOperationsDatabase->setAccount(account);

    mAccountId = accountId;
}

void OperationsThread::terminateThread()
{
    blockSignals(true);

    const QReadLocker lock(mRwMutex);

    if (mPortfolioStream != nullptr)
    {
        mGrpcClient->cancelPortfolioStream(mPortfolioStream);
    }

    requestInterruption();
}

bool OperationsThread::createPortfolioStream()
{
    bool res = false;

    const QWriteLocker lock(mRwMutex);

    if (!QThread::currentThread()->isInterruptionRequested())
    {
        mPortfolioStream = mGrpcClient->createPortfolioStream(mAccountId);

        res = mPortfolioStream != nullptr;
    }

    return res;
}

void OperationsThread::readOperations()
{
    const QList<Operation> operations = mOperationsDatabase->readOperations();
    mAmountOfEntries                  = operations.size();

    if (mAmountOfEntries > 0)
    {
        const Operation& lastOperation = operations.constFirst(); // Since it reversed

        mLastRequestTimestamp     = lastOperation.timestamp;
        mLastOperationTimestamp   = lastOperation.timestamp;
        mInputMoney               = lastOperation.inputMoney;
        mMaxInputMoney            = lastOperation.maxInputMoney;
        mTotalYieldWithCommission = lastOperation.totalYieldWithCommission;
        mRemainedMoney            = lastOperation.remainedMoney;
        mTotalMoney               = lastOperation.totalMoney;
    }
    else
    {
        mLastRequestTimestamp     = 0;
        mLastOperationTimestamp   = 0;
        mInputMoney               = Quotation();
        mMaxInputMoney            = Quotation();
        mTotalYieldWithCommission = Quotation();
        mRemainedMoney            = Quotation();
        mTotalMoney               = Quotation();
    }

    mAmountOfOperationsWithSameTimestamp = 0;
    mLastPositionUidForExtAccount        = "";

    mOperationsLastDay.clear();
    mInstruments.clear();

    for (int i = operations.size() - 1; i >= 0; --i)
    {
        const Operation& operation = operations.at(i);

        mOperationsLastDay.insert(
            QString("%1_%2_%3").arg(QString::number(operation.originalTimestamp), operation.instrumentId, operation.description)
        );

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

// NOLINTBEGIN(readability-function-cognitive-complexity)
bool OperationsThread::requestOperations()
{
    const qint64 startTimestamp = qMax(mLastRequestTimestamp - ONE_DAY, 0);
    const qint64 endTimestamp   = QDateTime::currentMSecsSinceEpoch() + ONE_DAY;
    QString      cursor;

    QSet<QString>                                                  operationsLastDay;
    QList<std::shared_ptr<tinkoff::GetOperationsByCursorResponse>> allTinkoffOperations;
    int                                                            totalOperations = 0;

    while (true)
    {
        const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> tinkoffOperations =
            mGrpcRetryClient->getValidOperations(QThread::currentThread(), mAccountId, startTimestamp, endTimestamp, cursor);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffOperations == nullptr)
        {
            return false;
        }

        if (tinkoffOperations->items_size() > 0)
        {
            allTinkoffOperations.append(tinkoffOperations);

            for (int i = tinkoffOperations->items_size() - 1; i >= 0; --i)
            {
                const tinkoff::OperationItem& tinkoffOperation = tinkoffOperations->items(i);

                const tinkoff::OperationType operationType = tinkoffOperation.type();

                if (operationType != tinkoff::OPERATION_TYPE_BROKER_FEE &&
                    ((operationType != tinkoff::OPERATION_TYPE_BUY && operationType != tinkoff::OPERATION_TYPE_SELL) ||
                     (tinkoffOperation.quantity_rest() == 0 || tinkoffOperation.has_cancel_date_time())))
                {
                    QString instrumentId = QString::fromStdString(tinkoffOperation.instrument_uid());

                    if (instrumentId == "")
                    {
                        instrumentId = RUBLE_UID;
                    }

                    const QString operationIdStr = QString("%1_%2_%3")
                                                       .arg(
                                                           QString::number(timeToTimestamp(tinkoffOperation.date())),
                                                           instrumentId,
                                                           QString::fromStdString(tinkoffOperation.description())
                                                       );

                    if (!mOperationsLastDay.contains(operationIdStr))
                    {
                        ++totalOperations;
                    }

                    operationsLastDay.insert(operationIdStr);
                }
            }
        }

        if (!tinkoffOperations->has_next())
        {
            break;
        }

        cursor = QString::fromStdString(tinkoffOperations->next_cursor());
    }

    if (!QThread::currentThread()->isInterruptionRequested() && totalOperations > 0)
    {
        QList<Operation> operations;

        operations.resizeForOverwrite(totalOperations);
        int curOperation = totalOperations - 1;

        for (int i = allTinkoffOperations.size() - 1; i >= 0; --i)
        {
            const std::shared_ptr<tinkoff::GetOperationsByCursorResponse>& tinkoffOperations = allTinkoffOperations.at(i);

            for (int j = tinkoffOperations->items_size() - 1; j >= 0; --j)
            {
                const tinkoff::OperationItem& tinkoffOperation = tinkoffOperations->items(j);

                const tinkoff::OperationType operationType = tinkoffOperation.type();

                if (operationType != tinkoff::OPERATION_TYPE_BROKER_FEE &&
                    ((operationType != tinkoff::OPERATION_TYPE_BUY && operationType != tinkoff::OPERATION_TYPE_SELL) ||
                     (tinkoffOperation.quantity_rest() == 0 || tinkoffOperation.has_cancel_date_time())))
                {
                    QString instrumentId = QString::fromStdString(tinkoffOperation.instrument_uid());

                    if (instrumentId == "")
                    {
                        instrumentId = RUBLE_UID;
                    }

                    const QString operationIdStr = QString("%1_%2_%3")
                                                       .arg(
                                                           QString::number(timeToTimestamp(tinkoffOperation.date())),
                                                           instrumentId,
                                                           QString::fromStdString(tinkoffOperation.description())
                                                       );

                    if (!mOperationsLastDay.contains(operationIdStr))
                    {
                        handleOperationItem(tinkoffOperation, &operations[curOperation]);
                        --curOperation;
                    }
                }
            }
        }

        alignRemainedAndTotalMoneyFromPortfolio(&operations.first()); // Since it reversed

        if (mLastRequestTimestamp == 0)
        {
            emit operationsRead(operations);
            mOperationsDatabase->writeOperations(operations);
        }
        else
        {
            emit operationsAdded(operations);
            mOperationsDatabase->appendOperations(operations);
        }

        mLastRequestTimestamp = operations.constFirst().timestamp; // Since it reversed
        mOperationsLastDay    = operationsLastDay;

        mAmountOfEntries += totalOperations;
    }

    return true;
}
// NOLINTEND(readability-function-cognitive-complexity)

// NOLINTBEGIN(readability-function-cognitive-complexity)
void OperationsThread::handleOperationItem(const tinkoff::OperationItem& tinkoffOperation, Operation* res)
{
    QString                      instrumentId      = QString::fromStdString(tinkoffOperation.instrument_uid());
    const QString                positionUid       = QString::fromStdString(tinkoffOperation.position_uid());
    const qint64                 originalTimestamp = timeToTimestamp(tinkoffOperation.date());
    const tinkoff::OperationType operationType     = tinkoffOperation.type();
    const double                 payment           = quotationToDouble(tinkoffOperation.payment());

    double    avgPriceFifo = 0.0;
    double    avgPriceWavg = 0.0;
    double    avgCostFifo  = 0.0;
    Quotation yield;
    Quotation yieldWithCommission;
    float     yieldWithCommissionPercent      = 0.0f;
    float     totalYieldWithCommissionPercent = 0.0f;

    if (originalTimestamp <= mLastOperationTimestamp)
    {
        ++mAmountOfOperationsWithSameTimestamp;
    }
    else
    {
        mLastOperationTimestamp              = originalTimestamp;
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
    else if (operationType != tinkoff::OPERATION_TYPE_INPUT && operationType != tinkoff::OPERATION_TYPE_OUTPUT)
    {
        avgCostFifo = quotationToDouble(quantityAndCost.costFifo);

        if (quantityAndCost.quantity > 0)
        {
            avgPriceFifo = avgCostFifo / quantityAndCost.quantity;
            avgPriceWavg = quotationToDouble(quantityAndCost.costWavg) / quantityAndCost.quantity;
        }

        if (operationType == tinkoff::OPERATION_TYPE_BOND_REPAYMENT_FULL)
        {
            yield = quotationDiff(tinkoffOperation.payment(), quantityAndCost.costFifo);
        }
        else
        {
            yield = quotationConvert(tinkoffOperation.payment());
        }

        yieldWithCommission = quotationSum(yield, tinkoffOperation.commission());

        if (avgCostFifo > 0)
        {
            yieldWithCommissionPercent = quotationToDouble(yieldWithCommission) / avgCostFifo * HUNDRED_PERCENT;
        }

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
            mTotalMoney = quotationSum(mTotalMoney, yieldWithCommission);

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

    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(instrumentId);
    mInstrumentsStorage->readUnlock();

    instrument.resetIfNotFound(instrumentId);

    if (instrumentId == RUBLE_UID)
    {
        instrument.pricePrecision = 2; // Price precision for ruble is 4 while we want 2
    }

    mLogosStorage->readLock();
    res->instrumentLogo = mLogosStorage->getLogo(instrumentId);
    mLogosStorage->readUnlock();

    res->timestamp                       = mLastOperationTimestamp + mAmountOfOperationsWithSameTimestamp;
    res->originalTimestamp               = originalTimestamp;
    res->instrumentId                    = instrumentId;
    res->instrumentTicker                = instrument.ticker;
    res->instrumentName                  = instrument.name;
    res->description                     = QString::fromStdString(tinkoffOperation.description());
    res->price                           = quotationToFloat(tinkoffOperation.price());
    res->fifoItems                       = quantityAndCost.fifoItems;
    res->avgPriceFifo                    = avgPriceFifo;
    res->avgPriceWavg                    = avgPriceWavg;
    res->quantity                        = tinkoffOperation.quantity_done();
    res->remainedQuantity                = quantityAndCost.quantity;
    res->payment                         = payment;
    res->avgCostFifo                     = avgCostFifo;
    res->costFifo                        = quantityAndCost.costFifo;
    res->costWavg                        = quantityAndCost.costWavg;
    res->commission                      = quotationToFloat(tinkoffOperation.commission());
    res->yield                           = quotationToFloat(yield);
    res->yieldWithCommission             = quotationToFloat(yieldWithCommission);
    res->yieldWithCommissionPercent      = yieldWithCommissionPercent;
    res->inputMoney                      = mInputMoney;
    res->maxInputMoney                   = mMaxInputMoney;
    res->totalYieldWithCommission        = mTotalYieldWithCommission;
    res->totalYieldWithCommissionPercent = totalYieldWithCommissionPercent;
    res->remainedMoney                   = mRemainedMoney;
    res->totalMoney                      = mTotalMoney;
    res->pricePrecision                  = instrument.pricePrecision;
    res->paymentPrecision                = quotationPrecision(tinkoffOperation.payment());
    res->commissionPrecision             = quotationPrecision(tinkoffOperation.commission());

    if (quantityAndCost.quantity <= 0)
    {
        mInstruments.remove(instrumentId);
    }
}
// NOLINTEND(readability-function-cognitive-complexity)

void OperationsThread::alignRemainedAndTotalMoneyFromPortfolio(Operation* lastOperation)
{
    const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
        mGrpcRetryClient->getValidPortfolio(QThread::currentThread(), mAccountId);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
    {
        mRemainedMoney = Quotation();
        mTotalMoney    = Quotation();

        for (int i = 0; i < tinkoffPortfolio->positions_size(); ++i)
        {
            const tinkoff::PortfolioPosition& position = tinkoffPortfolio->positions(i);

            if (QString::fromStdString(position.instrument_uid()) == RUBLE_UID)
            {
                mRemainedMoney = quotationConvert(position.quantity());
                mTotalMoney    = quotationSum(mTotalMoney, mRemainedMoney);
            }
            else
            {
                const Quotation avgCostFifo =
                    quotationMultiply(position.average_position_price_fifo(), position.quantity().units());

                mTotalMoney = quotationSum(mTotalMoney, avgCostFifo);
            }
        }

        lastOperation->remainedMoney = mRemainedMoney;
        lastOperation->totalMoney    = mTotalMoney;
    }
}

void OperationsThread::optimize()
{
    if (mAmountOfEntries > mLimitOperations)
    {
        QList<Operation> newOperations =
            mOptimizer->optimizeOperations(mOperationsDatabase->readOperations(), mOptimizeSize, mInstruments.keys());
        mAmountOfEntries = newOperations.size();

        emit operationsRead(newOperations);
        mOperationsDatabase->writeOperations(newOperations);
    }
}

bool OperationsThread::isOperationTypeWithExtAccount(tinkoff::OperationType operationType, const QString& positionUid) const
{
    return operationType == tinkoff::OPERATION_TYPE_DIV_EXT ||
           (operationType == tinkoff::OPERATION_TYPE_DIVIDEND_TAX && positionUid == mLastPositionUidForExtAccount);
}
