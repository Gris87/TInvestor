#pragma once



#include "src/threads/simulatordecisionmaker/isimulatordecisionmakerthread.h"

#include <gmock/gmock.h>



class SimulatorDecisionMakerThreadMock : public ISimulatorDecisionMakerThread
{
    Q_OBJECT

public:
    explicit SimulatorDecisionMakerThreadMock(QObject* parent = nullptr) :
        ISimulatorDecisionMakerThread(parent)
    {
    }
    ~SimulatorDecisionMakerThreadMock() override = default;

    SimulatorDecisionMakerThreadMock(const SimulatorDecisionMakerThreadMock& another)            = delete;
    SimulatorDecisionMakerThreadMock& operator=(const SimulatorDecisionMakerThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, reset, (), (override));
    MOCK_METHOD(void, setStartMoney, (int value), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
