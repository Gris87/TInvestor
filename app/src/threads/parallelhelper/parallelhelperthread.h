#pragma once



#include <QThread>



template<typename T>
class ParallelHelperThread : public QThread
{
public:
    using ActionType = void (*)(QThread* parentThread, QList<T>& array, int start, int end, void* additionalArgs);

    explicit ParallelHelperThread(
        ActionType action,
        QThread*   parentThread,
        QList<T>&  array,
        int        start,
        int        end,
        void*      additionalArgs,
        QObject*   parent = nullptr
    ) :
        QThread(parent),
        mAction(action),
        mParentThread(parentThread),
        mArray(array),
        mStart(start),
        mEnd(end),
        mAdditionalArgs(additionalArgs)
    {
    }

    ~ParallelHelperThread() override = default;

    ParallelHelperThread(const ParallelHelperThread& another)            = delete;
    ParallelHelperThread& operator=(const ParallelHelperThread& another) = delete;

    void run() override
    {
        mAction(mParentThread, mArray, mStart, mEnd, mAdditionalArgs);
    }

private:
    ActionType mAction;
    QThread*   mParentThread;
    QList<T>&  mArray;
    int        mStart;
    int        mEnd;
    void*      mAdditionalArgs;
};



template<typename T>
void processInParallel(
    QList<T>& array,
    void      action(QThread* parentThread, QList<T>& array, int start, int end, void* additionalArgs),
    void*     additionalArgs = nullptr
)
{
    QThread* parentThread = QThread::currentThread();

    const int cpuCount = QThread::idealThreadCount();

    const int partSize = array.size() / cpuCount;
    const int partTail = array.size() % cpuCount;

    int start = 0;
    int end   = 0;

    QList<ParallelHelperThread<T>*> threads(cpuCount);

    for (int i = 0; i < cpuCount; ++i)
    {
        end += partSize;

        if (i < partTail)
        {
            ++end;
        }

        ParallelHelperThread<T>* thread = new ParallelHelperThread<T>(action, parentThread, array, start, end, additionalArgs);
        thread->start();

        threads[i] = thread;

        start = end;
    }

    for (int i = 0; i < cpuCount; ++i)
    {
        ParallelHelperThread<T>* thread = threads.at(i);

        thread->wait();
        delete thread;
    }
}
