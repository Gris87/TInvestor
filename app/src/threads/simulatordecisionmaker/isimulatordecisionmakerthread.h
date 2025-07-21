#pragma once



#include <QThread>

#include "src/domain/log/logentry.h"
#include "src/domain/operation/operation.h"
#include "src/domain/portfolio/portfolio.h"



class ISimulatorDecisionMakerThread : public QThread
{
    Q_OBJECT

public:
    explicit ISimulatorDecisionMakerThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~ISimulatorDecisionMakerThread() override = default;

    ISimulatorDecisionMakerThread(const ISimulatorDecisionMakerThread& another)            = delete;
    ISimulatorDecisionMakerThread& operator=(const ISimulatorDecisionMakerThread& another) = delete;

    virtual void reset()           = 0;
    virtual void terminateThread() = 0;

signals:
    void operationsRead(const QList<Operation>& operations);
    void operationsAdded(const QList<Operation>& operations);
    void logsRead(const QList<LogEntry>& entries);
    void logAdded(const LogEntry& entry);
    void portfolioChanged(const Portfolio& portfolio);
};
