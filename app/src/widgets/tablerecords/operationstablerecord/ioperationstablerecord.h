#pragma once



#include <QObject>

#include "src/domain/operation/operation.h"
#include "src/qxlsx/xlsxdocument.h"



enum OperationsTableOldColumns : qint8
{
    OPERATIONS_OLD_TIME_COLUMN,
    OPERATIONS_OLD_NAME_COLUMN,
    OPERATIONS_OLD_DESCRIPTION_COLUMN,
    OPERATIONS_OLD_PRICE_COLUMN,
    OPERATIONS_OLD_AVG_PRICE_FIFO_COLUMN,
    OPERATIONS_OLD_AVG_PRICE_WAVG_COLUMN,
    OPERATIONS_OLD_QUANTITY_COLUMN,
    OPERATIONS_OLD_REMAINED_QUANTITY_COLUMN,
    OPERATIONS_OLD_PAYMENT_COLUMN,
    OPERATIONS_OLD_COMMISSION_COLUMN,
    OPERATIONS_OLD_YIELD_COLUMN,
    OPERATIONS_OLD_YIELD_WITH_COMMISSION_COLUMN,
    OPERATIONS_OLD_YIELD_WITH_COMMISSION_PERCENT_COLUMN,
    OPERATIONS_OLD_TOTAL_YIELD_WITH_COMMISSION_COLUMN,
    OPERATIONS_OLD_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN,
    OPERATIONS_OLD_REMAINED_MONEY_COLUMN,
    OPERATIONS_OLD_TOTAL_MONEY_COLUMN,
    OPERATIONS_OLD_COLUMN_COUNT,
};



class IOperationsTableRecord : public QObject
{
    Q_OBJECT

public:
    explicit IOperationsTableRecord(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    ~IOperationsTableRecord() override = default;

    IOperationsTableRecord(const IOperationsTableRecord& another)            = delete;
    IOperationsTableRecord& operator=(const IOperationsTableRecord& another) = delete;

    virtual void setOperation(const Operation& operation)  = 0;
    virtual void exportToExcel(QXlsx::Document& doc) const = 0;
};
