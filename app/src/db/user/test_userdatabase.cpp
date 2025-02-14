#include "src/db/user/userdatabase.h"

#include <gtest/gtest.h>



class Test_UserDatabase : public ::testing::Test
{
protected:
    void SetUp()
    {
        QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/db/user").removeRecursively();

        database = new UserDatabase();
    }

    void TearDown()
    {
        delete database;

        QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/db/user").removeRecursively();
    }

    UserDatabase* database;
};



TEST_F(Test_UserDatabase, Test_constructor_and_destructor)
{
}
