#pragma once



#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel.h"



class IStocksTableModelFactory
{
public:
    IStocksTableModelFactory()          = default;
    virtual ~IStocksTableModelFactory() = default;

    IStocksTableModelFactory(const IStocksTableModelFactory& another)            = delete;
    IStocksTableModelFactory& operator=(const IStocksTableModelFactory& another) = delete;

    virtual IStocksTableModel* newInstance(QObject* parent) const = 0;
};
