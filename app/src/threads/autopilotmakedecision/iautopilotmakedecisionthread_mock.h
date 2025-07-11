#pragma once



#include "src/threads/autopilotmakedecision/iautopilotmakedecisionthread.h"

#include <gmock/gmock.h>



class AutoPilotMakeDecisionThreadMock : public IAutoPilotMakeDecisionThread
{
    Q_OBJECT

public:
    explicit AutoPilotMakeDecisionThreadMock(QObject* parent = nullptr) :
        IAutoPilotMakeDecisionThread(parent)
    {
    }
    ~AutoPilotMakeDecisionThreadMock() override = default;

    AutoPilotMakeDecisionThreadMock(const AutoPilotMakeDecisionThreadMock& another)            = delete;
    AutoPilotMakeDecisionThreadMock& operator=(const AutoPilotMakeDecisionThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, setAccount, (const QString& accountId), (override));
    MOCK_METHOD(void, setKeepMoney, (int value), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
