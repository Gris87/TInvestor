#pragma once



#include "src/threads/autopilotdecisionmaker/iautopilotdecisionmakerthread.h"

#include <gmock/gmock.h>



class AutoPilotDecisionMakerThreadMock : public IAutoPilotDecisionMakerThread
{
    Q_OBJECT

public:
    explicit AutoPilotDecisionMakerThreadMock(QObject* parent = nullptr) :
        IAutoPilotDecisionMakerThread(parent)
    {
    }
    ~AutoPilotDecisionMakerThreadMock() override = default;

    AutoPilotDecisionMakerThreadMock(const AutoPilotDecisionMakerThreadMock& another)            = delete;
    AutoPilotDecisionMakerThreadMock& operator=(const AutoPilotDecisionMakerThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, setAccountId, (const QString& accountId), (override));
    MOCK_METHOD(void, setKeepMoney, (int value), (override));
    MOCK_METHOD(void, notifyAboutSell, (const QString& instrumentId), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
