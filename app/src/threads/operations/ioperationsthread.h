#pragma once



#include <QThread>



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

    virtual void setAccount(const QString& account) = 0;
    virtual void terminateThread()                  = 0;

signals:
    void accountNotFound();
};
