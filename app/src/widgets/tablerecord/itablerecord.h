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



class ITableRecord : public QObject
{
    Q_OBJECT

public:
    explicit ITableRecord(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    virtual ~ITableRecord() = default;

    ITableRecord(const ITableRecord& another)            = delete;
    ITableRecord& operator=(const ITableRecord& another) = delete;

    virtual void updateAll()                                             = 0;
    virtual void updatePrice()                                           = 0;
    virtual void updatePeriodicData()                                    = 0;
    virtual void filter(QTableWidget* tableWidget, const Filter& filter) = 0;
};
