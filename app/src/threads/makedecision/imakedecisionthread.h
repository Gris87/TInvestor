#pragma once



#include <QThread>



class IMakeDecisionThread : public QThread
{
    Q_OBJECT

public:
    explicit IMakeDecisionThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IMakeDecisionThread() override = default;

    IMakeDecisionThread(const IMakeDecisionThread& another)            = delete;
    IMakeDecisionThread& operator=(const IMakeDecisionThread& another) = delete;
};
