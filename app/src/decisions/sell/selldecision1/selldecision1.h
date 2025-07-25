#pragma once



#include "src/decisions/iactiondecision.h"



class SellDecision1 : public IActionDecision
{
public:
    SellDecision1();
    ~SellDecision1() override;

    SellDecision1(const SellDecision1& another)            = delete;
    SellDecision1& operator=(const SellDecision1& another) = delete;

    void makeDecision() override;
};
