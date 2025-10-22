#pragma once



#include "src/decisions/iactiondecision.h"

#include "src/utils/bollindger/ibollindger.h"



class SellDecision4 : public IActionDecision
{
public:
    explicit SellDecision4(IBollindger* bollindger);
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

    [[nodiscard]]
    AsapMode asapMode() const override;

private:
    QString makeDecisionBasedOnStockData(
        QThread*              parentThread,
        ISellDecision4Config* sellConfig,
        Stock*                stock,
        int                   dataIndex,
        float                 price,
        float                 avgPrice,
        float                 commission
    );

    IBollindger* mBollindger;
};
