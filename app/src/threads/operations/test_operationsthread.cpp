#include "src/threads/operations/operationsthread.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::StrictMock;



class Test_OperationsThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        thread = new OperationsThread(userStorageMock);
    }

    void TearDown() override
    {
        delete thread;
        delete userStorageMock;
    }

    OperationsThread*            thread;
    StrictMock<UserStorageMock>* userStorageMock;
};



TEST_F(Test_OperationsThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsThread, Test_run)
{
    // thread->run();
}
