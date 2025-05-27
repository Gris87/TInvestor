#pragma once



#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecord.h"

#include "src/storage/instruments/iinstrumentsstorage.h"



class PortfolioTreeRecord : public IPortfolioTreeRecord
{
    Q_OBJECT

public:
    explicit PortfolioTreeRecord(IInstrumentsStorage* instrumentsStorage, QObject* parent = nullptr);
    ~PortfolioTreeRecord() override;

    PortfolioTreeRecord(const PortfolioTreeRecord& another)            = delete;
    PortfolioTreeRecord& operator=(const PortfolioTreeRecord& another) = delete;

private:
    IInstrumentsStorage* mInstrumentsStorage;
};
