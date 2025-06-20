#pragma once



#include <QObject>

#include "src/domain/portfolio/portfolioitem.h"
#include "src/qxlsx/xlsxdocument.h"



enum PortfolioTreeOldColumns : qint8
{
    PORTFOLIO_OLD_NAME_COLUMN,
    PORTFOLIO_OLD_AVAILABLE_COLUMN,
    PORTFOLIO_OLD_PRICE_COLUMN,
    PORTFOLIO_OLD_AVG_PRICE_COLUMN,
    PORTFOLIO_OLD_COST_COLUMN,
    PORTFOLIO_OLD_PART_COLUMN,
    PORTFOLIO_OLD_YIELD_COLUMN,
    PORTFOLIO_OLD_YIELD_PERCENT_COLUMN,
    PORTFOLIO_OLD_DAILY_YIELD_COLUMN,
    PORTFOLIO_OLD_COLUMN_COUNT,
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
