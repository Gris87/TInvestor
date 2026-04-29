#pragma once



#include <QThread>



int getCpuCount();



template<typename T>
class ParallelHelperThread : public QThread
{
public:
    using ActionType =
        void (*)(QThread* parentThread, int threadId, T* array, int size, int start, int end, void* additionalArgs);

    explicit ParallelHelperThread(
        ActionType action,
        QThread*   parentThread,
        int        threadId,
        T*         array,
        int        size,
        int        start,
        int        end,
        void*      additionalArgs,
        QObject*   parent = nullptr
    ) :
        QThread(parent),
        mAction(action),
        mParentThread(parentThread),
        mThreadId(threadId),
        mArray(array),
        mSize(size),
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
        mAction(mParentThread, mThreadId, mArray, mSize, mStart, mEnd, mAdditionalArgs);
    }

private:
    ActionType mAction;
    QThread*   mParentThread;
    int        mThreadId;
    T*         mArray;
    int        mSize;
    int        mStart;
    int        mEnd;
    void*      mAdditionalArgs;
};



template<typename T>
void processInParallel(
    QThread*  parentThread,
    QList<T>& array,
    void      action(QThread* parentThread, int threadId, T* array, int size, int start, int end, void* additionalArgs),
    void*     additionalArgs = nullptr
)
{
#ifndef TESTING_MODE
    const int cpuCount = getCpuCount();

    T*  arrayData = array.data();
    int size      = array.size();
    int start     = 0;
    int end       = 0;

    const int partSize = size / cpuCount;
    const int partTail = size % cpuCount;

    QList<ParallelHelperThread<T>*> threads(cpuCount);

    for (int i = 0; i < cpuCount; ++i)
    {
        end += partSize;

        if (i < partTail)
        {
            ++end;
        }

        ParallelHelperThread<T>* thread =
            new ParallelHelperThread<T>(action, parentThread, i, arrayData, size, start, end, additionalArgs);
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
#else
    action(parentThread, 0, array.data(), array.size(), 0, array.size(), additionalArgs);
#endif
}
