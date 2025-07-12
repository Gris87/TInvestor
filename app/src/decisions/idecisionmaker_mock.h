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

    MOCK_METHOD(void, makeDecision, (const Portfolio& portfolio, const QList<Stock*>& stocks), (override));
};
