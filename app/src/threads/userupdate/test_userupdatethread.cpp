#include "src/threads/userupdate/userupdatethread.h"

#include <gtest/gtest.h>



class Test_UserUpdateThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        thread = new UserUpdateThread();
    }

    void TearDown()
    {
        delete thread;
    }

    UserUpdateThread* thread;
};



TEST_F(Test_UserUpdateThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_UserUpdateThread, Test_run)
{
}
