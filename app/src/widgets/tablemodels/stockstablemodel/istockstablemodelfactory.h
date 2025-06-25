#pragma once



#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel.h"

#include "src/storage/user/iuserstorage.h"



class IStocksTableModelFactory
{
public:
    IStocksTableModelFactory()          = default;
    virtual ~IStocksTableModelFactory() = default;

    IStocksTableModelFactory(const IStocksTableModelFactory& another)            = delete;
    IStocksTableModelFactory& operator=(const IStocksTableModelFactory& another) = delete;

    virtual IStocksTableModel* newInstance(IUserStorage* userStorage, QObject* parent) const = 0;
};
