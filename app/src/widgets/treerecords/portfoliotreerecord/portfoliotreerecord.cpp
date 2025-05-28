#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecord.h"

#include <QDebug>



PortfolioTreeRecord::PortfolioTreeRecord(
    IInstrumentsStorage* instrumentsStorage, CategoryTreeItem* /*categoryTreeItem*/, QObject* parent
) :
    IPortfolioTreeRecord(parent),
    mInstrumentsStorage(instrumentsStorage)
{
    qDebug() << "Create PortfolioTreeRecord";
}

PortfolioTreeRecord::~PortfolioTreeRecord()
{
    qDebug() << "Destroy PortfolioTreeRecord";
}
