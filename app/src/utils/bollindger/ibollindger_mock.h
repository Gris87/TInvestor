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

    MOCK_METHOD(float, getTopEdge, (), (const, override));
    MOCK_METHOD(float, getBottomEdge, (), (const, override));
};
