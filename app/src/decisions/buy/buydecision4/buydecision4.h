#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision4 : public IActionDecision
{
public:
    BuyDecision4();
    ~BuyDecision4() override;

    BuyDecision4(const BuyDecision4& another)            = delete;
    BuyDecision4& operator=(const BuyDecision4& another) = delete;

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

#ifdef TESTING_MODE
    void testSetStep(int step)
    {
        mStep = step;
    }
#endif

private:
    [[nodiscard]]
    QString makeDecisionBasedOnStockData(
        QThread* parentThread, IBuyDecision4Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex, float price
    ) const;

    [[nodiscard]]
    bool doubleCheck(QThread* parentThread, const StockData* stockData, int index, float maximumPrice) const;

    int mStep;
};
