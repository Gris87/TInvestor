#include "src/threads/cleanup/cleanupthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::Gt;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_CleanupThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        configMock        = new StrictMock<ConfigMock>();
        stocksStorageMock = new StrictMock<StocksStorageMock>();

        thread = new CleanupThread(configMock, stocksStorageMock);
    }

    void TearDown() override
    {
        delete thread;
        delete configMock;
        delete stocksStorageMock;
    }

    CleanupThread*                 thread;
    StrictMock<ConfigMock>*        configMock;
    StrictMock<StocksStorageMock>* stocksStorageMock;
};



TEST_F(Test_CleanupThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_CleanupThread, Test_run)
{
    const InSequence seq;

    QMutex mutex;

    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillOnce(Return(12));
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, deleteObsoleteData(Gt(1704056400000)));

    thread->run();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
