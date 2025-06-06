#pragma once



#include <QObject>

#include "src/domain/portfolio/portfolioitem.h"
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



class IPortfolioTreeRecord : public QObject
{
    Q_OBJECT

public:
    explicit IPortfolioTreeRecord(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    ~IPortfolioTreeRecord() override = default;

    IPortfolioTreeRecord(const IPortfolioTreeRecord& another)            = delete;
    IPortfolioTreeRecord& operator=(const IPortfolioTreeRecord& another) = delete;

    virtual void setPortfolioItem(const PortfolioItem& item)        = 0;
    virtual void updatePrice(float price)                           = 0;
    virtual void exportToExcel(QXlsx::Document& doc, int row) const = 0;

    [[nodiscard]]
    virtual float yield() const = 0;

    [[nodiscard]]
    virtual float dailyYield() const = 0;
};
