#include "src/threads/parallelhelper/parallelhelperthread.h"

#include <gtest/gtest.h>



TEST(Test_ParallelHelperThread, Test_constructor_and_destructor)
{
    ParallelHelperThread thread;
}

TEST(Test_ParallelHelperThread, Test_run)
{
    ParallelHelperThread thread;

    thread.run();
}
