#pragma once



#include <QObject>

#include <QTableWidget>

#include "src/domain/filter/filter.h"



enum StocksTableColums : qint8
{
    STOCK_COLUMN,
    PRICE_COLUMN,
    DAY_CHANGE_COLUMN,
    DATE_CHANGE_COLUMN,
    TURNOVER_COLUMN,
    PAYBACK_COLUMN,
    ACTIONS_COLUMN,
    COLUMN_COUNT
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

    virtual void updateAll()                                             = 0;
    virtual void updatePrice()                                           = 0;
    virtual void updatePeriodicData()                                    = 0;
    virtual void filter(QTableWidget* tableWidget, const Filter& filter) = 0;
};
