#include "src/threads/parallelhelper/parallelhelperthread.h"

#include <QMutex>
#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
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



static void
action(QThread* /*parentThread*/, int /*threadId*/, int* array, int /*size*/, int start, int end, void* additionalArgs)
{
    SumResult* sumResult = reinterpret_cast<SumResult*>(additionalArgs);

    int res = 0;

    for (int i = start; i < end; ++i)
    {
        res += array[i];
    }

    sumResult->mutex.lock();
    sumResult->result += res;
    sumResult->mutex.unlock();
}



class Test_ParallelHelperThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_ParallelHelperThread, Test_processInParallel)
{
    QList<int> array;
    array << 1 << 2 << 3;

    SumResult sumResult;
    processInParallel(QThread::currentThread(), array, action, &sumResult);

    ASSERT_EQ(sumResult.result, 6);

    const int datasize = 1000;
    array.resizeForOverwrite(datasize);

    int* arrayData = array.data();

    for (int i = 0; i < datasize; ++i)
    {
        arrayData[i] = i;
    }

    sumResult.result = 0;
    processInParallel(QThread::currentThread(), array, action, &sumResult);

    ASSERT_EQ(sumResult.result, 499500);
}

TEST_F(Test_ParallelHelperThread, Test_setCpuCount_and_getCpuCount)
{
    ASSERT_EQ(getCpuCount(), 1);
    setCpuCount("NORMAL");
    ASSERT_EQ(getCpuCount(), 1);
    setCpuCount("OPTIMAL");
    ASSERT_EQ(getCpuCount(), 1);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
