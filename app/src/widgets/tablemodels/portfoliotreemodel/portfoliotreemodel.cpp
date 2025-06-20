#include "src/widgets/tablemodels/portfoliotreemodel/portfoliotreemodel.h"

#include <QDebug>



PortfolioTreeModel::PortfolioTreeModel(QObject* parent) :
    IPortfolioTreeModel(parent),
    mHeader(),
    mHelpIcon(":/assets/images/question.png"),
    mPortfolio()
{
    qDebug() << "Create PortfolioTreeModel";

    mHeader << tr("Name") << tr("Available") << tr("Price") << tr("Avg price") << tr("Cost") << tr("Part") << tr("Yield")
            << tr("Yield, %") << tr("Daily yield, %");
    Q_ASSERT_X(mHeader.size() == PORTFOLIO_COLUMN_COUNT, __FUNCTION__, "Header is incorrect");
}

PortfolioTreeModel::~PortfolioTreeModel()
{
    qDebug() << "Destroy PortfolioTreeModel";
}

int PortfolioTreeModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mPortfolio.positionsList.size();
}

int PortfolioTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
    return PORTFOLIO_COLUMN_COUNT;
}

QVariant PortfolioTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        if (section == PORTFOLIO_AVG_PRICE_COLUMN)
        {
            return tr("Average price by FIFO");
        }

        return QVariant();
    }

    if (role == Qt::DecorationRole)
    {
        if (section == PORTFOLIO_AVG_PRICE_COLUMN)
        {
            return mHelpIcon;
        }

        return QVariant();
    }

    return QVariant();
}

QVariant PortfolioTreeModel::data(const QModelIndex& /*index*/, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return "a";
    }

    return QVariant();
}

void PortfolioTreeModel::portfolioChanged(const Portfolio& portfolio)
{
    beginResetModel();

    mPortfolio = portfolio;

    // TODO: Sort

    endResetModel();
}

void PortfolioTreeModel::lastPriceChanged(const QString& /*instrumentId*/, float /*price*/)
{
    // TODO: Implement
}

void PortfolioTreeModel::updateLastPrices()
{
    // TODO: Implement
}
