#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision9 : public IActionDecision
{
public:
    BuyDecision9();
    ~BuyDecision9() override;

    BuyDecision9(const BuyDecision9& another)            = delete;
    BuyDecision9& operator=(const BuyDecision9& another) = delete;

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
