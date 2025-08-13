#pragma once



#include "src/decisions/iactiondecision.h"



class SellDecision2 : public IActionDecision
{
public:
    SellDecision2();
    ~SellDecision2() override;

    SellDecision2(const SellDecision2& another)            = delete;
    SellDecision2& operator=(const SellDecision2& another) = delete;

    QString makeDecision(
        QThread*              parentThread,
        IDecisionMakerConfig* config,
        qint64                limitTimestamp,
        Stock*                stock,
        bool                  dateRange,
        int                   dataIndex,
        float                 price,
        float                 avgPrice,
        float                 commission
    ) override;
};
