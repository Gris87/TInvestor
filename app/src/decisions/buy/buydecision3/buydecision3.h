#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision3 : public IActionDecision
{
public:
    BuyDecision3();
    ~BuyDecision3() override;

    BuyDecision3(const BuyDecision3& another)            = delete;
    BuyDecision3& operator=(const BuyDecision3& another) = delete;

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

#ifdef TESTING_MODE
    void testSetStep(int step)
    {
        mStep = step;
    }
#endif

private:
    [[nodiscard]]
    QString makeDecisionBasedOnStockData(
        QThread* parentThread, IBuyDecision3Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex, float price
    ) const;

    [[nodiscard]]
    bool doubleCheck(QThread* parentThread, const StockData* stockData, int index, float maximumPrice) const;

    int mStep;
};
