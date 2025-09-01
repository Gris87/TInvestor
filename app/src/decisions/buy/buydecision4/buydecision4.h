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
    bool isAsap() const override;

#ifdef TESTING_MODE
    void testSetStep(int step)
    {
        mStep = step;
    }
#endif

private:
    int mStep;
};
