#pragma once



#include "src/decisions/iactiondecision.h"



class SellDecision4 : public IActionDecision
{
public:
    SellDecision4();
    ~SellDecision4() override;

    SellDecision4(const SellDecision4& another)            = delete;
    SellDecision4& operator=(const SellDecision4& another) = delete;

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
