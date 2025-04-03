#pragma once



#include <QObject>

#include <QTableWidget>

#include "src/domain/filter/filter.h"



#define STOCK_COLUMN       0
#define PRICE_COLUMN       1
#define DAY_CHANGE_COLUMN  2
#define DATE_CHANGE_COLUMN 3
#define TURNOVER_COLUMN    4
#define PAYBACK_COLUMN     5
#define ACTIONS_COLUMN     6
#define COLUMN_COUNT       7



class IStocksTableRecord : public QObject
{
    Q_OBJECT

public:
    explicit IStocksTableRecord(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    virtual ~IStocksTableRecord() = default;

    IStocksTableRecord(const IStocksTableRecord& another)            = delete;
    IStocksTableRecord& operator=(const IStocksTableRecord& another) = delete;

    virtual void updateAll()                                             = 0;
    virtual void updatePrice()                                           = 0;
    virtual void updatePeriodicData()                                    = 0;
    virtual void filter(QTableWidget* tableWidget, const Filter& filter) = 0;
};
