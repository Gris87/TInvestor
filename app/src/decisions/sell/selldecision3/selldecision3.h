#pragma once



#include "src/decisions/iactiondecision.h"



class SellDecision3 : public IActionDecision
{
public:
    SellDecision3();
    ~SellDecision3() override;

    SellDecision3(const SellDecision3& another)            = delete;
    SellDecision3& operator=(const SellDecision3& another) = delete;

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
        ISellDecision3Config* sellConfig,
        Stock*                stock,
        int                   dataIndex,
        float                 price,
        float                 avgPrice,
        float                 commission
    );
    QString makeDecisionBasedOnStockOperationalData(
        QThread* parentThread, ISellDecision3Config* sellConfig, Stock* stock, float price, float avgPrice, float commission
    );
    bool doubleCheckBasedOnStockData(QThread* parentThread, const StockData* stockData, int index, float minimumPrice);
    bool doubleCheckBasedOnStockOperationalData(
        QThread* parentThread, const StockOperationalData* stockOperationalData, int index, float minimumPrice
    );
};
