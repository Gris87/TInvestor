#pragma once



#include <QAbstractTableModel>

#include "src/domain/filter/stockfilter.h"
#include "src/domain/stock/stock.h"
#include "src/qxlsx/xlsxdocument.h"



enum StocksTableColumns : qint8
{
    STOCKS_NAME_COLUMN,
    STOCKS_PRICE_COLUMN,
    STOCKS_DAY_CHANGE_COLUMN,
    STOCKS_DATE_CHANGE_COLUMN,
    STOCKS_TURNOVER_COLUMN,
    STOCKS_PAYBACK_COLUMN,
    STOCKS_ACTIONS_COLUMN,
    STOCKS_COLUMN_COUNT
};



class IStocksTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit IStocksTableModel(QObject* parent = nullptr) :
        QAbstractTableModel(parent)
    {
    }
    ~IStocksTableModel() override = default;

    IStocksTableModel(const IStocksTableModel& another)            = delete;
    IStocksTableModel& operator=(const IStocksTableModel& another) = delete;

    virtual void setFilter(const StockFilter& filter)          = 0;
    virtual void updateTable(const QList<Stock*>& stocks)      = 0;
    virtual void updateAll()                                   = 0;
    virtual void lastPriceChanged(const QString& instrumentId) = 0;
    virtual void updateLastPrices()                            = 0;
    virtual void updatePrices()                                = 0;
    virtual void updatePeriodicData()                          = 0;
    virtual void exportToExcel(QXlsx::Document& doc) const     = 0;

    virtual void setDateChangeTooltip(const QString& tooltip) = 0;
};
