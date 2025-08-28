#pragma once



#include <QAbstractItemModel>

#include "src/domain/portfolio/portfolio.h"
#include "src/qxlsx/xlsxdocument.h"



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



class IPortfolioTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit IPortfolioTreeModel(QObject* parent = nullptr) :
        QAbstractItemModel(parent)
    {
    }
    ~IPortfolioTreeModel() override = default;

    IPortfolioTreeModel(const IPortfolioTreeModel& another)            = delete;
    IPortfolioTreeModel& operator=(const IPortfolioTreeModel& another) = delete;

    virtual void portfolioChanged(const Portfolio& portfolio)               = 0;
    virtual void lastPriceChanged(const QString& instrumentId, float price) = 0;
    virtual bool updateLastPrices()                                         = 0;
    virtual void exportToExcel(QXlsx::Document& doc) const                  = 0;

    [[nodiscard]]
    virtual double totalCost() const = 0;

    [[nodiscard]]
    virtual double totalYield() const = 0;

    [[nodiscard]]
    virtual double totalDailyCost() const = 0;

    [[nodiscard]]
    virtual double totalDailyYield() const = 0;
};
