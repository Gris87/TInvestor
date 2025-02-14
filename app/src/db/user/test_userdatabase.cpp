#include "src/db/account/accountdatabase.h"

#include <gtest/gtest.h>



class Test_AccountDatabase : public ::testing::Test
{
protected:
    void SetUp()
    {
        QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/db/account").removeRecursively();

        database = new AccountDatabase();
    }

    void TearDown()
    {
        delete database;

        QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/db/account").removeRecursively();
    }

    AccountDatabase* database;
};



TEST_F(Test_AccountDatabase, Test_constructor_and_destructor)
{
}
