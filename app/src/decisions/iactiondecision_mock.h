#pragma once



#include "src/decisions/iactiondecision.h"

#include <gmock/gmock.h>



class ActionDecisionMock : public IActionDecision
{
public:
    ActionDecisionMock() :
        IActionDecision()
    {
    }
    ~ActionDecisionMock() override = default;

    ActionDecisionMock(const ActionDecisionMock& another)            = delete;
    ActionDecisionMock& operator=(const ActionDecisionMock& another) = delete;

    MOCK_METHOD(
        QString,
        makeDecision,
        (QThread * parentThread,
         IDecisionMakerConfig* config,
         qint64                limitTimestamp,
         Stock*                stock,
         bool                  dateRange,
         int                   dataIndex,
         float                 price,
         float                 avgPrice,
         float                 commission),
        (override)
    );
    MOCK_METHOD(bool, isAsap, (), (const, override));
};
