#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision8 : public IActionDecision
{
public:
    BuyDecision8();
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
        QThread* parentThread, IBuyDecision8Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex, float price
    ) const;

    [[nodiscard]]
    bool doubleCheck(QThread* parentThread, const StockData* stockData, int index, qint64 limitTimestamp, float price) const;
};
