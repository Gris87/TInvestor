#pragma once



#include "src/decisions/iactiondecision.h"

#include "src/utils/bollindger/ibollindger.h"



class BuyDecision5 : public IActionDecision
{
public:
    explicit BuyDecision5(IBollindger* bollindger);
    ~BuyDecision5() override;

    BuyDecision5(const BuyDecision5& another)            = delete;
    BuyDecision5& operator=(const BuyDecision5& another) = delete;

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
    [[nodiscard]]
    QString makeDecisionBasedOnStockData(
        QThread* parentThread, IBuyDecision5Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex
    ) const;

    [[nodiscard]]
    QString makeDecisionBasedOnStockOperationalData(
        QThread* parentThread, IBuyDecision5Config* buyConfig, qint64 limitTimestamp, Stock* stock
    ) const;

    IBollindger* mBollindger;
};
