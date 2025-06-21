#include "src/widgets/tablemodels/operationstablemodel/operationstablemodel.h"

#include <QBrush>
#include <QDebug>
#include <algorithm>
#include <execution>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/widgets/tablemodels/modelroles.h"
#include "src/widgets/tablemodels/operationstablemodel/comparators.h"



constexpr QChar RUBLE           = QChar(0x20BD);
constexpr float ZERO_LIMIT      = 0.0001f;
constexpr float HUNDRED_PERCENT = 100.0f;

const char* const DATETIME_FORMAT       = "yyyy-MM-dd hh:mm:ss";
const QBrush      GREEN_COLOR           = QBrush(QColor("#2BD793")); // clazy:exclude=non-pod-global-static
const QBrush      RED_COLOR             = QBrush(QColor("#ED6F7E")); // clazy:exclude=non-pod-global-static
const QBrush      NORMAL_COLOR          = QBrush(QColor("#97AEC4")); // clazy:exclude=non-pod-global-static
const QColor      CELL_BACKGROUND_COLOR = QColor("#2C3C4B");         // clazy:exclude=non-pod-global-static
const QColor      CELL_FONT_COLOR       = QColor("#97AEC4");         // clazy:exclude=non-pod-global-static



OperationsTableModel::OperationsTableModel(QObject* parent) :
    IOperationsTableModel(parent),
    mHeader(),
    mEntries(std::make_shared<QList<Operation>>()),
    mSortColumn(OPERATIONS_TIME_COLUMN),
    mSortOrder(Qt::DescendingOrder)
{
    qDebug() << "Create OperationsTableModel";

    mHeader << tr("Time") << tr("Name") << tr("Description") << tr("Price") << tr("Avg price (FIFO)") << tr("Avg price (WAVG)")
            << tr("Quantity") << tr("Remained quantity") << tr("Payment") << tr("Commission") << tr("Yield")
            << tr("Yield with commission") << tr("Yield with commission (%)") << tr("Total yield") << tr("Total yield (%)")
            << tr("Remained money") << tr("Total money");
    Q_ASSERT_X(mHeader.size() == OPERATIONS_COLUMN_COUNT, __FUNCTION__, "Header is incorrect");
}

OperationsTableModel::~OperationsTableModel()
{
    qDebug() << "Destroy OperationsTableModel";
}

int OperationsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mEntries->size();
}

int OperationsTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return OPERATIONS_COLUMN_COUNT;
}

QVariant OperationsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            return mHeader.at(section);
        }

        return section + 1;
    }

    return QVariant();
}

static QVariant operationsTimeDisplayRole(const Operation& operation)
{
    return QDateTime::fromMSecsSinceEpoch(operation.timestamp).toString(DATETIME_FORMAT);
}

static QVariant operationsNameDisplayRole(const Operation& operation)
{
    return operation.instrumentTicker;
}

static QVariant operationsDescriptionDisplayRole(const Operation& operation)
{
    return operation.description;
}

static QVariant operationsPriceDisplayRole(const Operation& operation)
{
    return QString::number(operation.price, 'f', operation.pricePrecision) + " " + RUBLE;
}

static QVariant operationsAvgPriceFifoDisplayRole(const Operation& operation)
{
    return QString::number(operation.avgPriceFifo, 'f', operation.pricePrecision) + " " + RUBLE;
}

static QVariant operationsAvgPriceWavgDisplayRole(const Operation& operation)
{
    return QString::number(operation.avgPriceWavg, 'f', operation.pricePrecision) + " " + RUBLE;
}

static QVariant operationsQuantityDisplayRole(const Operation& operation)
{
    return operation.quantity;
}

static QVariant operationsRemainedQuantityDisplayRole(const Operation& operation)
{
    return operation.remainedQuantity;
}

static QVariant operationsPaymentDisplayRole(const Operation& operation)
{
    const QString prefix = operation.payment > 0 ? "+" : "";

    return prefix + QString::number(operation.payment, 'f', operation.paymentPrecision) + " " + RUBLE;
}

