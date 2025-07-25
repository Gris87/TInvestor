#pragma once



class IActionDecision
{
public:
    IActionDecision()          = default;
    virtual ~IActionDecision() = default;

    IActionDecision(const IActionDecision& another)            = delete;
    IActionDecision& operator=(const IActionDecision& another) = delete;

    virtual void makeDecision() = 0;
};
