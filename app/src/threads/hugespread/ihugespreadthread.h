#pragma once



#include <QThread>



class IHugeSpreadThread : public QThread
{
    Q_OBJECT

public:
    explicit IHugeSpreadThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IHugeSpreadThread() override = default;

    IHugeSpreadThread(const IHugeSpreadThread& another)            = delete;
    IHugeSpreadThread& operator=(const IHugeSpreadThread& another) = delete;

    virtual void terminateThread() = 0;
};