static QVariant operationsCommissionDisplayRole(const Operation& operation)
{
    const QString prefix = operation.commission > 0 ? "+" : "";

    return prefix + QString::number(operation.commission, 'f', operation.commissionPrecision) + " " + RUBLE;
}

static QVariant operationsYieldDisplayRole(const Operation& operation)
{
    const QString prefix = operation.yield > 0 ? "+" : "";

    return prefix + QString::number(operation.yield, 'f', 2) + " " + RUBLE;
}

static QVariant operationsYieldWithCommissionDisplayRole(const Operation& operation)
{
    const QString prefix = operation.yieldWithCommission > 0 ? "+" : "";

    return prefix + QString::number(operation.yieldWithCommission, 'f', 2) + " " + RUBLE;
}

static QVariant operationsYieldWithCommissionPercentDisplayRole(const Operation& operation)
{
    const QString prefix = operation.yieldWithCommissionPercent > 0 ? "+" : "";

    return prefix + QString::number(operation.yieldWithCommissionPercent, 'f', 2) + "%";
}

static QVariant operationsTotalYieldWithCommissionDisplayRole(const Operation& operation)
{
    const float   value  = quotationToFloat(operation.totalYieldWithCommission);
    const QString prefix = value > 0 ? "+" : "";

    return prefix + QString::number(value, 'f', 2) + " " + RUBLE;
}

static QVariant operationsTotalYieldWithCommissionPercentDisplayRole(const Operation& operation)
{
    const QString prefix = operation.totalYieldWithCommissionPercent > 0 ? "+" : "";

    return prefix + QString::number(operation.totalYieldWithCommissionPercent, 'f', 2) + "%";
}

static QVariant operationsRemainedMoneyDisplayRole(const Operation& operation)
{
    return QString::number(quotationToFloat(operation.remainedMoney), 'f', 2) + " " + RUBLE;
}

static QVariant operationsTotalMoneyDisplayRole(const Operation& operation)
{
    return QString::number(quotationToFloat(operation.totalMoney), 'f', 2) + " " + RUBLE;
}

using DisplayRoleHandler = QVariant (*)(const Operation& operation);

static const DisplayRoleHandler DISPLAY_ROLE_HANDLER[OPERATIONS_COLUMN_COUNT]{
    operationsTimeDisplayRole,
    operationsNameDisplayRole,
    operationsDescriptionDisplayRole,
    operationsPriceDisplayRole,
    operationsAvgPriceFifoDisplayRole,
    operationsAvgPriceWavgDisplayRole,
    operationsQuantityDisplayRole,
    operationsRemainedQuantityDisplayRole,
    operationsPaymentDisplayRole,
    operationsCommissionDisplayRole,
    operationsYieldDisplayRole,
    operationsYieldWithCommissionDisplayRole,
    operationsYieldWithCommissionPercentDisplayRole,
    operationsTotalYieldWithCommissionDisplayRole,
    operationsTotalYieldWithCommissionPercentDisplayRole,
    operationsRemainedMoneyDisplayRole,
    operationsTotalMoneyDisplayRole
};

static QVariant operationsTimeForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsNameForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsDescriptionForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsPriceForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsAvgPriceFifoForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsAvgPriceWavgForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsQuantityForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsRemainedQuantityForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsPaymentForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsCommissionForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsYieldForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsYieldWithCommissionForegroundRole(const Operation& operation)
{
    if (operation.yieldWithCommission > -ZERO_LIMIT && operation.yieldWithCommission < ZERO_LIMIT)
    {
        return NORMAL_COLOR;
    }

    if (operation.yieldWithCommission > 0)
    {
        return GREEN_COLOR;
    }

    return RED_COLOR;
}

