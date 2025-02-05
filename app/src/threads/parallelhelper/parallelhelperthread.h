#pragma once



#include <QThread>



template<typename T>
class ParallelHelperThread : public QThread
{
public:
    typedef void (*ActionType)(QList<T> *array, int start, int end, void *additionalArgs);

    explicit ParallelHelperThread(
        ActionType action,
        QList<T> *array,
        int start,
        int end,
        void *additionalArgs,
        QObject *parent = nullptr
    ) :
        QThread(parent),
        mAction(action),
        mArray(array),
        mStart(start),
        mEnd(end),
        mAdditionalArgs(additionalArgs)
    {
    }

    ~ParallelHelperThread()
    {
    }

    ParallelHelperThread(const ParallelHelperThread &another) = delete;
    ParallelHelperThread& operator=(const ParallelHelperThread &another) = delete;

    void run() override
    {
        mAction(mArray, mStart, mEnd, mAdditionalArgs);
    }

private:
    ActionType mAction;
    QList<T>   *mArray;
    int        mStart;
    int        mEnd;
    void       *mAdditionalArgs;
};



template<typename T>
void processInParallel(
    QList<T> *array,
    void action(QList<T> *array, int start, int end, void *additionalArgs),
    void *additionalArgs = nullptr
)
{
    int cpuCount = QThread::idealThreadCount();

    int partSize = array->size() / cpuCount;
    int partTail = array->size() % cpuCount;

    int start = 0;
    int end = 0;

    QList<ParallelHelperThread<T> *> threads(cpuCount);

    for (int i = 0; i < cpuCount; ++i)
    {
        end += partSize;

        if (i < partTail)
        {
            ++end;
        }

        ParallelHelperThread<T> *thread = new ParallelHelperThread<T>(action, array, start, end, additionalArgs);
        thread->start();

        threads[i] = thread;

        start = end;
    }

    for (int i = 0; i < cpuCount; ++i)
    {
        ParallelHelperThread<T> *thread = threads.at(i);

        thread->wait();
        delete thread;
    }
}
