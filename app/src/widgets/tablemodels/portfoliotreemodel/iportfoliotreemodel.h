#pragma once



#include <QAbstractTableModel>



enum PortfolioTreeColumns : qint8
{
    PORTFOLIO_NAME_COLUMN,
    PORTFOLIO_AVAILABLE_COLUMN,
    PORTFOLIO_PRICE_COLUMN,
    PORTFOLIO_AVG_PRICE_COLUMN,
    PORTFOLIO_COST_COLUMN,
    PORTFOLIO_PART_COLUMN,
    PORTFOLIO_YIELD_COLUMN,
    PORTFOLIO_YIELD_PERCENT_COLUMN,
    PORTFOLIO_DAILY_YIELD_COLUMN,
    PORTFOLIO_COLUMN_COUNT,
};



class IPortfolioTreeModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit IPortfolioTreeModel(QObject* parent = nullptr) :
        QAbstractTableModel(parent)
    {
    }
    ~IPortfolioTreeModel() override = default;

    IPortfolioTreeModel(const IPortfolioTreeModel& another)            = delete;
    IPortfolioTreeModel& operator=(const IPortfolioTreeModel& another) = delete;
};
