#pragma once



#include <QObject>



enum OperationsTableColumns : qint8
{
    TIME_COLUMN,
    NAME_COLUMN,
    OPERATION_COLUMN,
    PRICE_COLUMN,
    QUANTITY_COLUMN,
    COST_COLUMN,
    COLUMN_COUNT,
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
};
