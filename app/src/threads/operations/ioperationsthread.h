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
};
