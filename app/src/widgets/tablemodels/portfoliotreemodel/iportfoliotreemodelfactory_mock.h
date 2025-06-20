#pragma once



#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodelfactory.h"

#include <gmock/gmock.h>



class PortfolioTreeModelFactoryMock : public IPortfolioTreeModelFactory
{
public:
    PortfolioTreeModelFactoryMock() :
        IPortfolioTreeModelFactory()
    {
    }
    ~PortfolioTreeModelFactoryMock() override = default;

    PortfolioTreeModelFactoryMock(const PortfolioTreeModelFactoryMock& another)            = delete;
    PortfolioTreeModelFactoryMock& operator=(const PortfolioTreeModelFactoryMock& another) = delete;

    MOCK_METHOD(IPortfolioTreeModel*, newInstance, (QObject * parent), (const, override));
};
