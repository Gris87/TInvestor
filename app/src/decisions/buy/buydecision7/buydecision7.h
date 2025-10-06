#pragma once



#include "src/decisions/iactiondecision.h"

#include "src/utils/timeutils/itimeutils.h"



class BuyDecision7 : public IActionDecision
{
public:
    explicit BuyDecision7(ITimeUtils* timeUtils);
    ~BuyDecision7() override;

    BuyDecision7(const BuyDecision7& another)            = delete;
    BuyDecision7& operator=(const BuyDecision7& another) = delete;

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
        QThread* parentThread, IBuyDecision7Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex
    );

    ITimeUtils* mTimeUtils;
};
