#pragma once



#include "src/domain/operation/operation.h"



class IOperationsDatabase
{
public:
    explicit IOperationsDatabase() = default;
    virtual ~IOperationsDatabase() = default;

    IOperationsDatabase(const IOperationsDatabase& another)            = delete;
    IOperationsDatabase& operator=(const IOperationsDatabase& another) = delete;

    virtual void             setAccount(const QString& account)                                    = 0;
    virtual QList<Operation> readOperations(int partId = -1)                                       = 0;
    virtual void             writeOperations(QList<Operation>& operations, int partId = -1)        = 0;
    virtual void             appendOperations(const QList<Operation>& operations, int partId = -1) = 0;
    virtual void             deleteOperations(int partId = -1)                                     = 0;
};
