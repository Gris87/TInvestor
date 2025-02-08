#pragma once



#include <QThread>



class IMakeDecisionThread : public QThread
{
public:
    explicit IMakeDecisionThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    virtual ~IMakeDecisionThread() = default;

    IMakeDecisionThread(const IMakeDecisionThread& another)            = delete;
    IMakeDecisionThread& operator=(const IMakeDecisionThread& another) = delete;
};
