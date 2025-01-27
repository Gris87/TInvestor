#pragma once



#include <QThread>



class RefreshThread : public QThread
{
    Q_OBJECT

public:
    RefreshThread(QObject *parent = nullptr);
    ~RefreshThread();

    RefreshThread(const RefreshThread &another) = delete;
    RefreshThread& operator=(const RefreshThread &another) = delete;

protected:
    void run() override;
};
