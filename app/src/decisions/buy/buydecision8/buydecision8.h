#pragma once



#include "src/decisions/iactiondecision.h"

#include "src/utils/bollindger/ibollindger.h"



class BuyDecision8 : public IActionDecision
{
public:
    explicit BuyDecision8(IBollindger* bollindger);
    ~BuyDecision8() override;

    BuyDecision8(const BuyDecision8& another)            = delete;
    BuyDecision8& operator=(const BuyDecision8& another) = delete;

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
        QThread* parentThread, IBuyDecision8Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex
    );

    IBollindger* mBollindger;
};
