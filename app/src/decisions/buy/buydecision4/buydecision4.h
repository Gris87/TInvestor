#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision4 : public IActionDecision
{
public:
    BuyDecision4();
    ~BuyDecision4() override;

    BuyDecision4(const BuyDecision4& another)            = delete;
    BuyDecision4& operator=(const BuyDecision4& another) = delete;

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
