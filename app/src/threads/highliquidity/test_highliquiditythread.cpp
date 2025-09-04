#include "src/threads/highliquidity/highliquiditythread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_HighLiquidityThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        configMock     = new StrictMock<ConfigMock>();
        timeUtilsMock  = new StrictMock<TimeUtilsMock>();
        grpcClientMock = new StrictMock<GrpcClientMock>();

        thread = new HighLiquidityThread(configMock, timeUtilsMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete configMock;
        delete timeUtilsMock;
        delete grpcClientMock;
    }

    HighLiquidityThread*        thread;
    StrictMock<ConfigMock>*     configMock;
    StrictMock<TimeUtilsMock>*  timeUtilsMock;
    StrictMock<GrpcClientMock>* grpcClientMock;
};



TEST_F(Test_HighLiquidityThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_HighLiquidityThread, Test_run)
{
}

TEST_F(Test_HighLiquidityThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
