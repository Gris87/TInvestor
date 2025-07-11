#pragma once



#include "src/threads/simulatormakedecision/isimulatormakedecisionthread.h"

#include <gmock/gmock.h>



class SimulatorMakeDecisionThreadMock : public ISimulatorMakeDecisionThread
{
    Q_OBJECT

public:
    explicit SimulatorMakeDecisionThreadMock(QObject* parent = nullptr) :
        ISimulatorMakeDecisionThread(parent)
    {
    }
    ~SimulatorMakeDecisionThreadMock() override = default;

    SimulatorMakeDecisionThreadMock(const SimulatorMakeDecisionThreadMock& another)            = delete;
    SimulatorMakeDecisionThreadMock& operator=(const SimulatorMakeDecisionThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
