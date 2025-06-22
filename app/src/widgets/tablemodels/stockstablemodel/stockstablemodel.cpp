#include "src/widgets/tablemodels/stockstablemodel/stockstablemodel.h"

#include <QDebug>



StocksTableModel::StocksTableModel(QObject* parent) :
    IStocksTableModel(parent),
    mHeader()
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
