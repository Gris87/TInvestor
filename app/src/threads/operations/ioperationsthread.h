#pragma once



#include <QThread>

#include "src/domain/operation/operation.h"



class IOperationsThread : public QThread
{
    Q_OBJECT

public:
    explicit IOperationsThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IOperationsThread() override = default;

    IOperationsThread(const IOperationsThread& another)            = delete;
    IOperationsThread& operator=(const IOperationsThread& another) = delete;

    virtual void setAccountId(const QString& account, const QString& accountId) = 0;
    virtual void terminateThread()                                              = 0;

signals:
    void operationsRead(const QList<Operation>& operations);
    void operationsAdded(const QList<Operation>& operations);
};
