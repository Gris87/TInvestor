#include "src/widgets/tablemodels/operationstablemodel/operationstablemodelfactory.h"

#include <QDebug>

#include "src/widgets/tablemodels/operationstablemodel/operationstablemodel.h"



OperationsTableModelFactory::OperationsTableModelFactory() :
    IOperationsTableModelFactory()
{
    qDebug() << "Create OperationsTableModelFactory";
}

OperationsTableModelFactory::~OperationsTableModelFactory()
{
    qDebug() << "Destroy OperationsTableModelFactory";
}

IOperationsTableModel* OperationsTableModelFactory::newInstance(QObject* parent) const
{
    return new OperationsTableModel(parent);
}
