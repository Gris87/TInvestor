#pragma once



#include "src/domain/operation/operation.h"



class IOperationsDatabase
{
public:
    explicit IOperationsDatabase() = default;
    virtual ~IOperationsDatabase() = default;

    IOperationsDatabase(const IOperationsDatabase& another)            = delete;
    IOperationsDatabase& operator=(const IOperationsDatabase& another) = delete;

    virtual void             setAccount(const QString& account)                   = 0;
    virtual QList<Operation> readOperations()                                     = 0;
    virtual void             writeOperations(const QList<Operation>& operations)  = 0;
    virtual void             appendOperations(const QList<Operation>& operations) = 0;
};
