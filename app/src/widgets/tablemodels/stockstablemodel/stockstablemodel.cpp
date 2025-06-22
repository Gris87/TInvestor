#include "src/widgets/tablemodels/stockstablemodel/stockstablemodel.h"

#include <QDebug>



StocksTableModel::StocksTableModel(QObject* parent) :
    IStocksTableModel(parent),
    mHeader(),
    mHelpIcon(":/assets/images/question.png"),
    mDateChangeTooltip()
{
    qDebug() << "Create StocksTableModel";

    mHeader << tr("Stock") << tr("Price") << tr("Change from day start") << tr("Change from some date") << tr("Turnover")
            << tr("Payback") << tr("Actions");
    Q_ASSERT_X(mHeader.size() == STOCKS_COLUMN_COUNT, __FUNCTION__, "Header is incorrect");
}

StocksTableModel::~StocksTableModel()
{
    qDebug() << "Destroy StocksTableModel";
}

int StocksTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return 5;
}

int StocksTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return STOCKS_COLUMN_COUNT;
}

QVariant StocksTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            return mHeader.at(section);
        }

        return section + 1;
    }

    if (role == Qt::ToolTipRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == STOCKS_DATE_CHANGE_COLUMN)
            {
                return mDateChangeTooltip;
            }

            if (section == STOCKS_TURNOVER_COLUMN)
            {
                return tr("Average daily turnover in the last 31 days");
            }

            if (section == STOCKS_PAYBACK_COLUMN)
            {
                return tr("Opportunity to get money back (with commission) based on the last 24 hours");
            }
        }

        return QVariant();
    }

    if (role == Qt::DecorationRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == STOCKS_DATE_CHANGE_COLUMN || section == STOCKS_TURNOVER_COLUMN || section == STOCKS_PAYBACK_COLUMN)
            {
                return mHelpIcon;
            }
        }

        return QVariant();
    }

    return QVariant();
}

QVariant StocksTableModel::data(const QModelIndex& /*index*/, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return "a";
    }

    return QVariant();
}

void StocksTableModel::setDateChangeTooltip(const QString& tooltip)
{
    mDateChangeTooltip = tooltip;
}
