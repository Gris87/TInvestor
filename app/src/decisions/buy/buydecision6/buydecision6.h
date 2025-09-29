#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision6 : public IActionDecision
{
public:
    BuyDecision6();
    ~BuyDecision6() override;

    BuyDecision6(const BuyDecision6& another)            = delete;
    BuyDecision6& operator=(const BuyDecision6& another) = delete;

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

    [[nodiscard]]
    AsapMode asapMode() const override;
};
