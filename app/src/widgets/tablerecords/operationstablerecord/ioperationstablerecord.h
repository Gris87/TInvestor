#pragma once



#include <QObject>

#include "src/domain/operation/operation.h"
#include "src/qxlsx/xlsxdocument.h"



enum OperationsTableColumns : qint8
{
    OPERATIONS_TIME_COLUMN,
    OPERATIONS_NAME_COLUMN,
    OPERATIONS_DESCRIPTION_COLUMN,
    OPERATIONS_PRICE_COLUMN,
    OPERATIONS_AVG_PRICE_COLUMN,
    OPERATIONS_QUANTITY_COLUMN,
    OPERATIONS_REMAINED_QUANTITY_COLUMN,
    OPERATIONS_PAYMENT_COLUMN,
    OPERATIONS_COMMISSION_COLUMN,
    OPERATIONS_YIELD_COLUMN,
    OPERATIONS_YIELD_WITH_COMMISSION_COLUMN,
    OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN,
    OPERATIONS_REMAINED_MONEY_COLUMN,
    OPERATIONS_TOTAL_MONEY_COLUMN,
    OPERATIONS_COLUMN_COUNT,
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
