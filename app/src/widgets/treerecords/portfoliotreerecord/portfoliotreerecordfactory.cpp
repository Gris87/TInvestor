#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecordfactory.h"

#include <QDebug>

#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecord.h"



PortfolioTreeRecordFactory::PortfolioTreeRecordFactory() :
    IPortfolioTreeRecordFactory()
{
    qDebug() << "Create PortfolioTreeRecordFactory";
}

PortfolioTreeRecordFactory::~PortfolioTreeRecordFactory()
{
    qDebug() << "Destroy PortfolioTreeRecordFactory";
}

IPortfolioTreeRecord* PortfolioTreeRecordFactory::newInstance(
    IInstrumentsStorage* instrumentsStorage, CategoryTreeItem* categoryTreeItem, QObject* parent
) const
{
    return new PortfolioTreeRecord(instrumentsStorage, categoryTreeItem, parent);
}
