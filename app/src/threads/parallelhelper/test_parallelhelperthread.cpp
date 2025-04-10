#include "src/threads/parallelhelper/parallelhelperthread.h"

#include <QMutex>
#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
struct SumResult
{
    SumResult() :
        mutex(),
        result(0)
    {
    }

    QMutex mutex;
    int    result;
};



static void action(QThread* /*parentThread*/, QList<int>& array, int start, int end, void* additionalArgs)
{
    SumResult* sumResult = reinterpret_cast<SumResult*>(additionalArgs);

    int res = 0;

    const int* arrayData = array.constData();

    for (int i = start; i < end; ++i)
    {
        res += arrayData[i];
    }

    sumResult->mutex.lock();
    sumResult->result += res;
    sumResult->mutex.unlock();
}



TEST(Test_ParallelHelperThread, Test_processInParallel)
{
    QList<int> array;
    array << 1 << 2 << 3;

    SumResult sumResult;
    processInParallel(array, action, &sumResult);

    ASSERT_EQ(sumResult.result, 6);

    const int datasize = 1000;
    array.resize(datasize);

    int* arrayData = array.data();

    for (int i = 0; i < datasize; ++i)
    {
        arrayData[i] = i;
    }

    sumResult.result = 0;
    processInParallel(array, action, &sumResult);

    ASSERT_EQ(sumResult.result, 499500);
}
