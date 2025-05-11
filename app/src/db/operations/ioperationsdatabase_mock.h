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
    MOCK_METHOD(QList<Operation>, readOperations, (), (override));
    MOCK_METHOD(void, writeOperations, (const QList<Operation>& operations), (override));
    MOCK_METHOD(void, appendOperations, (const QList<Operation>& operations), (override));
};
