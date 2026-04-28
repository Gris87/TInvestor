#pragma once



#include <QThread>



class IRequestThread : public QThread
{
    Q_OBJECT

public:
    explicit IRequestThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IRequestThread() override = default;

    IRequestThread(const IRequestThread& another)            = delete;
    IRequestThread& operator=(const IRequestThread& another) = delete;

    virtual void terminateThread() = 0;
};
