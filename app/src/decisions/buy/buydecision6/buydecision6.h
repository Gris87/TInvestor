#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision6 : public IActionDecision
{
public:
    BuyDecision6();
    ~BuyDecision6() override;

    BuyDecision6(const BuyDecision6& another)            = delete;
    BuyDecision6& operator=(const BuyDecision6& another) = delete;

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
        QThread* parentThread, IBuyDecision6Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex, float price
    ) const;

    [[nodiscard]]
    QList<float> getDayPrices(QThread* parentThread, Stock* stock, int dataIndex, int duration) const;

    [[nodiscard]]
    float calculateSma(const QList<float>& prices) const;

    [[nodiscard]]
    float calculateEma(const QList<float>& prices) const;

    [[nodiscard]]
    float calculateRsi(const QList<float>& prices) const;
};
