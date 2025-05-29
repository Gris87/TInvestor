#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecord.h"

#include <QDebug>



PortfolioTreeRecord::PortfolioTreeRecord(
    IInstrumentWidgetFactory* instrumentWidgetFactory,
    IUserStorage*             userStorage,
    IInstrumentsStorage*      instrumentsStorage,
    CategoryTreeItem*         categoryTreeItem,
    const QString&            instrumentId,
    QObject*                  parent
) :
    IPortfolioTreeRecord(parent),
    mPortfolioTreeItem(
        new PortfolioTreeItem(categoryTreeItem, instrumentWidgetFactory, userStorage, instrumentsStorage, instrumentId)
    )
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
        mPortfolioTreeItem->setYieldPercent(item.yieldPercent, item.avgPrice);
        mPortfolioTreeItem->setDailyYieldPercent(item.dailyYieldPercent, item.priceForDailyYield);
    }

    mPortfolioTreeItem->setAvailable(item.available);
    mPortfolioTreeItem->setCost(item.cost);
    mPortfolioTreeItem->setPart(item.part);
}
