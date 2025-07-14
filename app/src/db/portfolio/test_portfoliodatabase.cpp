#include "src/db/portfolio/portfoliodatabase.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_PortfolioDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        appDir = qApp->applicationDirPath();

        StrictMock<DirMock>* dirMock = new StrictMock<DirMock>(); // Will be deleted in PortfolioDatabase constructor
        dirFactoryMock               = new StrictMock<DirFactoryMock>();
        fileFactoryMock              = new StrictMock<FileFactoryMock>();
        logosStorageMock             = new StrictMock<LogosStorageMock>();

        EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
        EXPECT_CALL(*dirMock, mkpath(appDir + "/data/simulator")).WillOnce(Return(true));

        database = new PortfolioDatabase(dirFactoryMock, fileFactoryMock, logosStorageMock);
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
        delete fileFactoryMock;
        delete logosStorageMock;
    }

    PortfolioDatabase*            database;
    StrictMock<DirFactoryMock>*   dirFactoryMock;
    StrictMock<FileFactoryMock>*  fileFactoryMock;
    StrictMock<LogosStorageMock>* logosStorageMock;
    QString                       appDir;
};



TEST_F(Test_PortfolioDatabase, Test_constructor_and_destructor)
{
}
