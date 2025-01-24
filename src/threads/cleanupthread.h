#ifndef CLEANUPTHREAD_H
#define CLEANUPTHREAD_H

#include <QThread>



class CleanupThread : public QThread
{
    Q_OBJECT

public:
    CleanupThread(QObject *parent = nullptr);
    ~CleanupThread();

protected:
    void run() override;
};



#endif // CLEANUPTHREAD_H
