#pragma once



#include "src/decisions/iactiondecision.h"



class BuyDecision2 : public IActionDecision
{
public:
    BuyDecision2();
    ~BuyDecision2() override;

    BuyDecision2(const BuyDecision2& another)            = delete;
    BuyDecision2& operator=(const BuyDecision2& another) = delete;

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
    bool isAsap() const override;

#ifdef TESTING_MODE
    void testSetStepForTripleCheck(int step)
    {
        mStepForTripleCheck = step;
    }
#endif

private:
    int mStepForTripleCheck;
};
