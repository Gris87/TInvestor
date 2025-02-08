#pragma once



#include "src/threads/makedecision/imakedecisionthread.h"

#include <gmock/gmock.h>



class MakeDecisionThreadMock : public IMakeDecisionThread
{
public:
    MakeDecisionThreadMock() :
        IMakeDecisionThread()
    {
    }
    ~MakeDecisionThreadMock() override = default;

    MakeDecisionThreadMock(const MakeDecisionThreadMock& another)            = delete;
    MakeDecisionThreadMock& operator=(const MakeDecisionThreadMock& another) = delete;

    MOCK_METHOD0(run, void());
};
