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
    IInstrumentWidgetFactory* instrumentWidgetFactory,
    IUserStorage*             userStorage,
    IInstrumentsStorage*      instrumentsStorage,
    CategoryTreeItem*         categoryTreeItem,
    const QString&            instrumentId,
    QObject*                  parent
) const
{
    return new PortfolioTreeRecord(
        instrumentWidgetFactory, userStorage, instrumentsStorage, categoryTreeItem, instrumentId, parent
    );
}
