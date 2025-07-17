#pragma once



#include "src/threads/simulatordaterangedecisionmaker/isimulatordaterangedecisionmakerthread.h"

#include <gmock/gmock.h>



class SimulatorDateRangeDecisionMakerThreadMock : public ISimulatorDateRangeDecisionMakerThread
{
    Q_OBJECT

public:
    explicit SimulatorDateRangeDecisionMakerThreadMock(QObject* parent = nullptr) :
        ISimulatorDateRangeDecisionMakerThread(parent)
    {
    }
    ~SimulatorDateRangeDecisionMakerThreadMock() override = default;

    SimulatorDateRangeDecisionMakerThreadMock(const SimulatorDateRangeDecisionMakerThreadMock& another)            = delete;
    SimulatorDateRangeDecisionMakerThreadMock& operator=(const SimulatorDateRangeDecisionMakerThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, reset, (), (override));
    MOCK_METHOD(void, setStartMoney, (int value), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
