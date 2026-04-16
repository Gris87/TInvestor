#pragma once



#include "src/decisions/iactiondecision.h"



class SellDecision1 : public IActionDecision
{
public:
    SellDecision1();
    ~SellDecision1() override;

    SellDecision1(const SellDecision1& another)            = delete;
    SellDecision1& operator=(const SellDecision1& another) = delete;

    QString makeDecision(
        QThread*              parentThread,
        IDecisionMakerConfig* config,
        qint64                limitTimestamp,
        Stock*                stock,
        bool                  dateRange,
        int                   dataIndex,
        bool                  isShort,
        float                 price,
        float                 avgPrice,
        float                 commission
    ) override;

    [[nodiscard]]
    AsapMode asapMode() const override;
};
