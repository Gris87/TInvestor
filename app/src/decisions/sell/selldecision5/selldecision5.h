#pragma once



#include "src/decisions/iactiondecision.h"

#include "src/utils/bollindger/ibollindger.h"



class SellDecision5 : public IActionDecision
{
public:
    explicit SellDecision5(IBollindger* bollindger);
    ~SellDecision5() override;

    SellDecision5(const SellDecision5& another)            = delete;
    SellDecision5& operator=(const SellDecision5& another) = delete;

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
        QThread* parentThread, ISellDecision5Config* sellConfig, qint64 limitTimestamp, Stock* stock, int dataIndex
    );

    IBollindger* mBollindger;
};
