#include "src/threads/bidirtradingcontrol/bidirtradingcontrolthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_BiDirTradingControlThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        userStorageMock   = new StrictMock<UserStorageMock>();
        configMock        = new StrictMock<ConfigMock>();
        grpcClientMock    = new StrictMock<GrpcClientMock>();

        thread = new BiDirTradingControlThread(stocksStorageMock, userStorageMock, configMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
        delete userStorageMock;
        delete configMock;
        delete grpcClientMock;
    }

    BiDirTradingControlThread*     thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<UserStorageMock>*   userStorageMock;
    StrictMock<ConfigMock>*        configMock;
    StrictMock<GrpcClientMock>*    grpcClientMock;
};



TEST_F(Test_BiDirTradingControlThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirTradingControlThread, Test_run)
{
}

TEST_F(Test_BiDirTradingControlThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