static QVariant operationsYieldWithCommissionPercentForegroundRole(const Operation& operation)
{
    if (operation.yieldWithCommissionPercent > -ZERO_LIMIT && operation.yieldWithCommissionPercent < ZERO_LIMIT)
    {
        return NORMAL_COLOR;
    }

    if (operation.yieldWithCommissionPercent > 0)
    {
        return GREEN_COLOR;
    }

    return RED_COLOR;
}

static QVariant operationsTotalYieldWithCommissionForegroundRole(const Operation& operation)
{
    const float value = quotationToFloat(operation.totalYieldWithCommission);

    if (value > -ZERO_LIMIT && value < ZERO_LIMIT)
    {
        return NORMAL_COLOR;
    }

    if (value > 0)
    {
        return GREEN_COLOR;
    }

    return RED_COLOR;
}

static QVariant operationsTotalYieldWithCommissionPercentForegroundRole(const Operation& operation)
{
    if (operation.totalYieldWithCommissionPercent > -ZERO_LIMIT && operation.totalYieldWithCommissionPercent < ZERO_LIMIT)
    {
        return NORMAL_COLOR;
    }

    if (operation.totalYieldWithCommissionPercent > 0)
    {
        return GREEN_COLOR;
    }

    return RED_COLOR;
}

static QVariant operationsRemainedMoneyForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

static QVariant operationsTotalMoneyForegroundRole(const Operation& /*operation*/)
{
    return QVariant();
}

using ForegroundRoleHandler = QVariant (*)(const Operation& operation);

static const ForegroundRoleHandler FOREGROUND_ROLE_HANDLER[OPERATIONS_COLUMN_COUNT]{
    operationsTimeForegroundRole,
    operationsNameForegroundRole,
    operationsDescriptionForegroundRole,
    operationsPriceForegroundRole,
    operationsAvgPriceFifoForegroundRole,
    operationsAvgPriceWavgForegroundRole,
    operationsQuantityForegroundRole,
    operationsRemainedQuantityForegroundRole,
    operationsPaymentForegroundRole,
    operationsCommissionForegroundRole,
    operationsYieldForegroundRole,
    operationsYieldWithCommissionForegroundRole,
    operationsYieldWithCommissionPercentForegroundRole,
    operationsTotalYieldWithCommissionForegroundRole,
    operationsTotalYieldWithCommissionPercentForegroundRole,
    operationsRemainedMoneyForegroundRole,
    operationsTotalMoneyForegroundRole
};

QVariant OperationsTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const int row    = index.row();
        const int column = index.column();

        return DISPLAY_ROLE_HANDLER[column](mEntries->at(row));
    }

    if (role == Qt::ForegroundRole)
    {
        const int row    = index.row();
        const int column = index.column();

        return FOREGROUND_ROLE_HANDLER[column](mEntries->at(row));
    }

    if (role == Qt::ToolTipRole)
    {
        const int row    = index.row();
        const int column = index.column();

        if (column == OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN)
        {
            const Operation& operation = mEntries->at(row);

            if (operation.avgCostFifo <= 0)
            {
                return QVariant();
            }

            return tr("From: %1").arg(operation.avgCostFifo, 0, 'f', operation.pricePrecision) + " " + RUBLE;
        }

        if (column == OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN)
        {
            const float fromPrice = quotationToFloat(mEntries->at(row).maxInputMoney);

            if (fromPrice <= 0)
            {
                return QVariant();
            }

            return tr("From: %1").arg(fromPrice, 0, 'f', 2) + " " + RUBLE;
        }

        return QVariant();
    }

    if (role == ROLE_INSTRUMENT_LOGO)
    {
        const int row = index.row();
        Q_ASSERT_X(index.column() == OPERATIONS_NAME_COLUMN, __FUNCTION__, "Unexpected behavior");

        return reinterpret_cast<qint64>(mEntries->at(row).instrumentLogo);
    }

    if (role == ROLE_INSTRUMENT_NAME)
    {
        const int row = index.row();
        Q_ASSERT_X(index.column() == OPERATIONS_NAME_COLUMN, __FUNCTION__, "Unexpected behavior");

        return mEntries->at(row).instrumentName;
    }

    return QVariant();
}

