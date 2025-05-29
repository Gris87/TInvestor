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
        IPortfolioTreeRecord*,
        newInstance,
        (IInstrumentWidgetFactory * instrumentWidgetFactory,
         IUserStorage*        userStorage,
         IInstrumentsStorage* instrumentsStorage,
         CategoryTreeItem*    categoryTreeItem,
         const QString&       instrumentId,
         QObject*             parent),
        (const, override)
    );
};
