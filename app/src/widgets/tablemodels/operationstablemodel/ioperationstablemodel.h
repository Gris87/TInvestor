#pragma once



#include <QAbstractTableModel>

#include "src/domain/operation/operation.h"
#include "src/qxlsx/xlsxdocument.h"



enum OperationsTableColumns : qint8
{
    OPERATIONS_TIME_COLUMN,
    OPERATIONS_NAME_COLUMN,
    OPERATIONS_DESCRIPTION_COLUMN,
    OPERATIONS_PRICE_COLUMN,
    OPERATIONS_AVG_PRICE_FIFO_COLUMN,
    OPERATIONS_AVG_PRICE_WAVG_COLUMN,
    OPERATIONS_QUANTITY_COLUMN,
    OPERATIONS_REMAINED_QUANTITY_COLUMN,
    OPERATIONS_PAYMENT_COLUMN,
    OPERATIONS_COMMISSION_COLUMN,
    OPERATIONS_YIELD_COLUMN,
    OPERATIONS_YIELD_WITH_COMMISSION_COLUMN,
    OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN,
    OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN,
    OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN,
    OPERATIONS_REMAINED_MONEY_COLUMN,
    OPERATIONS_TOTAL_MONEY_COLUMN,
    OPERATIONS_COLUMN_COUNT,
};



class IOperationsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit IOperationsTableModel(QObject* parent = nullptr) :
        QAbstractTableModel(parent)
    {
    }
    ~IOperationsTableModel() override = default;

    IOperationsTableModel(const IOperationsTableModel& another)            = delete;
    IOperationsTableModel& operator=(const IOperationsTableModel& another) = delete;

    virtual void operationsRead(const QList<Operation>& operations)  = 0;
    virtual void operationsAdded(const QList<Operation>& operations) = 0;
    virtual void exportToExcel(QXlsx::Document& doc) const           = 0;
};