static void fillEntriesIndeciesForParallel(
    QThread* parentThread, int /*threadId*/, QList<int>& res, int start, int end, void* /*additionalArgs*/
)
{
    int* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = i;
    }
}

struct MergeSortedEntriesInfo
{
    explicit MergeSortedEntriesInfo(QList<Operation>* _entries, QList<int>* _sortedIndecies) :
        entries(_entries),
        sortedIndecies(_sortedIndecies)
    {
    }

    QList<Operation>* entries;
    QList<int>*       sortedIndecies;
};

static void mergeSortedEntriesForParallel(
    QThread* parentThread, int /*threadId*/, QList<Operation>& res, int start, int end, void* additionalArgs
)
{
    MergeSortedEntriesInfo* mergeSortedEntriesInfo = reinterpret_cast<MergeSortedEntriesInfo*>(additionalArgs);

    Operation* entriesArray  = mergeSortedEntriesInfo->entries->data();
    int*       indeciesArray = mergeSortedEntriesInfo->sortedIndecies->data();

    Operation* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = entriesArray[indeciesArray[i]];
    }
}

struct ReverseEntriesInfo
{
    explicit ReverseEntriesInfo(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;
};

static void reverseEntriesForParallel(
    QThread* parentThread, int /*threadId*/, QList<Operation>& res, int start, int end, void* additionalArgs
)
{
    ReverseEntriesInfo* reverseEntriesInfo = reinterpret_cast<ReverseEntriesInfo*>(additionalArgs);

    Operation* entriesArray = reverseEntriesInfo->entries->data();

    Operation* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = entriesArray[res.size() - i - 1];
    }
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void OperationsTableModel::sort(int column, Qt::SortOrder order)
{
    if (mSortColumn != column || mSortOrder != order)
    {
        const QList<QPersistentModelIndex> parents;

        emit layoutAboutToBeChanged(parents, QAbstractItemModel::VerticalSortHint);

        if (mSortColumn != column)
        {
            mSortColumn = column;
            mSortOrder  = order;

            QList<int> entriesIndecies;
            entriesIndecies.resizeForOverwrite(mEntries->size());
            processInParallel(entriesIndecies, fillEntriesIndeciesForParallel);

            if (mSortOrder == Qt::AscendingOrder)
            {
                if (mSortColumn == OPERATIONS_TIME_COLUMN)
                {
                    const OperationsTableTimeLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_NAME_COLUMN)
                {
                    const OperationsTableNameLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_DESCRIPTION_COLUMN)
                {
                    const OperationsTableDescriptionLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_PRICE_COLUMN)
                {
                    const OperationsTablePriceLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_AVG_PRICE_FIFO_COLUMN)
                {
                    const OperationsTableAvgPriceFifoLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_AVG_PRICE_WAVG_COLUMN)
                {
                    const OperationsTableAvgPriceWavgLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_QUANTITY_COLUMN)
                {
                    const OperationsTableQuantityLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_REMAINED_QUANTITY_COLUMN)
                {
                    const OperationsTableRemainedQuantityLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_PAYMENT_COLUMN)
                {
                    const OperationsTablePaymentLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_COMMISSION_COLUMN)
                {
                    const OperationsTableCommissionLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_YIELD_COLUMN)
                {
                    const OperationsTableYieldLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_YIELD_WITH_COMMISSION_COLUMN)
                {
                    const OperationsTableYieldWithCommissionLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN)
                {
                    const OperationsTableYieldWithCommissionPercentLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN)
                {
                    const OperationsTableTotalYieldWithCommissionLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN)
                {
                    const OperationsTableTotalYieldWithCommissionPercentLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_REMAINED_MONEY_COLUMN)
                {
                    const OperationsTableRemainedMoneyLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_TOTAL_MONEY_COLUMN)
                {
                    const OperationsTableTotalMoneyLessThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
            }
            else
            {
                if (mSortColumn == OPERATIONS_TIME_COLUMN)
                {
                    const OperationsTableTimeGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_NAME_COLUMN)
                {
                    const OperationsTableNameGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_DESCRIPTION_COLUMN)
                {
                    const OperationsTableDescriptionGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_PRICE_COLUMN)
                {
                    const OperationsTablePriceGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_AVG_PRICE_FIFO_COLUMN)
                {
                    const OperationsTableAvgPriceFifoGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_AVG_PRICE_WAVG_COLUMN)
                {
                    const OperationsTableAvgPriceWavgGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_QUANTITY_COLUMN)
                {
                    const OperationsTableQuantityGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_REMAINED_QUANTITY_COLUMN)
                {
                    const OperationsTableRemainedQuantityGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_PAYMENT_COLUMN)
                {
                    const OperationsTablePaymentGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_COMMISSION_COLUMN)
                {
                    const OperationsTableCommissionGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_YIELD_COLUMN)
                {
                    const OperationsTableYieldGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_YIELD_WITH_COMMISSION_COLUMN)
                {
                    const OperationsTableYieldWithCommissionGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN)
                {
                    const OperationsTableYieldWithCommissionPercentGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN)
                {
                    const OperationsTableTotalYieldWithCommissionGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN)
                {
                    const OperationsTableTotalYieldWithCommissionPercentGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_REMAINED_MONEY_COLUMN)
                {
                    const OperationsTableRemainedMoneyGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
                else if (mSortColumn == OPERATIONS_TOTAL_MONEY_COLUMN)
                {
                    const OperationsTableTotalMoneyGreaterThan cmp(mEntries.get());

                    std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
                }
            }

            const std::shared_ptr<QList<Operation>> entries = std::make_shared<QList<Operation>>();
            entries->resizeForOverwrite(mEntries->size());

            MergeSortedEntriesInfo mergeSortedEntriesInfo(mEntries.get(), &entriesIndecies);
            processInParallel(*entries, mergeSortedEntriesForParallel, &mergeSortedEntriesInfo);

            mEntries = entries;
        }
        else
        {
            mSortOrder = order;

            const std::shared_ptr<QList<Operation>> entries = std::make_shared<QList<Operation>>();
            entries->resizeForOverwrite(mEntries->size());

            ReverseEntriesInfo reverseEntriesInfo(mEntries.get());
            processInParallel(*entries, reverseEntriesForParallel, &reverseEntriesInfo);

            mEntries = entries;
        }

        emit layoutChanged(parents, QAbstractItemModel::VerticalSortHint);
    }
}
// NOLINTEND(readability-function-cognitive-complexity)

void OperationsTableModel::operationsRead(const QList<Operation>& operations)
{
    beginResetModel();

    mSortColumn = OPERATIONS_TIME_COLUMN;
    mSortOrder  = Qt::DescendingOrder;

    mEntries = std::make_shared<QList<Operation>>(operations);

    endResetModel();
}

void OperationsTableModel::operationsAdded(const QList<Operation>& operations)
{
    if (mSortColumn == OPERATIONS_TIME_COLUMN)
    {
        if (mSortOrder == Qt::AscendingOrder)
        {
            beginInsertRows(QModelIndex(), mEntries->size(), mEntries->size() + operations.size() - 1);

            for (int i = operations.size() - 1; i >= 0; --i)
            {
                const Operation& operation = operations.at(i);

                mEntries->append(operation);
            }

            endInsertRows();
        }
        else
        {
            beginInsertRows(QModelIndex(), 0, operations.size() - 1);

            for (int i = operations.size() - 1; i >= 0; --i)
            {
                const Operation& operation = operations.at(i);

                mEntries->prepend(operation);
            }

            endInsertRows();
        }
    }
    else
    {
        for (int i = operations.size() - 1; i >= 0; --i)
        {
            const Operation& operation = operations.at(i);

            insertRow(mEntries.get(), indexOfSortedInsert(mEntries.get(), operation), operation);
        }
    }
}

void OperationsTableModel::exportToExcel(QXlsx::Document& doc) const
{
    QXlsx::Format cellStyle;
    cellStyle.setFillPattern(QXlsx::Format::PatternSolid);
    cellStyle.setBorderStyle(QXlsx::Format::BorderThin);
    cellStyle.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    cellStyle.setFontColor(CELL_FONT_COLOR);

    QXlsx::Format dateFormat;
    dateFormat.setNumberFormat(DATETIME_FORMAT);
    dateFormat.setFillPattern(QXlsx::Format::PatternSolid);
    dateFormat.setBorderStyle(QXlsx::Format::BorderThin);
    dateFormat.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    dateFormat.setFontColor(CELL_FONT_COLOR);

    for (int i = 0; i < mEntries->size(); ++i)
    {
        const int        row   = i + 2; // Header and start index from 1
        const Operation& entry = mEntries->at(i);

        // clang-format off
        doc.write(row, OPERATIONS_TIME_COLUMN + 1,                                QDateTime::fromMSecsSinceEpoch(entry.timestamp), dateFormat);
        doc.write(row, OPERATIONS_NAME_COLUMN + 1,                                entry.instrumentName, cellStyle);
        doc.write(row, OPERATIONS_DESCRIPTION_COLUMN + 1,                         entry.description, cellStyle);
        doc.write(row, OPERATIONS_PRICE_COLUMN + 1,                               entry.price, createRubleFormat(CELL_FONT_COLOR, false, entry.pricePrecision));
        doc.write(row, OPERATIONS_AVG_PRICE_FIFO_COLUMN + 1,                      entry.avgPriceFifo, createRubleFormat(CELL_FONT_COLOR, false, entry.pricePrecision));
        doc.write(row, OPERATIONS_AVG_PRICE_WAVG_COLUMN + 1,                      entry.avgPriceWavg, createRubleFormat(CELL_FONT_COLOR, false, entry.pricePrecision));
        doc.write(row, OPERATIONS_QUANTITY_COLUMN + 1,                            entry.quantity, cellStyle);
        doc.write(row, OPERATIONS_REMAINED_QUANTITY_COLUMN + 1,                   entry.remainedQuantity, cellStyle);
        doc.write(row, OPERATIONS_PAYMENT_COLUMN + 1,                             entry.payment, createRubleFormat(CELL_FONT_COLOR, true, entry.paymentPrecision));
        doc.write(row, OPERATIONS_COMMISSION_COLUMN + 1,                          entry.commission, createRubleFormat(CELL_FONT_COLOR, true, entry.commissionPrecision));
        doc.write(row, OPERATIONS_YIELD_COLUMN + 1,                               entry.yield, createRubleFormat(CELL_FONT_COLOR, true, 2));
        doc.write(row, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN + 1,               entry.yieldWithCommission, createRubleFormat(operationsYieldWithCommissionForegroundRole(entry).value<QBrush>().color(), true, 2));
        doc.write(row, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN + 1,       entry.yieldWithCommissionPercent / HUNDRED_PERCENT, createPercentFormat(operationsYieldWithCommissionPercentForegroundRole(entry).value<QBrush>().color(), true));
        doc.write(row, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN + 1,         quotationToFloat(entry.totalYieldWithCommission), createRubleFormat(operationsTotalYieldWithCommissionForegroundRole(entry).value<QBrush>().color(), true, 2));
        doc.write(row, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN + 1, entry.totalYieldWithCommissionPercent / HUNDRED_PERCENT, createPercentFormat(operationsTotalYieldWithCommissionPercentForegroundRole(entry).value<QBrush>().color(), true));
        doc.write(row, OPERATIONS_REMAINED_MONEY_COLUMN + 1,                      quotationToFloat(entry.remainedMoney), createRubleFormat(CELL_FONT_COLOR, false, 2));
        doc.write(row, OPERATIONS_TOTAL_MONEY_COLUMN + 1,                         quotationToFloat(entry.totalMoney), createRubleFormat(CELL_FONT_COLOR, false, 2));
        // clang-format on
    }
}

QXlsx::Format OperationsTableModel::createRubleFormat(const QColor& color, bool withPlus, int precision) const
{
    QXlsx::Format res;

    if (withPlus)
    {
        res.setNumberFormat(QString("+0.%1 \u20BD;-0.%1 \u20BD;0.%1 \u20BD").arg("", precision, '0'));
    }
    else
    {
        res.setNumberFormat(QString("0.%1 \u20BD").arg("", precision, '0'));
    }

    res.setFillPattern(QXlsx::Format::PatternSolid);
    res.setBorderStyle(QXlsx::Format::BorderThin);
    res.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    res.setFontColor(color);

    return res;
}

QXlsx::Format OperationsTableModel::createPercentFormat(const QColor& color, bool withPlus) const
{
    QXlsx::Format res;

    if (withPlus)
    {
        res.setNumberFormat("+0.00%;-0.00%;0.00%");
    }
    else
    {
        res.setNumberFormat("0.00%");
    }

    res.setFillPattern(QXlsx::Format::PatternSolid);
    res.setBorderStyle(QXlsx::Format::BorderThin);
    res.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    res.setFontColor(color);

    return res;
}

using AscSortHandler = bool (*)(const Operation& l, const Operation& r);

static const AscSortHandler ASC_SORT_HANDLER[OPERATIONS_COLUMN_COUNT]{
    operationsTimeLess,
    operationsNameLess,
    operationsDescriptionLess,
    operationsPriceLess,
    operationsAvgPriceFifoLess,
    operationsAvgPriceWavgLess,
    operationsQuantityLess,
    operationsRemainedQuantityLess,
    operationsPaymentLess,
    operationsCommissionLess,
    operationsYieldLess,
    operationsYieldWithCommissionLess,
    operationsYieldWithCommissionPercentLess,
    operationsTotalYieldWithCommissionLess,
    operationsTotalYieldWithCommissionPercentLess,
    operationsRemainedMoneyLess,
    operationsTotalMoneyLess
};

using DescSortHandler = bool (*)(const Operation& l, const Operation& r);

static const DescSortHandler DESC_SORT_HANDLER[OPERATIONS_COLUMN_COUNT]{
    operationsTimeGreater,
    operationsNameGreater,
    operationsDescriptionGreater,
    operationsPriceGreater,
    operationsAvgPriceFifoGreater,
    operationsAvgPriceWavgGreater,
    operationsQuantityGreater,
    operationsRemainedQuantityGreater,
    operationsPaymentGreater,
    operationsCommissionGreater,
    operationsYieldGreater,
    operationsYieldWithCommissionGreater,
    operationsYieldWithCommissionPercentGreater,
    operationsTotalYieldWithCommissionGreater,
    operationsTotalYieldWithCommissionPercentGreater,
    operationsRemainedMoneyGreater,
    operationsTotalMoneyGreater
};

int OperationsTableModel::indexOfSortedInsert(QList<Operation>* entries, const Operation& entry)
{
    int res = 0;

    if (mSortOrder == Qt::AscendingOrder)
    {
        res = std::distance(
            entries->begin(), std::lower_bound(entries->begin(), entries->end(), entry, ASC_SORT_HANDLER[mSortColumn])
        );
    }
    else
    {
        res = std::distance(
            entries->begin(), std::lower_bound(entries->begin(), entries->end(), entry, DESC_SORT_HANDLER[mSortColumn])
        );
    }

    return res;
}

void OperationsTableModel::insertRow(QList<Operation>* entries, int row, const Operation& entry)
{
    beginInsertRows(QModelIndex(), row, row);

    entries->insert(row, entry);

    endInsertRows();
}
