#pragma once



#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecordfactory.h"



class PortfolioTreeRecordFactory : public IPortfolioTreeRecordFactory
{
public:
    PortfolioTreeRecordFactory();
    ~PortfolioTreeRecordFactory() override;

    PortfolioTreeRecordFactory(const PortfolioTreeRecordFactory& another)            = delete;
    PortfolioTreeRecordFactory& operator=(const PortfolioTreeRecordFactory& another) = delete;

    IPortfolioTreeRecord* newInstance(
        IInstrumentsStorage* instrumentsStorage, CategoryTreeItem* categoryTreeItem, const QString& instrumentId, QObject* parent
    ) const override;
};
