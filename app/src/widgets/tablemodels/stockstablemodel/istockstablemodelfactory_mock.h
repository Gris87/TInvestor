#pragma once



#include "src/widgets/tablemodels/stockstablemodel/istockstablemodelfactory.h"

#include <gmock/gmock.h>



class StocksTableModelFactoryMock : public IStocksTableModelFactory
{
public:
    StocksTableModelFactoryMock() :
        IStocksTableModelFactory()
    {
    }
    ~StocksTableModelFactoryMock() override = default;

    StocksTableModelFactoryMock(const StocksTableModelFactoryMock& another)            = delete;
    StocksTableModelFactoryMock& operator=(const StocksTableModelFactoryMock& another) = delete;

    MOCK_METHOD(IStocksTableModel*, newInstance, (IUserStorage * userStorage, QObject* parent), (const, override));
};
