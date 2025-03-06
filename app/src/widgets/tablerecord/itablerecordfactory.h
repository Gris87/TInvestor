#pragma once



#include "src/widgets/tablerecord/itablerecord.h"

#include <QTableWidget>

#include "src/domain/stocks/stocks.h"



class ITableRecordFactory
{
public:
    ITableRecordFactory()
    {
    }
    virtual ~ITableRecordFactory() = default;

    ITableRecordFactory(const ITableRecordFactory& another)            = delete;
    ITableRecordFactory& operator=(const ITableRecordFactory& another) = delete;

    virtual ITableRecord* newInstance(QTableWidget* tableWidget, Stock* stock, QObject* parent) = 0;
};
