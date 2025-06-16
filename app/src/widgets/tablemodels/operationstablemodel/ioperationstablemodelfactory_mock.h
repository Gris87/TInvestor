#pragma once



#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodelfactory.h"

#include <gmock/gmock.h>



class OperationsTableModelFactoryMock : public IOperationsTableModelFactory
{
public:
    OperationsTableModelFactoryMock() :
        IOperationsTableModelFactory()
    {
    }
    ~OperationsTableModelFactoryMock() override = default;

    OperationsTableModelFactoryMock(const OperationsTableModelFactoryMock& another)            = delete;
    OperationsTableModelFactoryMock& operator=(const OperationsTableModelFactoryMock& another) = delete;

    MOCK_METHOD(IOperationsTableModel*, newInstance, (QObject * parent), (const, override));
};
