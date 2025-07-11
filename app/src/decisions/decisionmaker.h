#pragma once



#include "src/decisions/idecisionmaker.h"



class DecisionMaker : public IDecisionMaker
{
public:
    DecisionMaker();
    ~DecisionMaker() override;

    DecisionMaker(const DecisionMaker& another)            = delete;
    DecisionMaker& operator=(const DecisionMaker& another) = delete;
};
