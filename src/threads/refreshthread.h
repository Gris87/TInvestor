#ifndef REFRESHTHREAD_H
#define REFRESHTHREAD_H

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



#endif // REFRESHTHREAD_H
