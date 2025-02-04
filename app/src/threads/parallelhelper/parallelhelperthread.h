#pragma once



#include <QThread>



class ParallelHelperThread : public QThread
{
    Q_OBJECT

public:
    explicit ParallelHelperThread(QObject *parent = nullptr);
    ~ParallelHelperThread();

    ParallelHelperThread(const ParallelHelperThread &another) = delete;
    ParallelHelperThread& operator=(const ParallelHelperThread &another) = delete;

    void run() override;
};
