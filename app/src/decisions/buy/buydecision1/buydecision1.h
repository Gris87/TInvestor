#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision1 : public IActionDecision
{
public:
    BuyDecision1();
    ~BuyDecision1() override;

    BuyDecision1(const BuyDecision1& another)            = delete;
    BuyDecision1& operator=(const BuyDecision1& another) = delete;

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
    void testSetStepForTripleCheck(int step)
    {
        mStepForTripleCheck = step;
    }
#endif

private:
    int mStepForTripleCheck;
};
