#include "src/threads/cleanupthread.h"

#include <gtest/gtest.h>



TEST(Test_CleanupThread, Test_constructor_and_destructor)
{
    CleanupThread thread;
}

TEST(Test_CleanupThread, Test_process)
{
    CleanupThread thread;

    thread.process();
}
