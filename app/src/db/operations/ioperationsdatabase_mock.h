#pragma once



#include "src/db/operations/ioperationsdatabase.h"

#include <gmock/gmock.h>



class OperationsDatabaseMock : public IOperationsDatabase
{
public:
    explicit OperationsDatabaseMock() :
        IOperationsDatabase()
    {
    }
    ~OperationsDatabaseMock() override = default;

    OperationsDatabaseMock(const OperationsDatabaseMock& another)            = delete;
    OperationsDatabaseMock& operator=(const OperationsDatabaseMock& another) = delete;

    MOCK_METHOD(void, setAccount, (const QString& account), (override));
    MOCK_METHOD(QList<Operation>, readOperations, (int partId), (override));
    MOCK_METHOD(void, writeOperations, (QList<Operation> & operations, int partId), (override));
    MOCK_METHOD(void, appendOperations, (const QList<Operation>& operations, int partId), (override));
    MOCK_METHOD(void, deleteOperations, (int partId), (override));
};
