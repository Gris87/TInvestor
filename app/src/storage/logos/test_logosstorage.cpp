#include "src/storage/logos/logosstorage.h"

#include <gtest/gtest.h>

#include "src/db/logos/ilogosdatabase_mock.h"



using ::testing::StrictMock;



class Test_LogosStorage : public ::testing::Test
{
protected:
    void SetUp() override
    {
        logosDatabaseMock = new StrictMock<LogosDatabaseMock>();

        storage = new LogosStorage(logosDatabaseMock);
    }

    void TearDown() override
    {
        delete storage;
        delete logosDatabaseMock;
    }

    LogosStorage*                  storage;
    StrictMock<LogosDatabaseMock>* logosDatabaseMock;
};



TEST_F(Test_LogosStorage, Test_constructor_and_destructor)
{
}
