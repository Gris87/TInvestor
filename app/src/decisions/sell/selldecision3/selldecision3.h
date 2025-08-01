#pragma once



#include "src/decisions/iactiondecision.h"



class SellDecision3 : public IActionDecision
{
public:
    SellDecision3();
    ~SellDecision3() override;

    SellDecision3(const SellDecision3& another)            = delete;
    SellDecision3& operator=(const SellDecision3& another) = delete;

    QString makeDecision(
        QThread*              parentThread,
        IDecisionMakerConfig* config,
        Stock*                stock,
        bool                  dateRange,
        int                   dataIndex,
        float                 price,
        float                 avgPrice,
        float                 commission
    ) override;
};
