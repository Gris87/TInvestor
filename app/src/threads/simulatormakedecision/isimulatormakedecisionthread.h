#pragma once



#include <QThread>



class ISimulatorMakeDecisionThread : public QThread
{
    Q_OBJECT

public:
    explicit ISimulatorMakeDecisionThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~ISimulatorMakeDecisionThread() override = default;

    ISimulatorMakeDecisionThread(const ISimulatorMakeDecisionThread& another)            = delete;
    ISimulatorMakeDecisionThread& operator=(const ISimulatorMakeDecisionThread& another) = delete;

    virtual void terminateThread() = 0;
};
