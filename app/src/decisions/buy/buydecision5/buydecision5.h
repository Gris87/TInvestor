#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision5 : public IActionDecision
{
public:
    BuyDecision5();
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

#ifdef TESTING_MODE
    void testSetStep(int step)
    {
        mStep = step;
    }
#endif

private:
    int mStep;
};
