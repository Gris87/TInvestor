#pragma once



#include "src/widgets/tablerecord/itablerecordfactory.h"



class TableRecordFactory : public ITableRecordFactory
{
public:
    TableRecordFactory();
    ~TableRecordFactory();

    TableRecordFactory(const TableRecordFactory& another)            = delete;
    TableRecordFactory& operator=(const TableRecordFactory& another) = delete;

    ITableRecord* newInstance(QTableWidget* tableWidget, Stock* stock, QObject* parent) override;
};
