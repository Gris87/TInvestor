#pragma once



#include <QThread>



class IHighLiquidityThread : public QThread
{
    Q_OBJECT

public:
    explicit IHighLiquidityThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IHighLiquidityThread() override = default;

    IHighLiquidityThread(const IHighLiquidityThread& another)            = delete;
    IHighLiquidityThread& operator=(const IHighLiquidityThread& another) = delete;

    virtual void terminateThread() = 0;
};
