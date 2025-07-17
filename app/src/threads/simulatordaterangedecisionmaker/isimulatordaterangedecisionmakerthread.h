#pragma once



#include <QThread>

#include "src/domain/log/logentry.h"
#include "src/domain/operation/operation.h"
#include "src/domain/portfolio/portfolio.h"



class ISimulatorDateRangeDecisionMakerThread : public QThread
{
    Q_OBJECT

public:
    explicit ISimulatorDateRangeDecisionMakerThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~ISimulatorDateRangeDecisionMakerThread() override = default;

    ISimulatorDateRangeDecisionMakerThread(const ISimulatorDateRangeDecisionMakerThread& another)            = delete;
    ISimulatorDateRangeDecisionMakerThread& operator=(const ISimulatorDateRangeDecisionMakerThread& another) = delete;

    virtual void reset()                  = 0;
    virtual void setStartMoney(int value) = 0;
    virtual void terminateThread()        = 0;

signals:
    void operationsRead(const QList<Operation>& operations);
    void logsRead(const QList<LogEntry>& entries);
    void portfolioChanged(const Portfolio& portfolio);
};
