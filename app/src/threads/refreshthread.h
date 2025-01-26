#pragma once



#include <QThread>



class RefreshThread : public QThread
{
    Q_OBJECT

public:
    RefreshThread(QObject *parent = nullptr);
    ~RefreshThread();

protected:
    void run() override;
};
