#include "src/threads/refreshthread.h"

#include <gtest/gtest.h>



TEST(Test_RefreshThread, Test_constructor_and_destructor)
{
    RefreshThread thread;
}

TEST(Test_RefreshThread, Test_process)
{
    RefreshThread thread;

    thread.process();
}
