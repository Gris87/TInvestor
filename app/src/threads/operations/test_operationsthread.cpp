#include "src/threads/operations/operationsthread.h"

#include <gtest/gtest.h>

#include "src/db/operations/ioperationsdatabase_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"



using ::testing::StrictMock;



class Test_OperationsThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        operationsDatabaseMock = new StrictMock<OperationsDatabaseMock>();
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        logosStorageMock       = new StrictMock<LogosStorageMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();

        thread = new OperationsThread(operationsDatabaseMock, instrumentsStorageMock, logosStorageMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete operationsDatabaseMock;
        delete instrumentsStorageMock;
        delete logosStorageMock;
        delete grpcClientMock;
    }

    OperationsThread*                   thread;
    StrictMock<OperationsDatabaseMock>* operationsDatabaseMock;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<LogosStorageMock>*       logosStorageMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
};



TEST_F(Test_OperationsThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsThread, Test_run)
{
    // thread->run();
}
