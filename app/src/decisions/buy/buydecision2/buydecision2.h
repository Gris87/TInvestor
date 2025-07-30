#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision2 : public IActionDecision
{
public:
    BuyDecision2();
    ~BuyDecision2() override;

    BuyDecision2(const BuyDecision2& another)            = delete;
    BuyDecision2& operator=(const BuyDecision2& another) = delete;

    QString
    makeDecision(IDecisionMakerConfig* config, Stock* stock, bool dateRange, int dataIndex, float price, float avgPrice) override;
};
