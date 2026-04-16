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
        bool                  isShort,
        float                 price,
        float                 avgPrice,
        float                 commission
    ) override;

    [[nodiscard]]
    AsapMode asapMode() const override;

private:
    [[nodiscard]]
    QString makeDecisionBasedOnStockData(
        QThread*              parentThread,
        ISellDecision4Config* sellConfig,
        qint64                limitTimestamp,
        Stock*                stock,
        int                   dataIndex,
        bool                  isShort,
        float                 price,
        float                 avgPrice,
        float                 commission
    ) const;

    [[nodiscard]]
    QString makeDecisionBasedOnStockOperationalData(
        QThread*              parentThread,
        ISellDecision4Config* sellConfig,
        qint64                limitTimestamp,
        Stock*                stock,
        bool                  isShort,
        float                 price,
        float                 avgPrice,
        float                 commission
    ) const;

    IBollindger* mBollindger;
};
