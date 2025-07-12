#pragma once



#include <QThread>

#include "src/domain/log/logentry.h"
#include "src/domain/operation/operation.h"
#include "src/domain/portfolio/portfolio.h"



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

    virtual void reset()                  = 0;
    virtual void setStartMoney(int value) = 0;
    virtual void terminateThread()        = 0;

signals:
    void operationsRead(const QList<Operation>& operations);
    void operationsAdded(const QList<Operation>& operations);
    void logsRead(const QList<LogEntry>& entries);
    void logAdded(const LogEntry& entry);
    void portfolioChanged(const Portfolio& portfolio);
};
