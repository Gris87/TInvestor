#pragma once



#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodel.h"



class IPortfolioTreeModelFactory
{
public:
    IPortfolioTreeModelFactory()          = default;
    virtual ~IPortfolioTreeModelFactory() = default;

    IPortfolioTreeModelFactory(const IPortfolioTreeModelFactory& another)            = delete;
    IPortfolioTreeModelFactory& operator=(const IPortfolioTreeModelFactory& another) = delete;

    virtual IPortfolioTreeModel* newInstance(QObject* parent) const = 0;
};
