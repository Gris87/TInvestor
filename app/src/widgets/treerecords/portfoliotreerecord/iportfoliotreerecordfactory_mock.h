#pragma once



#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecordfactory.h"

#include <gmock/gmock.h>



class PortfolioTreeRecordFactoryMock : public IPortfolioTreeRecordFactory
{
public:
    PortfolioTreeRecordFactoryMock() :
        IPortfolioTreeRecordFactory()
    {
    }
    ~PortfolioTreeRecordFactoryMock() override = default;

    PortfolioTreeRecordFactoryMock(const PortfolioTreeRecordFactoryMock& another)            = delete;
    PortfolioTreeRecordFactoryMock& operator=(const PortfolioTreeRecordFactoryMock& another) = delete;

    MOCK_METHOD(
        IPortfolioTreeRecord*, newInstance, (IInstrumentsStorage * instrumentsStorage, QObject* parent), (const, override)
    );
};
