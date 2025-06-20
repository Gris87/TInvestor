#include "src/widgets/tablemodels/portfoliotreemodel/portfoliotreemodelfactory.h"

#include <QDebug>

#include "src/widgets/tablemodels/portfoliotreemodel/portfoliotreemodel.h"



PortfolioTreeModelFactory::PortfolioTreeModelFactory() :
    IPortfolioTreeModelFactory()
{
    qDebug() << "Create PortfolioTreeModelFactory";
}

PortfolioTreeModelFactory::~PortfolioTreeModelFactory()
{
    qDebug() << "Destroy PortfolioTreeModelFactory";
}

IPortfolioTreeModel* PortfolioTreeModelFactory::newInstance(QObject* parent) const
{
    return new PortfolioTreeModel(parent);
}
