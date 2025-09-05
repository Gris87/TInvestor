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
         qint64                 timestamp,
         IConfig*               config,
         const InstrumentSells& instrumentSells,
         const Portfolio&       portfolio,
         const QList<Stock*>&   stocks,
         bool                   autoPilot,
         bool                   dateRange,
         bool                   useParallel),
        (override)
    );
};
