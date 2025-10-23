#pragma once



#include "src/utils/bollindger/ibollindger.h"

#include <gmock/gmock.h>



class BollindgerMock : public IBollindger
{
public:
    BollindgerMock() :
        IBollindger()
    {
    }
    ~BollindgerMock() override = default;

    BollindgerMock(const BollindgerMock& another)            = delete;
    BollindgerMock& operator=(const BollindgerMock& another) = delete;

    MOCK_METHOD(float, getTopEdge, (Stock * stock, int startIndex, int endIndex), (const, override));
    MOCK_METHOD(float, getBottomEdge, (Stock * stock, int startIndex, int endIndex), (const, override));
    MOCK_METHOD(float, getTopEdgeOperational, (Stock * stock, int startIndex, int endIndex), (const, override));
    MOCK_METHOD(float, getBottomEdgeOperational, (Stock * stock, int startIndex, int endIndex), (const, override));
};
