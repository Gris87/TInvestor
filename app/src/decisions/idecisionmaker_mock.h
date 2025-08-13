#pragma once



#include "src/decisions/idecisionmaker.h"

#include <gmock/gmock.h>



class DecisionMakerMock : public IDecisionMaker
{
public:
    DecisionMakerMock() :
        IDecisionMaker()
    {
    }
    ~DecisionMakerMock() override = default;

    DecisionMakerMock(const DecisionMakerMock& another)            = delete;
    DecisionMakerMock& operator=(const DecisionMakerMock& another) = delete;

    MOCK_METHOD(
        InstrumentsForTrading,
        makeDecision,
        (QThread * parentThread,
         qint64                  timestamp,
         IConfig*                config,
         const QList<Operation>& operations,
         const Portfolio&        portfolio,
         const QList<Stock*>&    stocks,
         bool                    autoPilot,
         int                     keepMoney,
         bool                    dateRange,
         bool                    useParallel),
        (override)
    );
};
