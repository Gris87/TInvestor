#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecord.h"

#include <QDebug>



PortfolioTreeRecord::PortfolioTreeRecord(
    IInstrumentsStorage* instrumentsStorage, CategoryTreeItem* categoryTreeItem, const QString& instrumentId, QObject* parent
) :
    IPortfolioTreeRecord(parent),
    mInstrumentsStorage(instrumentsStorage),
    mPortfolioTreeItem(new PortfolioTreeItem(categoryTreeItem, instrumentId))
{
    qDebug() << "Create PortfolioTreeRecord";
}

PortfolioTreeRecord::~PortfolioTreeRecord()
{
    qDebug() << "Destroy PortfolioTreeRecord";
}

void PortfolioTreeRecord::setPortfolioItem(const PortfolioItem& item)
{
    if (item.showPrices)
    {
        mPortfolioTreeItem->setPrice(item.price);
        mPortfolioTreeItem->setAvgPrice(item.avgPrice);
        mPortfolioTreeItem->setYield(item.yield);
        mPortfolioTreeItem->setYieldPercent(item.yieldPercent);
        mPortfolioTreeItem->setDailyYieldPercent(item.dailyYieldPercent);
    }

    mPortfolioTreeItem->setAvailable(item.available);
    mPortfolioTreeItem->setCost(item.cost);
    mPortfolioTreeItem->setPart(item.part);
}
