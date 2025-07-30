#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision3 : public IActionDecision
{
public:
    BuyDecision3();
    ~BuyDecision3() override;

    BuyDecision3(const BuyDecision3& another)            = delete;
    BuyDecision3& operator=(const BuyDecision3& another) = delete;

    QString
    makeDecision(IDecisionMakerConfig* config, Stock* stock, bool dateRange, int dataIndex, float price, float avgPrice) override;
};
