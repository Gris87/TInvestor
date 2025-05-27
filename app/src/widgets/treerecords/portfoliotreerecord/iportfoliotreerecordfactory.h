#pragma once



#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecord.h"



class IPortfolioTreeRecordFactory
{
public:
    IPortfolioTreeRecordFactory()          = default;
    virtual ~IPortfolioTreeRecordFactory() = default;

    IPortfolioTreeRecordFactory(const IPortfolioTreeRecordFactory& another)            = delete;
    IPortfolioTreeRecordFactory& operator=(const IPortfolioTreeRecordFactory& another) = delete;

    virtual IPortfolioTreeRecord* newInstance(IInstrumentsStorage* instrumentsStorage, QObject* parent) const = 0;
};
