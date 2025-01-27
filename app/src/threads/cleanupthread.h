#pragma once



#include <QThread>



class CleanupThread : public QThread
{
    Q_OBJECT

public:
    CleanupThread(QObject *parent = nullptr);
    ~CleanupThread();

    CleanupThread(const CleanupThread &another) = delete;
    CleanupThread& operator=(const CleanupThread &another) = delete;

protected:
    void run() override;
};
