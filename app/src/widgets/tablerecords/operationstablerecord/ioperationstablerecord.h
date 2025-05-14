#pragma once



#include <QObject>

#include "src/domain/operation/operation.h"



enum OperationsTableColumns : qint8
{
    OPERATIONS_TIME_COLUMN,
    OPERATIONS_NAME_COLUMN,
    OPERATIONS_DESCRIPTION_COLUMN,
    OPERATIONS_PRICE_COLUMN,
    OPERATIONS_QUANTITY_COLUMN,
    OPERATIONS_COST_COLUMN,
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

    virtual void setOperation(const Operation& operation) = 0;
};
