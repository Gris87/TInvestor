#include "src/threads/portfolio/portfoliothread.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::StrictMock;



class Test_PortfolioThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        thread = new PortfolioThread(userStorageMock);
    }

    void TearDown() override
    {
        delete thread;
        delete userStorageMock;
    }

    PortfolioThread*             thread;
    StrictMock<UserStorageMock>* userStorageMock;
};



TEST_F(Test_PortfolioThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioThread, Test_run)
{
    // thread->run();
}
