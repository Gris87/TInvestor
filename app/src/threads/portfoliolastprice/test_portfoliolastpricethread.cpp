#include "src/threads/portfoliolastprice/portfoliolastpricethread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::StrictMock;



class Test_PortfolioLastPriceThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        timeUtilsMock  = new StrictMock<TimeUtilsMock>();
        grpcClientMock = new StrictMock<GrpcClientMock>();

        thread = new PortfolioLastPriceThread(timeUtilsMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete timeUtilsMock;
        delete grpcClientMock;
    }

    PortfolioLastPriceThread*   thread;
    StrictMock<TimeUtilsMock>*  timeUtilsMock;
    StrictMock<GrpcClientMock>* grpcClientMock;
};



TEST_F(Test_PortfolioLastPriceThread, Test_constructor_and_destructor)
{
}
