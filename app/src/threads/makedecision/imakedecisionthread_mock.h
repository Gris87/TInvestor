#pragma once



#include "src/threads/makedecision/imakedecisionthread.h"

#include <gmock/gmock.h>



class MakeDecisionThreadMock : public IMakeDecisionThread
{
    Q_OBJECT

public:
    MakeDecisionThreadMock(QObject* parent = nullptr) :
        IMakeDecisionThread(parent)
    {
    }
    ~MakeDecisionThreadMock() override = default;

    MakeDecisionThreadMock(const MakeDecisionThreadMock& another)            = delete;
    MakeDecisionThreadMock& operator=(const MakeDecisionThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
};
