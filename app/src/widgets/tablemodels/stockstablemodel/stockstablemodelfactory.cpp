#include "src/widgets/tablemodels/stockstablemodel/stockstablemodelfactory.h"

#include <QDebug>

#include "src/widgets/tablemodels/stockstablemodel/stockstablemodel.h"



StocksTableModelFactory::StocksTableModelFactory() :
    IStocksTableModelFactory()
{
    qDebug() << "Create StocksTableModelFactory";
}

StocksTableModelFactory::~StocksTableModelFactory()
{
    qDebug() << "Destroy StocksTableModelFactory";
}

IStocksTableModel* StocksTableModelFactory::newInstance(QObject* parent) const
{
    return new StocksTableModel(parent);
}
