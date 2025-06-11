#pragma once



#include <QObject>

#include <QTableWidget>

#include "src/domain/filter/stockfilter.h"
#include "src/qxlsx/xlsxdocument.h"



enum StocksTableColumns : qint8
{
    STOCKS_STOCK_COLUMN,
    STOCKS_PRICE_COLUMN,
    STOCKS_DAY_CHANGE_COLUMN,
    STOCKS_DATE_CHANGE_COLUMN,
    STOCKS_TURNOVER_COLUMN,
    STOCKS_PAYBACK_COLUMN,
    STOCKS_ACTIONS_COLUMN,
    STOCKS_COLUMN_COUNT
};



class IStocksTableRecord : public QObject
{
    Q_OBJECT

public:
    explicit IStocksTableRecord(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    ~IStocksTableRecord() override = default;

    IStocksTableRecord(const IStocksTableRecord& another)            = delete;
    IStocksTableRecord& operator=(const IStocksTableRecord& another) = delete;

    virtual void updateAll()                                                  = 0;
    virtual void updatePrice()                                                = 0;
    virtual void updatePeriodicData()                                         = 0;
    virtual void filter(QTableWidget* tableWidget, const StockFilter& filter) = 0;
    virtual void exportToExcel(QXlsx::Document& doc) const                    = 0;
};
