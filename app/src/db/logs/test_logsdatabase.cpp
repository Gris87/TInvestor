#include "src/db/logs/logsdatabase.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_LogsDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        dirFactoryMock  = new StrictMock<DirFactoryMock>();
        fileFactoryMock = new StrictMock<FileFactoryMock>();

        database = new LogsDatabase(dirFactoryMock, fileFactoryMock, true);
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
        delete fileFactoryMock;
    }

    LogsDatabase*                database;
    StrictMock<DirFactoryMock>*  dirFactoryMock;
    StrictMock<FileFactoryMock>* fileFactoryMock;
};



TEST_F(Test_LogsDatabase, Test_constructor_and_destructor)
{
}
