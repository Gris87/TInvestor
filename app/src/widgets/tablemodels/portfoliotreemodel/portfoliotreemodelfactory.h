#pragma once



#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodelfactory.h"



class PortfolioTreeModelFactory : public IPortfolioTreeModelFactory
{
public:
    PortfolioTreeModelFactory();
    ~PortfolioTreeModelFactory() override;

    PortfolioTreeModelFactory(const PortfolioTreeModelFactory& another)            = delete;
    PortfolioTreeModelFactory& operator=(const PortfolioTreeModelFactory& another) = delete;

    IPortfolioTreeModel* newInstance(QObject* parent) const override;
};
