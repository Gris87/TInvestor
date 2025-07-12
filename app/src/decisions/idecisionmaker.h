#pragma once



#include "src/domain/portfolio/portfolio.h"
#include "src/domain/stock/stock.h"



class IDecisionMaker
{
public:
    IDecisionMaker()          = default;
    virtual ~IDecisionMaker() = default;

    IDecisionMaker(const IDecisionMaker& another)            = delete;
    IDecisionMaker& operator=(const IDecisionMaker& another) = delete;

    virtual void makeDecision(const Portfolio& portfolio, const QList<Stock*>& stocks) = 0;
};
