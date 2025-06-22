#pragma once



#include "src/widgets/tablemodels/stockstablemodel/istockstablemodelfactory.h"



class StocksTableModelFactory : public IStocksTableModelFactory
{
public:
    StocksTableModelFactory();
    ~StocksTableModelFactory() override;

    StocksTableModelFactory(const StocksTableModelFactory& another)            = delete;
    StocksTableModelFactory& operator=(const StocksTableModelFactory& another) = delete;

    IStocksTableModel* newInstance(QObject* parent) const override;
};
