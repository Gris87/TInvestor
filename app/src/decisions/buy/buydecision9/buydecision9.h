#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision9 : public IActionDecision
{
public:
    BuyDecision9();
    ~BuyDecision9() override;

    BuyDecision9(const BuyDecision9& another)            = delete;
    BuyDecision9& operator=(const BuyDecision9& another) = delete;

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
        QThread* parentThread, IBuyDecision9Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex, float price
    );

    QList<float> getDayPrices(QThread* parentThread, Stock* stock, int dataIndex, int duration);
    float        calculateSma(const QList<float>& prices);
    float        calculateEma(const QList<float>& prices);
    float        calculateRsi(const QList<float>& prices);
};
