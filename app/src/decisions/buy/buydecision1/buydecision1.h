#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision1 : public IActionDecision
{
public:
    BuyDecision1();
    ~BuyDecision1() override;

    BuyDecision1(const BuyDecision1& another)            = delete;
    BuyDecision1& operator=(const BuyDecision1& another) = delete;

    QString makeDecision(Stock* stock, bool dateRange, int dataIndex, float price) override;
};
