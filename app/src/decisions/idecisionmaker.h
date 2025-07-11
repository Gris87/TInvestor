#pragma once



class IDecisionMaker
{
public:
    IDecisionMaker()          = default;
    virtual ~IDecisionMaker() = default;

    IDecisionMaker(const IDecisionMaker& another)            = delete;
    IDecisionMaker& operator=(const IDecisionMaker& another) = delete;
};
