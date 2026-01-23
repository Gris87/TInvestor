#pragma once



#include <QThread>



class IDetectDividendsThread : public QThread
{
    Q_OBJECT

public:
    explicit IDetectDividendsThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IDetectDividendsThread() override = default;

    IDetectDividendsThread(const IDetectDividendsThread& another)            = delete;
    IDetectDividendsThread& operator=(const IDetectDividendsThread& another) = delete;

    virtual void terminateThread() = 0;
};
